/*	$OpenBSD: machdep.c,v 1.135 2004/09/15 21:32:43 mickey Exp $	*/

/*
 * Copyright (c) 1999-2003 Michael Shalayeff
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR OR HIS RELATIVES BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF MIND, USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/signalvar.h>
#include <sys/kernel.h>
#include <sys/proc.h>
#include <sys/buf.h>
#include <sys/reboot.h>
#include <sys/device.h>
#include <sys/conf.h>
#include <sys/file.h>
#include <sys/timeout.h>
#include <sys/malloc.h>
#include <sys/mbuf.h>
#include <sys/msgbuf.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include <sys/user.h>
#include <sys/exec.h>
#include <sys/sysctl.h>
#include <sys/core.h>
#include <sys/kcore.h>
#include <sys/extent.h>
#ifdef SYSVMSG
#include <sys/msg.h>
#endif

#include <sys/mount.h>
#include <sys/syscallargs.h>

#include <uvm/uvm.h>
#include <uvm/uvm_page.h>

#include <dev/cons.h>

#include <machine/pdc.h>
#include <machine/iomod.h>
#include <machine/psl.h>
#include <machine/reg.h>
#include <machine/cpufunc.h>
#include <machine/autoconf.h>
#include <machine/kcore.h>

#ifdef COMPAT_HPUX
#include <compat/hpux/hpux.h>
#endif

#ifdef DDB
#include <machine/db_machdep.h>
#include <ddb/db_access.h>
#include <ddb/db_sym.h>
#include <ddb/db_extern.h>
#endif

#include <hppa/dev/cpudevs.h>

/*
 * Patchable buffer cache parameters
 */
#ifdef NBUF
int nbuf = NBUF;
#else
int nbuf = 0;
#endif

#ifndef BUFCACHEPERCENT
#define BUFCACHEPERCENT 10
#endif /* BUFCACHEPERCENT */

#ifdef BUFPAGES
int bufpages = BUFPAGES;
#else
int bufpages = 0;
#endif
int bufcachepercent = BUFCACHEPERCENT;

/*
 * Different kinds of flags used throughout the kernel.
 */
int cold = 1;			/* unset when engine is up to go */
extern int msgbufmapped;	/* set when safe to use msgbuf */

/*
 * cache configuration, for most machines is the same
 * numbers, so it makes sense to do defines w/ numbers depending
 * on configured cpu types in the kernel
 */
int icache_stride, icache_line_mask;
int dcache_stride, dcache_line_mask;

/*
 * things to not kill
 */
volatile u_int8_t *machine_ledaddr;
int machine_ledword, machine_leds;

/*
 * CPU params (should be the same for all cpus in the system)
 */
struct pdc_cache pdc_cache PDC_ALIGNMENT;
struct pdc_btlb pdc_btlb PDC_ALIGNMENT;
struct pdc_model pdc_model PDC_ALIGNMENT;

	/* w/ a little deviation should be the same for all installed cpus */
u_int	cpu_ticksnum, cpu_ticksdenom;

	/* exported info */
char	machine[] = MACHINE_ARCH;
char	cpu_model[128];
enum hppa_cpu_type cpu_type;
const char *cpu_typename;
int	cpu_hvers;
u_int	fpu_version;
#ifdef COMPAT_HPUX
int	cpu_model_hpux;	/* contains HPUX_SYSCONF_CPU* kind of value */
#endif

/*
 * exported methods for cpus
 */
int (*cpu_desidhash)(void);
int (*cpu_hpt_init)(vaddr_t hpt, vsize_t hptsize);
int (*cpu_ibtlb_ins)(int i, pa_space_t sp, vaddr_t va, paddr_t pa,
	    vsize_t sz, u_int prot);
int (*cpu_dbtlb_ins)(int i, pa_space_t sp, vaddr_t va, paddr_t pa,
	    vsize_t sz, u_int prot);

dev_t	bootdev;
int	totalphysmem, resvmem, physmem, esym;
paddr_t	avail_end;

/*
 * Things for MI glue to stick on.
 */
struct user *proc0paddr;
long mem_ex_storage[EXTENT_FIXED_STORAGE_SIZE(32) / sizeof(long)];
struct extent *hppa_ex;

struct vm_map *exec_map = NULL;
struct vm_map *phys_map = NULL;
/* Virtual page frame for /dev/mem (see mem.c) */
vaddr_t vmmap;

void delay_init(void);
static __inline void fall(int, int, int, int, int);
void dumpsys(void);
void hpmc_dump(void);
void cpuid(void);

/*
 * wide used hardware params
 */
struct pdc_hwtlb pdc_hwtlb PDC_ALIGNMENT;
struct pdc_coproc pdc_coproc PDC_ALIGNMENT;
struct pdc_coherence pdc_coherence PDC_ALIGNMENT;
struct pdc_spidb pdc_spidbits PDC_ALIGNMENT;
struct pdc_model pdc_model PDC_ALIGNMENT;

#ifdef DEBUG
int sigdebug = 0;
pid_t sigpid = 0;
#define SDB_FOLLOW	0x01
#endif

/*
 * Whatever CPU types we support
 */
extern const u_int itlb_x[], itlbna_x[], dtlb_x[], dtlbna_x[], tlbd_x[];
extern const u_int itlb_s[], itlbna_s[], dtlb_s[], dtlbna_s[], tlbd_s[];
extern const u_int itlb_t[], itlbna_t[], dtlb_t[], dtlbna_t[], tlbd_t[];
extern const u_int itlb_l[], itlbna_l[], dtlb_l[], dtlbna_l[], tlbd_l[];
extern const u_int itlb_u[], itlbna_u[], dtlb_u[], dtlbna_u[], tlbd_u[];
int iibtlb_s(int i, pa_space_t sp, vaddr_t va, paddr_t pa,
    vsize_t sz, u_int prot);
int idbtlb_s(int i, pa_space_t sp, vaddr_t va, paddr_t pa,
    vsize_t sz, u_int prot);
int ibtlb_t(int i, pa_space_t sp, vaddr_t va, paddr_t pa,
    vsize_t sz, u_int prot);
int ibtlb_l(int i, pa_space_t sp, vaddr_t va, paddr_t pa,
    vsize_t sz, u_int prot);
int ibtlb_u(int i, pa_space_t sp, vaddr_t va, paddr_t pa,
    vsize_t sz, u_int prot);
int ibtlb_g(int i, pa_space_t sp, vaddr_t va, paddr_t pa,
    vsize_t sz, u_int prot);
