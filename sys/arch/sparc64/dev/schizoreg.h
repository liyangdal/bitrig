/*	$OpenBSD: schizoreg.h,v 1.2 2002/06/09 00:01:05 jason Exp $	*/

/*
 * Copyright (c) 2002 Jason L. Wright (jason@thought.net)
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
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Jason L. Wright
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#define	SCZ_PCIA_MEM_MATCH		0x00040
#define	SCZ_PCIA_MEM_MASK		0x00048
#define	SCZ_PCIA_IO_MATCH		0x00050
#define	SCZ_PCIA_IO_MASK		0x00058
#define	SCZ_PCIB_MEM_MATCH		0x00060
#define	SCZ_PCIB_MEM_MASK		0x00068
#define	SCZ_PCIB_IO_MATCH		0x00070
#define	SCZ_PCIB_IO_MASK		0x00078
#define	SCZ_SAFARI_ERRLOG		0x10018
#define	SCZ_ECCCTRL			0x10020
#define	SCZ_UE_AFSR			0x10030
#define	SCZ_UE_AFAR			0x10038
#define	SCZ_CE_AFSR			0x10040
#define	SCZ_CE_AFAR			0x10048

#define	SCZ_PCI_AFSR			0x2010
#define	SCZ_PCI_AFAR			0x2018

#define	SCZ_ECCCTRL_EE			0x8000000000000000UL
#define	SCZ_ECCCTRL_UE			0x4000000000000000UL
#define	SCZ_ECCCTRL_CE			0x2000000000000000UL

#define	SCZ_UEAFSR_PPIO			0x8000000000000000UL
#define	SCZ_UEAFSR_PDRD			0x4000000000000000UL
#define	SCZ_UEAFSR_PDWR			0x2000000000000000UL
#define	SCZ_UEAFSR_SPIO			0x1000000000000000UL
#define	SCZ_UEAFSR_SDMA			0x0800000000000000UL
#define	SCZ_UEAFSR_ERRPNDG		0x0300000000000000UL
#define	SCZ_UEAFSR_BMSK			0x000003ff00000000UL
#define	SCZ_UEAFSR_QOFF			0x00000000c0000000UL
#define	SCZ_UEAFSR_AID			0x000000001f000000UL
#define	SCZ_UEAFSR_PARTIAL		0x0000000000800000UL
#define	SCZ_UEAFSR_OWNEDIN		0x0000000000400000UL
#define	SCZ_UEAFSR_MTAGSYND		0x00000000000f0000UL
#define	SCZ_UEAFSR_MTAG			0x000000000000e000UL
#define	SCZ_UEAFSR_ECCSYND		0x00000000000001ffUL

#define	SCZ_CEAFSR_PPIO			0x8000000000000000UL
#define	SCZ_CEAFSR_PDRD			0x4000000000000000UL
#define	SCZ_CEAFSR_PDWR			0x2000000000000000UL
#define	SCZ_CEAFSR_SPIO			0x1000000000000000UL
#define	SCZ_CEAFSR_SDMA			0x0800000000000000UL
#define	SCZ_CEAFSR_ERRPNDG		0x0300000000000000UL
#define	SCZ_CEAFSR_BMSK			0x000003ff00000000UL
#define	SCZ_CEAFSR_QOFF			0x00000000c0000000UL
#define	SCZ_CEAFSR_AID			0x000000001f000000UL
#define	SCZ_CEAFSR_PARTIAL		0x0000000000800000UL
#define	SCZ_CEAFSR_OWNEDIN		0x0000000000400000UL
#define	SCZ_CEAFSR_MTAGSYND		0x00000000000f0000UL
#define	SCZ_CEAFSR_MTAG			0x000000000000e000UL
#define	SCZ_CEAFSR_ECCSYND		0x00000000000001ffUL

struct schizo_range {
	u_int32_t	cspace;
	u_int32_t	child_hi;
	u_int32_t	child_lo;
	u_int32_t	phys_hi;
	u_int32_t	phys_lo;
	u_int32_t	size_hi;
	u_int32_t	size_lo;
};