int pbtlb_g(int i);
int pbtlb_u(int i);
int hpti_l(vaddr_t, vsize_t);
int hpti_u(vaddr_t, vsize_t);
int hpti_g(vaddr_t, vsize_t);
int desidhash_x(void);
int desidhash_s(void);
int desidhash_t(void);
int desidhash_l(void);
int desidhash_u(void);
int desidhash_g(void);
const struct hppa_cpu_typed {
	char name[8];
	enum hppa_cpu_type type;
	int  cpuid;
	int  features;
	int  patch;
	int  (*desidhash)(void);
	int  (*dbtlbins)(int i, pa_space_t sp, vaddr_t va, paddr_t pa,
	     vsize_t sz, u_int prot);
	int  (*ibtlbins)(int i, pa_space_t sp, vaddr_t va, paddr_t pa,
	     vsize_t sz, u_int prot);
	int  (*btlbprg)(int i);
	int  (*hptinit)(vaddr_t hpt, vsize_t hptsize);
} cpu_types[] = {
#ifdef HP7000_CPU
	{ "PCXS",  hpcxs,  0, 0, 3, desidhash_s, ibtlb_g, NULL, pbtlb_g},
#endif
#ifdef HP7100_CPU
	{ "PCXT",  hpcxt, 0, HPPA_FTRS_BTLBU,
	  2, desidhash_t, ibtlb_g, NULL, pbtlb_g},
#endif
#ifdef HP7200_CPU
	{ "PCXT'", hpcxta,HPPA_CPU_PCXT2, HPPA_FTRS_BTLBU,
	  2, desidhash_t, ibtlb_g, NULL, pbtlb_g},
#endif
#ifdef HP7100LC_CPU
	{ "PCXL",  hpcxl, HPPA_CPU_PCXL, HPPA_FTRS_BTLBU|HPPA_FTRS_HVT,
	  0, desidhash_l, ibtlb_g, NULL, pbtlb_g, hpti_g},
#endif
#ifdef HP7300LC_CPU
	{ "PCXL2", hpcxl2,HPPA_CPU_PCXL2, HPPA_FTRS_BTLBU|HPPA_FTRS_HVT,
	  0, desidhash_l, ibtlb_g, NULL, pbtlb_g, hpti_g},
#endif
#ifdef HP8000_CPU
	{ "PCXU",  hpcxu, HPPA_CPU_PCXU, HPPA_FTRS_W32B,
	  4, desidhash_g, ibtlb_u, NULL, pbtlb_g },
#endif
#ifdef HP8200_CPU
	{ "PCXU+", hpcxu2,HPPA_CPU_PCXUP, HPPA_FTRS_W32B,
	  4, desidhash_g, ibtlb_u, NULL, pbtlb_u },
#endif
#ifdef HP8500_CPU
	{ "PCXW",  hpcxw, HPPA_CPU_PCXW, HPPA_FTRS_W32B,
	  4, desidhash_g, ibtlb_u, NULL, pbtlb_u },
#endif
	{ "", 0 }
};

int
hppa_cpuspeed(int *mhz)
{
	*mhz = PAGE0->mem_10msec / 10000;

	return (0);
}

void
hppa_init(start)
	paddr_t start;
{
	extern u_long cpu_hzticks;
	extern int kernel_text;
	vaddr_t v, v1;
	int error;

	pdc_init();	/* init PDC iface, so we can call em easy */

	cpu_hzticks = (PAGE0->mem_10msec * 100) / hz;
	delay_init();	/* calculate cpu clock ratio */

	/* cache parameters */
	if ((error = pdc_call((iodcio_t)pdc, 0, PDC_CACHE, PDC_CACHE_DFLT,
	    &pdc_cache)) < 0) {
#ifdef DEBUG
		printf("WARNING: PDC_CACHE error %d\n", error);
#endif
	}

	dcache_line_mask = pdc_cache.dc_conf.cc_line * 16 - 1;
	dcache_stride = pdc_cache.dc_stride;
	icache_line_mask = pdc_cache.ic_conf.cc_line * 16 - 1;
	icache_stride = pdc_cache.ic_stride;

	/* cache coherence params (pbably available for 8k only) */
	error = pdc_call((iodcio_t)pdc, 0, PDC_CACHE, PDC_CACHE_SETCS,
	    &pdc_coherence, 1, 1, 1, 1);
#ifdef DEBUG
	printf ("PDC_CACHE_SETCS: %d, %d, %d, %d (%d)\n",
	    pdc_coherence.ia_cst, pdc_coherence.da_cst,
	    pdc_coherence.ita_cst, pdc_coherence.dta_cst, error);
#endif
	error = pdc_call((iodcio_t)pdc, 0, PDC_CACHE, PDC_CACHE_GETSPIDB,
	    &pdc_spidbits, 0, 0, 0, 0);
	printf("SPID bits: 0x%x, error = %d\n", pdc_spidbits.spidbits, error);

	/* setup hpmc handler */
	{
		extern u_int hpmc_v[];	/* from locore.s */
		register u_int *p = hpmc_v;

		if (pdc_call((iodcio_t)pdc, 0, PDC_INSTR, PDC_INSTR_DFLT, p))
			*p = 0x08000240;

		p[6] = (u_int)&hpmc_dump;
		p[7] = 32;
		p[5] = -(p[0] + p[1] + p[2] + p[3] + p[4] + p[6] + p[7]);
	}

	{
		extern u_int hppa_toc[], hppa_toc_end[];
		register u_int cksum, *p;

		for (cksum = 0, p = hppa_toc; p < hppa_toc_end; p++)
			cksum += *p;

		*p = cksum;
		PAGE0->ivec_toc = (int (*)(void))hppa_toc;
		PAGE0->ivec_toclen = (hppa_toc_end - hppa_toc + 1) * 4;
	}

	{
		extern u_int hppa_pfr[], hppa_pfr_end[];
		register u_int cksum, *p;

		for (cksum = 0, p = hppa_pfr; p < hppa_pfr_end; p++)
			cksum += *p;

		*p = cksum;
		PAGE0->ivec_mempf = (int (*)(void))hppa_pfr;
		PAGE0->ivec_mempflen = (hppa_pfr_end - hppa_pfr + 1) * 4;
	}

	cpuid();
	ptlball();
	ficacheall();
	fdcacheall();

	avail_end = trunc_page(PAGE0->imm_max_mem);
	if (avail_end > SYSCALLGATE)
		avail_end = SYSCALLGATE;
	totalphysmem = btoc(avail_end);
	resvmem = btoc(((vaddr_t)&kernel_text));

	/* we hope this won't fail */
	hppa_ex = extent_create("mem", 0x0, 0xffffffff, M_DEVBUF,
	    (caddr_t)mem_ex_storage, sizeof(mem_ex_storage),
	    EX_NOCOALESCE|EX_NOWAIT);
	if (extent_alloc_region(hppa_ex, 0, (vaddr_t)PAGE0->imm_max_mem,
	    EX_NOWAIT))
		panic("cannot reserve main memory");

	/*
	 * Now allocate kernel dynamic variables
	 */

	/* buffer cache parameters */
	if (bufpages == 0)
		bufpages = totalphysmem / 100 *
		    (totalphysmem <= 0x1000? 5 : bufcachepercent);

	if (nbuf == 0)
		nbuf = bufpages < 16? 16 : bufpages;

	/* Restrict to at most 70% filled kvm */
	if (nbuf * MAXBSIZE >
	    (VM_MAX_KERNEL_ADDRESS-VM_MIN_KERNEL_ADDRESS) * 7 / 10)
		nbuf = (VM_MAX_KERNEL_ADDRESS-VM_MIN_KERNEL_ADDRESS) /
		    MAXBSIZE * 7 / 10;

	/* More buffer pages than fits into the buffers is senseless. */
	if (bufpages > nbuf * MAXBSIZE / PAGE_SIZE)
		bufpages = nbuf * MAXBSIZE / PAGE_SIZE;

	v1 = v = hppa_round_page(start);
#define valloc(name, type, num) (name) = (type *)v; v = (vaddr_t)((name)+(num))

	valloc(buf, struct buf, nbuf);

#ifdef SYSVMSG
	valloc(msgpool, char, msginfo.msgmax);
	valloc(msgmaps, struct msgmap, msginfo.msgseg);
	valloc(msghdrs, struct msg, msginfo.msgtql);
	valloc(msqids, struct msqid_ds, msginfo.msgmni);
#endif
#undef valloc
	v = hppa_round_page(v);
	bzero ((void *)v1, (v - v1));

	msgbufp = (struct msgbuf *)v;
	v += round_page(MSGBUFSIZE);
	bzero(msgbufp, MSGBUFSIZE);

	/* sets physmem */
	pmap_bootstrap(v);

	msgbufmapped = 1;
	initmsgbuf((caddr_t)msgbufp, round_page(MSGBUFSIZE));

	/* they say PDC_COPROC might turn fault light on */
	pdc_call((iodcio_t)pdc, 0, PDC_CHASSIS, PDC_CHASSIS_DISP,
	    PDC_OSTAT(PDC_OSTAT_RUN) | 0xCEC0);

	cpu_cpuspeed = &hppa_cpuspeed;
#ifdef DDB
	ddb_init();
#endif
	ficacheall();
	fdcacheall();
}

void
cpuid()
{
	/*
	 * Ptrs to various tlb handlers, to be filled
	 * based on cpu features.
	 * from locore.S
	 */
	extern u_int trap_ep_T_TLB_DIRTY[];
	extern u_int trap_ep_T_DTLBMISS[];
	extern u_int trap_ep_T_DTLBMISSNA[];
	extern u_int trap_ep_T_ITLBMISS[];
	extern u_int trap_ep_T_ITLBMISSNA[];

	extern u_int fpu_enable;
	extern int cpu_fpuena;
	struct pdc_cpuid pdc_cpuid PDC_ALIGNMENT;
	const struct hppa_cpu_typed *p = NULL;
	u_int cpu_features;
	int error;

	/* may the scientific guessing begin */
	cpu_features = 0;
	cpu_type = 0;

	/* identify system type */
	if ((error = pdc_call((iodcio_t)pdc, 0, PDC_MODEL, PDC_MODEL_INFO,
	    &pdc_model)) < 0) {
#ifdef DEBUG
		printf("WARNING: PDC_MODEL error %d\n", error);
#endif
		pdc_model.hvers = 0;
	}

	bzero(&pdc_cpuid, sizeof(pdc_cpuid));
	if (pdc_call((iodcio_t)pdc, 0, PDC_MODEL, PDC_MODEL_CPUID,
	    &pdc_cpuid, 0, 0, 0, 0) >= 0) {

		/* patch for old 8200 */
		if (pdc_cpuid.version == HPPA_CPU_PCXU &&
		    pdc_cpuid.revision > 0x0d)
			pdc_cpuid.version = HPPA_CPU_PCXUP;

		cpu_type = pdc_cpuid.version;
	}

	/* locate coprocessors and SFUs */
	bzero(&pdc_coproc, sizeof(pdc_coproc));
	if ((error = pdc_call((iodcio_t)pdc, 0, PDC_COPROC, PDC_COPROC_DFLT,
	    &pdc_coproc, 0, 0, 0, 0)) < 0) {
		printf("WARNING: PDC_COPROC error %d\n", error);
		cpu_fpuena = 0;
	} else {
		printf("pdc_coproc: 0x%x, 0x%x; model %x rev %x\n",
		    pdc_coproc.ccr_enable, pdc_coproc.ccr_present,
		    pdc_coproc.fpu_model, pdc_coproc.fpu_revision);
		fpu_enable = pdc_coproc.ccr_enable & CCR_MASK;
		cpu_fpuena = 1;

		/* a kludge to detect PCXW */
		if (pdc_coproc.fpu_model == HPPA_FPU_PCXW)
			cpu_type = HPPA_CPU_PCXW;
	}

	/* BTLB params */
	if ((error = pdc_call((iodcio_t)pdc, 0, PDC_BLOCK_TLB,
	    PDC_BTLB_DEFAULT, &pdc_btlb)) < 0) {
#ifdef DEBUG
		printf("WARNING: PDC_BTLB error %d\n", error);
#endif
	} else {
#ifdef BTLBDEBUG
		printf("btlb info: minsz=%d, maxsz=%d\n",
		    pdc_btlb.min_size, pdc_btlb.max_size);
		printf("btlb fixed: i=%d, d=%d, c=%d\n",
		    pdc_btlb.finfo.num_i,
		    pdc_btlb.finfo.num_d,
		    pdc_btlb.finfo.num_c);
		printf("btlb varbl: i=%d, d=%d, c=%d\n",
		    pdc_btlb.vinfo.num_i,
		    pdc_btlb.vinfo.num_d,
		    pdc_btlb.vinfo.num_c);
#endif /* BTLBDEBUG */
		/* purge TLBs and caches */
		if (pdc_call((iodcio_t)pdc, 0, PDC_BLOCK_TLB,
		    PDC_BTLB_PURGE_ALL) < 0)
			printf("WARNING: BTLB purge failed\n");

		if (pdc_btlb.finfo.num_c)
			cpu_features |= HPPA_FTRS_BTLBU;
	}

	if (!pdc_call((iodcio_t)pdc, 0, PDC_TLB, PDC_TLB_INFO, &pdc_hwtlb) &&
	    pdc_hwtlb.min_size && pdc_hwtlb.max_size) {
		cpu_features |= HPPA_FTRS_HVT;
		if (pmap_hptsize > pdc_hwtlb.max_size)
			pmap_hptsize = pdc_hwtlb.max_size;
		else if (pmap_hptsize && pmap_hptsize < pdc_hwtlb.min_size)
			pmap_hptsize = pdc_hwtlb.min_size;
	} else {
		printf("WARNING: no HPT support, fine!\n");
		pmap_hptsize = 0;
	}

	if (cpu_type)
		for (p = cpu_types; p->name[0] && p->cpuid != cpu_type; p++);
	else
		for (p = cpu_types;
		    p->name[0] && p->features != cpu_features; p++);

	if (!p->name[0]) {
		printf("WARNING: UNKNOWN CPU TYPE; GOOD LUCK "
		    "(type 0x%x, features 0x%x)\n", cpu_type, cpu_features);
		p = cpu_types;
	} else if ((p->type == hpcxl || p->type == hpcxl2) && !fpu_enable) {
		/* we know PCXL and PCXL2 do not exist w/o FPU */
		fpu_enable = 0xc0;
		cpu_fpuena = 1;
	}

	/*
	 * TODO: HPT on 7200 is not currently supported
	 */
	if (pmap_hptsize && p->type != hpcxl && p->type != hpcxl2)
		pmap_hptsize = 0;

	cpu_type = p->type;
	cpu_typename = p->name;
	cpu_ibtlb_ins = p->ibtlbins;
	cpu_dbtlb_ins = p->dbtlbins;
	cpu_hpt_init = p->hptinit;
	cpu_desidhash = p->desidhash;

	/* patch tlb handler branches */
	if (p->patch) {
		trap_ep_T_TLB_DIRTY [0] = trap_ep_T_TLB_DIRTY [p->patch];
		trap_ep_T_DTLBMISS  [0] = trap_ep_T_DTLBMISS  [p->patch];
		trap_ep_T_DTLBMISSNA[0] = trap_ep_T_DTLBMISSNA[p->patch];
		trap_ep_T_ITLBMISS  [0] = trap_ep_T_ITLBMISS  [p->patch];
		trap_ep_T_ITLBMISSNA[0] = trap_ep_T_ITLBMISSNA[p->patch];
	}

	/* force strong ordering for now */
	if (p->features & HPPA_FTRS_W32B) {
		extern register_t kpsw;	/* intr.c */

		kpsw |= PSL_O;
	}

	{
		const char *p, *q;
		char buf[32];
		int lev;

		lev = 0xa + (*cpu_desidhash)();
		cpu_hvers = pdc_model.hvers >> 4;
		if (!cpu_hvers) {
			p = "(UNKNOWN)";
			q = lev == 0xa? "1.0" : "1.1";
		} else {
			p = hppa_mod_info(HPPA_TYPE_BOARD, cpu_hvers);
			if (!p) {
				snprintf(buf, sizeof buf, "(UNKNOWN 0x%x)",
				    cpu_hvers);
				p = buf;
			}

			switch (pdc_model.arch_rev) {
			default:
			case 0:
				q = "1.0";
#ifdef COMPAT_HPUX
				cpu_model_hpux = HPUX_SYSCONF_CPUPA10;
#endif
				break;
			case 4:
				q = "1.1";
#ifdef COMPAT_HPUX
				cpu_model_hpux = HPUX_SYSCONF_CPUPA11;
#endif
				/* this one is just a 100MHz pcxl */
				if (lev == 0x10)
					lev = 0xc;
				/* this one is a pcxl2 */
				if (lev == 0x16)
					lev = 0xe;
				break;
			case 8:
				q = "2.0";
#ifdef COMPAT_HPUX
				cpu_model_hpux = HPUX_SYSCONF_CPUPA20;
#endif
				break;
			}
		}

		snprintf(cpu_model, sizeof cpu_model,
		    "HP 9000/%s PA-RISC %s%x", p, q, lev);
	}
}

void
cpu_startup(void)
{
	vaddr_t minaddr, maxaddr;
	vsize_t size;
	int i, base, residual;

	/*
	 * i won't understand a friend of mine,
	 * who sat in a room full of artificial ice,
	 * fogging the air w/ humid cries --
	 *	WELCOME TO SUMMER!
	 */
	printf(version);

	printf("%s\n", cpu_model);
	printf("real mem = %d (%d reserved for PROM, %d used by OpenBSD)\n",
	    ctob(totalphysmem), ctob(resvmem), ctob(physmem - resvmem));

	size = MAXBSIZE * nbuf;
	if (uvm_map(kernel_map, &minaddr, round_page(size),
	    NULL, UVM_UNKNOWN_OFFSET, 0, UVM_MAPFLAG(UVM_PROT_NONE,
	    UVM_PROT_NONE, UVM_INH_NONE, UVM_ADV_NORMAL, 0)))
		panic("cpu_startup: cannot allocate VM for buffers");
	buffers = (caddr_t)minaddr;
	base = bufpages / nbuf;
	residual = bufpages % nbuf;
	for (i = 0; i < nbuf; i++) {
		vaddr_t curbuf;
		int cbpgs;

		/*
		 * First <residual> buffers get (base+1) physical pages
		 * allocated for them.  The rest get (base) physical pages.
		 *
		 * The rest of each buffer occupies virtual space,
		 * but has no physical memory allocated for it.
		 */
		curbuf = (vaddr_t) buffers + (i * MAXBSIZE);

		for (cbpgs = base + (i < residual? 1 : 0); cbpgs--; ) {
			struct vm_page *pg;

			if ((pg = uvm_pagealloc(NULL, 0, NULL, 0)) == NULL)
				panic("cpu_startup: not enough memory for "
				    "buffer cache");
			pmap_kenter_pa(curbuf, VM_PAGE_TO_PHYS(pg),
			    UVM_PROT_RW);
			curbuf += PAGE_SIZE;
		}
	}

	/*
	 * Allocate a submap for exec arguments.  This map effectively
	 * limits the number of processes exec'ing at any time.
	 */
	exec_map = uvm_km_suballoc(kernel_map, &minaddr, &maxaddr,
	    16*NCARGS, VM_MAP_PAGEABLE, FALSE, NULL);

	/*
	 * Allocate a submap for physio
	 */
	phys_map = uvm_km_suballoc(kernel_map, &minaddr, &maxaddr,
	    VM_PHYS_SIZE, 0, FALSE, NULL);

	printf("avail mem = %ld\n", ptoa(uvmexp.free));
	printf("using %d buffers containing %d bytes of memory\n",
	    nbuf, bufpages * PAGE_SIZE);

	/*
	 * Set up buffers, so they can be used to read disk labels.
	 */
	bufinit();
	vmmap = uvm_km_valloc_wait(kernel_map, NBPG);

	/*
	 * Configure the system.
	 */
	if (boothowto & RB_CONFIG) {
#ifdef BOOT_CONFIG
		user_config();
#else
		printf("kernel does not support -c; continuing..\n");
#endif
	}
}

/*
 * compute cpu clock ratio such as:
 *	cpu_ticksnum / cpu_ticksdenom = t + delta
 *	delta -> 0
 */
void
delay_init(void)
{
	register u_int num, denom, delta, mdelta;

	mdelta = UINT_MAX;
	for (denom = 1; denom < 1000; denom++) {
		num = (PAGE0->mem_10msec * denom) / 10000;
		delta = num * 10000 / denom - PAGE0->mem_10msec;
		if (!delta) {
			cpu_ticksdenom = denom;
			cpu_ticksnum = num;
			break;
		} else if (delta < mdelta) {
			cpu_ticksdenom = denom;
			cpu_ticksnum = num;
			mdelta = delta;
		}
	}
}

void
delay(us)
	u_int us;
{
	register u_int start, end, n;

	mfctl(CR_ITMR, start);
	while (us) {
		n = min(1000, us);
		end = start + n * cpu_ticksnum / cpu_ticksdenom;

		/* N.B. Interval Timer may wrap around */
		if (end < start)
			do
				mfctl(CR_ITMR, start);
			while (start > end);

		do
			mfctl(CR_ITMR, start);
		while (start < end);

		us -= n;
	}
}

void
microtime(struct timeval *tv)
{
	extern u_long cpu_itmr;
	u_long itmr, mask;
	int s;

	s = splhigh();
	tv->tv_sec  = time.tv_sec;
	tv->tv_usec = time.tv_usec;

	rsm(PSL_I, mask);
	mfctl(CR_ITMR, itmr);
	itmr -= cpu_itmr;
	ssm(PSL_I, mask);
	splx(s);

	tv->tv_usec += itmr * cpu_ticksdenom / cpu_ticksnum;
	if (tv->tv_usec >= 1000000) {
		tv->tv_usec -= 1000000;
		tv->tv_sec++;
	}
}


static __inline void
fall(c_base, c_count, c_loop, c_stride, data)
	int c_base, c_count, c_loop, c_stride, data;
{
	register int loop;

	for (; c_count--; c_base += c_stride)
		for (loop = c_loop; loop--; )
			if (data)
				fdce(0, c_base);
			else
				fice(0, c_base);
}

void
ficacheall(void)
{
	/*
	 * Flush the instruction, then data cache.
	 */
	fall(pdc_cache.ic_base, pdc_cache.ic_count, pdc_cache.ic_loop,
	    pdc_cache.ic_stride, 0);
	sync_caches();
}

void
fdcacheall(void)
{
	fall(pdc_cache.dc_base, pdc_cache.dc_count, pdc_cache.dc_loop,
	    pdc_cache.dc_stride, 1);
	sync_caches();
}

void
ptlball(void)
{
	register pa_space_t sp;
	register int i, j, k;

	/* instruction TLB */
	sp = pdc_cache.it_sp_base;
	for (i = 0; i < pdc_cache.it_sp_count; i++) {
		register vaddr_t off = pdc_cache.it_off_base;
		for (j = 0; j < pdc_cache.it_off_count; j++) {
			for (k = 0; k < pdc_cache.it_loop; k++)
				pitlbe(sp, off);
			off += pdc_cache.it_off_stride;
		}
		sp += pdc_cache.it_sp_stride;
	}

	/* data TLB */
	sp = pdc_cache.dt_sp_base;
	for (i = 0; i < pdc_cache.dt_sp_count; i++) {
		register vaddr_t off = pdc_cache.dt_off_base;
		for (j = 0; j < pdc_cache.dt_off_count; j++) {
			for (k = 0; k < pdc_cache.dt_loop; k++)
				pdtlbe(sp, off);
			off += pdc_cache.dt_off_stride;
		}
		sp += pdc_cache.dt_sp_stride;
	}
}

int
desidhash_g(void)
{
	/* TODO call PDC to disable SID hashing in the cache index */

	return 0;
}

int
hpti_g(hpt, hptsize)
	vaddr_t hpt;
	vsize_t hptsize;
{
	return pdc_call((iodcio_t)pdc, 0, PDC_TLB, PDC_TLB_CONFIG,
	    &pdc_hwtlb, hpt, hptsize, PDC_TLB_CURRPDE);
}

int
pbtlb_g(i)
	int i;
{
	return -1;
}

int
ibtlb_g(i, sp, va, pa, sz, prot)
	int i;
	pa_space_t sp;
	vaddr_t va;
	paddr_t pa;
	vsize_t sz;
	u_int prot;
{
	int error;

	if ((error = pdc_call((iodcio_t)pdc, 0, PDC_BLOCK_TLB, PDC_BTLB_INSERT,
	    sp, va, pa, sz, prot, i)) < 0) {
#ifdef BTLBDEBUG
		printf("WARNING: BTLB insert failed (%d)\n", error);
#endif
	}
	return error;
}

int
btlb_insert(space, va, pa, lenp, prot)
	pa_space_t space;
	vaddr_t va;
	paddr_t pa;
	vsize_t *lenp;
	u_int prot;
{
	static u_int32_t mask;
	register vsize_t len;
	register int error, i;

	if (!pdc_btlb.min_size && !pdc_btlb.max_size)
		return -(ENXIO);

	/* align size */
	for (len = pdc_btlb.min_size << PGSHIFT; len < *lenp; len <<= 1);
	len >>= PGSHIFT;
	i = ffs(~mask) - 1;
	if (len > pdc_btlb.max_size || i < 0) {
#ifdef BTLBDEBUG
		printf("btln_insert: too big (%u < %u < %u)\n",
		    pdc_btlb.min_size, len, pdc_btlb.max_size);
#endif
		return -(ENOMEM);
	}

	mask |= 1 << i;
	pa >>= PGSHIFT;
	va >>= PGSHIFT;
	/* check address alignment */
	if (pa & (len - 1)) {
#ifdef BTLBDEBUG
		printf("WARNING: BTLB address misaligned pa=0x%x, len=0x%x\n",
		    pa, len);
#endif
		return -(ERANGE);
	}

	/* ensure IO space is uncached */
	if ((pa & (HPPA_IOBEGIN >> PGSHIFT)) == (HPPA_IOBEGIN >> PGSHIFT))
		prot |= TLB_UNCACHABLE;

#ifdef BTLBDEBUG
	printf("btlb_insert(%d): %x:%x=%x[%x,%x]\n", i, space, va, pa, len, prot);
#endif
	if ((error = (*cpu_dbtlb_ins)(i, space, va, pa, len, prot)) < 0)
		return -(EINVAL);
	*lenp = len << PGSHIFT;

	return i;
}

int waittime = -1;

void
boot(howto)
	int howto;
{
	/* If system is cold, just halt. */
	if (cold) {
		/* (Unless the user explicitly asked for reboot.) */
		if ((howto & RB_USERREQ) == 0)
			howto |= RB_HALT;
	} else {

		boothowto = howto | (boothowto & RB_HALT);

		if (!(howto & RB_NOSYNC)) {
			waittime = 0;
			vfs_shutdown();
			/*
			 * If we've been adjusting the clock, the todr
			 * will be out of synch; adjust it now unless
			 * the system was sitting in ddb.
			 */
			if ((howto & RB_TIMEBAD) == 0)
				resettodr();
			else
				printf("WARNING: not updating battery clock\n");
		}

		/* XXX probably save howto into stable storage */

		splhigh();

		if (howto & RB_DUMP)
			dumpsys();

		doshutdownhooks();
	}

	/* in case we came on powerfail interrupt */
	if (cold_hook)
		(*cold_hook)(HPPA_COLD_COLD);

	if (howto & RB_HALT) {
		if (howto & RB_POWERDOWN && cold_hook) {
			printf("Powering off...");
			DELAY(1000000);
			(*cold_hook)(HPPA_COLD_OFF);
			DELAY(1000000);
		}

		printf("System halted!\n");
		DELAY(1000000);
		__asm __volatile("stwas %0, 0(%1)"
		    :: "r" (CMD_STOP), "r" (LBCAST_ADDR + iomod_command));
	} else {
		printf("rebooting...");
		DELAY(1000000);
		__asm __volatile(".export hppa_reset, entry\n\t"
		    ".label hppa_reset");
		__asm __volatile("stwas %0, 0(%1)"
		    :: "r" (CMD_RESET), "r" (LBCAST_ADDR + iomod_command));
	}

	for(;;); /* loop while bus reset is comming up */
	/* NOTREACHED */
}

u_long	dumpmag = 0x8fca0101;	/* magic number */
int	dumpsize = 0;		/* pages */
long	dumplo = 0;		/* blocks */

/*
 * cpu_dumpsize: calculate size of machine-dependent kernel core dump headers.
 */
int
cpu_dumpsize(void)
{
	int size;

	size = ALIGN(sizeof(kcore_seg_t)) + ALIGN(sizeof(cpu_kcore_hdr_t));
	if (roundup(size, dbtob(1)) != dbtob(1))
		return -1;

	return 1;
}

/*
 * Called from HPMC handler in locore
 */
void
hpmc_dump(void)
{
	printf("HPMC\n");

	cold = 0;
	boot(RB_NOSYNC);
}

int
cpu_dump(void)
{
	long buf[dbtob(1) / sizeof (long)];
	kcore_seg_t	*segp;
	cpu_kcore_hdr_t	*cpuhdrp;

	segp = (kcore_seg_t *)buf;
	cpuhdrp = (cpu_kcore_hdr_t *)&buf[ALIGN(sizeof(*segp)) / sizeof (long)];

	/*
	 * Generate a segment header.
	 */
	CORE_SETMAGIC(*segp, KCORE_MAGIC, MID_MACHINE, CORE_CPU);
	segp->c_size = dbtob(1) - ALIGN(sizeof(*segp));

	/*
	 * Add the machine-dependent header info
	 */
	/* nothing for now */

	return (bdevsw[major(dumpdev)].d_dump)
	    (dumpdev, dumplo, (caddr_t)buf, dbtob(1));
}

/*
 * Dump the kernel's image to the swap partition.
 */
#define	BYTES_PER_DUMP	NBPG

void
dumpsys(void)
{
	int psize, bytes, i, n;
	register caddr_t maddr;
	register daddr_t blkno;
	register int (*dump)(dev_t, daddr_t, caddr_t, size_t);
	register int error;

	/* Save registers
	savectx(&dumppcb); */

	if (dumpsize == 0)
		dumpconf();
	if (dumplo <= 0) {
		printf("\ndump to dev %x not possible\n", dumpdev);
		return;
	}
	printf("\ndumping to dev %x, offset %ld\n", dumpdev, dumplo);

	psize = (*bdevsw[major(dumpdev)].d_psize)(dumpdev);
	printf("dump ");
	if (psize == -1) {
		printf("area unavailable\n");
		return;
	}

	if (!(error = cpu_dump())) {

		bytes = ctob(totalphysmem);
		maddr = NULL;
		blkno = dumplo + cpu_dumpsize();
		dump = bdevsw[major(dumpdev)].d_dump;
		/* TODO block map the whole physical memory */
		for (i = 0; i < bytes; i += n) {

			/* Print out how many MBs we are to go. */
			n = bytes - i;
			if (n && (n % (1024*1024)) == 0)
				printf("%d ", n / (1024 * 1024));

			/* Limit size for next transfer. */

			if (n > BYTES_PER_DUMP)
				n = BYTES_PER_DUMP;

			if ((error = (*dump)(dumpdev, blkno, maddr, n)))
				break;
			maddr += n;
			blkno += btodb(n);
		}
	}

	switch (error) {
	case ENXIO:	printf("device bad\n");			break;
	case EFAULT:	printf("device not ready\n");		break;
	case EINVAL:	printf("area improper\n");		break;
	case EIO:	printf("i/o error\n");			break;
	case EINTR:	printf("aborted from console\n");	break;
	case 0:		printf("succeeded\n");			break;
	default:	printf("error %d\n", error);		break;
	}
}

/* bcopy(), error on fault */
int
kcopy(from, to, size)
	const void *from;
	void *to;
	size_t size;
{
	return spcopy(HPPA_SID_KERNEL, from, HPPA_SID_KERNEL, to, size);
}

int
copystr(src, dst, size, lenp)
	const void *src;
	void *dst;
	size_t size;
	size_t *lenp;
{
	return spstrcpy(HPPA_SID_KERNEL, src, HPPA_SID_KERNEL, dst, size, lenp);
}

int
copyinstr(src, dst, size, lenp)
	const void *src;
	void *dst;
	size_t size;
	size_t *lenp;
{
	return spstrcpy(curproc->p_addr->u_pcb.pcb_space, src,
	    HPPA_SID_KERNEL, dst, size, lenp);
}


int
copyoutstr(src, dst, size, lenp)
	const void *src;
	void *dst;
	size_t size;
	size_t *lenp;
{
	return spstrcpy(HPPA_SID_KERNEL, src,
	    curproc->p_addr->u_pcb.pcb_space, dst, size, lenp);
}


int
copyin(src, dst, size)
	const void *src;
	void *dst;
	size_t size;
{
	return spcopy(curproc->p_addr->u_pcb.pcb_space, src,
	    HPPA_SID_KERNEL, dst, size);
}

int
copyout(src, dst, size)
	const void *src;
	void *dst;
	size_t size;
{
	return spcopy(HPPA_SID_KERNEL, src,
	    curproc->p_addr->u_pcb.pcb_space, dst, size);
}

/*
 * Set registers on exec.
 */
void
setregs(p, pack, stack, retval)
	struct proc *p;
	struct exec_package *pack;
	u_long stack;
	register_t *retval;
{
	extern paddr_t fpu_curpcb;	/* from locore.S */
	struct trapframe *tf = p->p_md.md_regs;
	struct pcb *pcb = &p->p_addr->u_pcb;
	register_t zero;

	tf->tf_flags = TFF_SYS|TFF_LAST;
	tf->tf_iioq_tail = 4 +
	    (tf->tf_iioq_head = pack->ep_entry | HPPA_PC_PRIV_USER);
	tf->tf_rp = 0;
	tf->tf_arg0 = (u_long)PS_STRINGS;
	tf->tf_arg1 = tf->tf_arg2 = 0; /* XXX dynload stuff */

	/* setup terminal stack frame */
	stack = (stack + 0x1f) & ~0x1f;
	tf->tf_r3 = stack;
	tf->tf_sp = stack += HPPA_FRAME_SIZE;
	zero = 0;
	copyout(&zero, (caddr_t)(stack - HPPA_FRAME_SIZE), sizeof(register_t));
	copyout(&zero, (caddr_t)(stack + HPPA_FRAME_CRP), sizeof(register_t));

	/* reset any of the pending FPU exceptions */
	if (tf->tf_cr30 == fpu_curpcb) {
		fpu_exit();
		fpu_curpcb = 0;
	}
	pcb->pcb_fpregs[0] = ((u_int64_t)HPPA_FPU_INIT) << 32;
	pcb->pcb_fpregs[1] = 0;
	pcb->pcb_fpregs[2] = 0;
	pcb->pcb_fpregs[3] = 0;
	fdcache(HPPA_SID_KERNEL, (vaddr_t)pcb->pcb_fpregs, 8 * 4);

	retval[1] = 0;
}

/*
 * Send an interrupt to process.
 */
void
sendsig(catcher, sig, mask, code, type, val)
	sig_t catcher;
	int sig, mask;
	u_long code;
	int type;
	union sigval val;
{
	extern paddr_t fpu_curpcb;	/* from locore.S */
	extern u_int fpu_enable;
	struct proc *p = curproc;
	struct trapframe *tf = p->p_md.md_regs;
	struct sigacts *psp = p->p_sigacts;
	struct sigcontext ksc;
	siginfo_t ksi;
	register_t scp, sip;
	int sss;

#ifdef DEBUG
	if ((sigdebug & SDB_FOLLOW) && (!sigpid || p->p_pid == sigpid))
		printf("sendsig: %s[%d] sig %d catcher %p\n",
		    p->p_comm, p->p_pid, sig, catcher);
#endif

	/* flush the FPU ctx first */
	if (tf->tf_cr30 == fpu_curpcb) {
		mtctl(fpu_enable, CR_CCR);
		fpu_save(fpu_curpcb);
		/* fpu_curpcb = 0; only needed if fpregs are preset */
		mtctl(0, CR_CCR);
	}

	ksc.sc_onstack = psp->ps_sigstk.ss_flags & SS_ONSTACK;

	/*
	 * Allocate space for the signal handler context.
	 */
	if ((psp->ps_flags & SAS_ALTSTACK) && !ksc.sc_onstack &&
	    (psp->ps_sigonstack & sigmask(sig))) {
		scp = (register_t)psp->ps_sigstk.ss_sp;
		psp->ps_sigstk.ss_flags |= SS_ONSTACK;
	} else
		scp = (tf->tf_sp + 63) & ~63;

	sss = (sizeof(ksc) + 63) & ~63;
	sip = 0;
	if (psp->ps_siginfo & sigmask(sig)) {
		sip = scp + sizeof(ksc);
		sss += (sizeof(ksi) + 63) & ~63;
	}

#ifdef DEBUG
	if ((tf->tf_iioq_head & ~PAGE_MASK) == SYSCALLGATE)
		printf("sendsig: interrupted syscall at 0x%x:0x%x, flags %b\n",
		    tf->tf_iioq_head, tf->tf_iioq_tail, tf->tf_ipsw, PSL_BITS);
#endif

	ksc.sc_mask = mask;
	ksc.sc_fp = scp + sss;
	ksc.sc_ps = tf->tf_ipsw;
	ksc.sc_pcoqh = tf->tf_iioq_head;
	ksc.sc_pcoqt = tf->tf_iioq_tail;
	ksc.sc_regs[0] = tf->tf_t1;
	ksc.sc_regs[1] = tf->tf_t2;
	ksc.sc_regs[2] = tf->tf_sp;
	ksc.sc_regs[3] = tf->tf_t3;
	ksc.sc_regs[4] = tf->tf_sar;
	ksc.sc_regs[5] = tf->tf_r1;
	ksc.sc_regs[6] = tf->tf_rp;
	ksc.sc_regs[7] = tf->tf_r3;
	ksc.sc_regs[8] = tf->tf_r4;
	ksc.sc_regs[9] = tf->tf_r5;
	ksc.sc_regs[10] = tf->tf_r6;
	ksc.sc_regs[11] = tf->tf_r7;
	ksc.sc_regs[12] = tf->tf_r8;
	ksc.sc_regs[13] = tf->tf_r9;
	ksc.sc_regs[14] = tf->tf_r10;
	ksc.sc_regs[15] = tf->tf_r11;
	ksc.sc_regs[16] = tf->tf_r12;
	ksc.sc_regs[17] = tf->tf_r13;
	ksc.sc_regs[18] = tf->tf_r14;
	ksc.sc_regs[19] = tf->tf_r15;
	ksc.sc_regs[20] = tf->tf_r16;
	ksc.sc_regs[21] = tf->tf_r17;
	ksc.sc_regs[22] = tf->tf_r18;
	ksc.sc_regs[23] = tf->tf_t4;
	ksc.sc_regs[24] = tf->tf_arg3;
	ksc.sc_regs[25] = tf->tf_arg2;
	ksc.sc_regs[26] = tf->tf_arg1;
	ksc.sc_regs[27] = tf->tf_arg0;
	ksc.sc_regs[28] = tf->tf_dp;
	ksc.sc_regs[29] = tf->tf_ret0;
	ksc.sc_regs[30] = tf->tf_ret1;
	ksc.sc_regs[31] = tf->tf_r31;
	bcopy(p->p_addr->u_pcb.pcb_fpregs, ksc.sc_fpregs,
	    sizeof(ksc.sc_fpregs));

	sss += HPPA_FRAME_SIZE;
	tf->tf_arg0 = sig;
	tf->tf_arg1 = sip;
	tf->tf_arg2 = tf->tf_r4 = scp;
	tf->tf_arg3 = (register_t)catcher;
	tf->tf_sp = scp + sss;
	tf->tf_ipsw &= ~(PSL_N|PSL_B);
	tf->tf_iioq_head = HPPA_PC_PRIV_USER | p->p_sigcode;
	tf->tf_iioq_tail = tf->tf_iioq_head + 4;
	/* disable tracing in the trapframe */

#ifdef DEBUG
	if ((sigdebug & SDB_FOLLOW) && (!sigpid || p->p_pid == sigpid))
		printf("sendsig(%d): sig %d scp %p fp %p sp 0x%x\n",
		    p->p_pid, sig, scp, ksc.sc_fp, (register_t)scp + sss);
#endif

	if (copyout(&ksc, (void *)scp, sizeof(ksc)))
		sigexit(p, SIGILL);

	if (sip) {
		initsiginfo(&ksi, sig, code, type, val);
		if (copyout(&ksi, (void *)sip, sizeof(ksi)))
			sigexit(p, SIGILL);
	}

	zero = 0;
	if (copyout(&zero, (caddr_t)scp + sss - HPPA_FRAME_SIZE,
	    sizeof(register_t)))
		sigexit(p, SIGILL);

#ifdef DEBUG
	if ((sigdebug & SDB_FOLLOW) && (!sigpid || p->p_pid == sigpid))
		printf("sendsig(%d): pc 0x%x, catcher 0x%x\n", p->p_pid,
		    tf->tf_iioq_head, tf->tf_arg3);
#endif
}

int
sys_sigreturn(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	extern paddr_t fpu_curpcb;	/* from locore.S */
	struct sys_sigreturn_args /* {
		syscallarg(struct sigcontext *) sigcntxp;
	} */ *uap = v;
	struct sigcontext *scp, ksc;
	struct trapframe *tf = p->p_md.md_regs;
	int error;

	scp = SCARG(uap, sigcntxp);
#ifdef DEBUG
	if ((sigdebug & SDB_FOLLOW) && (!sigpid || p->p_pid == sigpid))
		printf("sigreturn: pid %d, scp %p\n", p->p_pid, scp);
#endif

	/* flush the FPU ctx first */
	if (tf->tf_cr30 == fpu_curpcb) {
		fpu_exit();
		fpu_curpcb = 0;
	}

	if ((error = copyin((caddr_t)scp, (caddr_t)&ksc, sizeof ksc)))
		return (error);

#define PSL_MBS (PSL_C|PSL_Q|PSL_P|PSL_D|PSL_I)
#define PSL_MBZ (PSL_Y|PSL_Z|PSL_S|PSL_X|PSL_M|PSL_R)
	if ((ksc.sc_ps & (PSL_MBS|PSL_MBZ)) != PSL_MBS)
		return (EINVAL);

	if (ksc.sc_onstack)
		p->p_sigacts->ps_sigstk.ss_flags |= SS_ONSTACK;
	else
		p->p_sigacts->ps_sigstk.ss_flags &= ~SS_ONSTACK;
	p->p_sigmask = ksc.sc_mask &~ sigcantmask;

	tf->tf_t1 = ksc.sc_regs[0];		/* r22 */
	tf->tf_t2 = ksc.sc_regs[1];		/* r21 */
	tf->tf_sp = ksc.sc_regs[2];
	tf->tf_t3 = ksc.sc_regs[3];		/* r20 */
	tf->tf_sar = ksc.sc_regs[4];
	tf->tf_r1 = ksc.sc_regs[5];
	tf->tf_rp = ksc.sc_regs[6];
	tf->tf_r3 = ksc.sc_regs[7];
	tf->tf_r4 = ksc.sc_regs[8];
	tf->tf_r5 = ksc.sc_regs[9];
	tf->tf_r6 = ksc.sc_regs[10];
	tf->tf_r7 = ksc.sc_regs[11];
	tf->tf_r8 = ksc.sc_regs[12];
	tf->tf_r9 = ksc.sc_regs[13];
	tf->tf_r10 = ksc.sc_regs[14];
	tf->tf_r11 = ksc.sc_regs[15];
	tf->tf_r12 = ksc.sc_regs[16];
	tf->tf_r13 = ksc.sc_regs[17];
	tf->tf_r14 = ksc.sc_regs[18];
	tf->tf_r15 = ksc.sc_regs[19];
	tf->tf_r16 = ksc.sc_regs[20];
	tf->tf_r17 = ksc.sc_regs[21];
	tf->tf_r18 = ksc.sc_regs[22];
	tf->tf_t4 = ksc.sc_regs[23];		/* r19 */
	tf->tf_arg3 = ksc.sc_regs[24];	/* r23 */
	tf->tf_arg2 = ksc.sc_regs[25];	/* r24 */
	tf->tf_arg1 = ksc.sc_regs[26];	/* r25 */
	tf->tf_arg0 = ksc.sc_regs[27];	/* r26 */
	tf->tf_dp = ksc.sc_regs[28];
	tf->tf_ret0 = ksc.sc_regs[29];
	tf->tf_ret1 = ksc.sc_regs[30];
	tf->tf_r31 = ksc.sc_regs[31];
	bcopy(ksc.sc_fpregs, p->p_addr->u_pcb.pcb_fpregs,
	    sizeof(ksc.sc_fpregs));
	fdcache(HPPA_SID_KERNEL, (vaddr_t)p->p_addr->u_pcb.pcb_fpregs,
	    sizeof(ksc.sc_fpregs));

	tf->tf_iioq_head = ksc.sc_pcoqh;
	tf->tf_iioq_tail = ksc.sc_pcoqt;
	tf->tf_ipsw = ksc.sc_ps;

#ifdef DEBUG
	if ((sigdebug & SDB_FOLLOW) && (!sigpid || p->p_pid == sigpid))
		printf("sigreturn(%d): returns\n", p->p_pid);
#endif
	return (EJUSTRETURN);
}

/*
 * machine dependent system variables.
 */
int
cpu_sysctl(name, namelen, oldp, oldlenp, newp, newlen, p)
	int *name;
	u_int namelen;
	void *oldp;
	size_t *oldlenp;
	void *newp;
	size_t newlen;
	struct proc *p;
{
	extern paddr_t fpu_curpcb;	/* from locore.S */
	extern u_int fpu_enable;
	extern int cpu_fpuena;
	dev_t consdev;

	/* all sysctl names at this level are terminal */
	if (namelen != 1)
		return (ENOTDIR);	/* overloaded */
	switch (name[0]) {
	case CPU_CONSDEV:
		if (cn_tab != NULL)
			consdev = cn_tab->cn_dev;
		else
			consdev = NODEV;
		return (sysctl_rdstruct(oldp, oldlenp, newp, &consdev,
		    sizeof consdev));
	case CPU_FPU:
		if (fpu_curpcb) {
			mtctl(fpu_enable, CR_CCR);
			fpu_save(fpu_curpcb);
			fpu_curpcb = 0;
			mtctl(0, CR_CCR);
		}
		return (sysctl_int(oldp, oldlenp, newp, newlen, &cpu_fpuena));
	default:
		return (EOPNOTSUPP);
	}
	/* NOTREACHED */
}


/*
 * consinit:
 * initialize the system console.
 */
void
consinit(void)
{
	static int initted;

	if (!initted) {
		initted++;
		cninit();
	}
}
