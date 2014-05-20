/*	$OpenBSD: mount.h,v 1.119 2014/05/20 13:32:22 syl Exp $	*/
/*	$NetBSD: mount.h,v 1.48 1996/02/18 11:55:47 fvdl Exp $	*/

/*
 * Copyright (c) 1989, 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)mount.h	8.15 (Berkeley) 7/14/94
 */

#ifndef _SYS_MOUNT_H_
#define _SYS_MOUNT_H_

#include <sys/cdefs.h>
#ifndef _KERNEL
#include <sys/ucred.h>
#endif
#include <sys/queue.h>
#include <sys/rwlock.h>

typedef struct { int32_t val[2]; } fsid_t;	/* file system id type */

/*
 * File identifier.
 * These are unique per filesystem on a single machine.
 */
#define	MAXFIDSZ	16

struct fid {
	u_short		fid_len;		/* length of data in bytes */
	u_short		fid_reserved;		/* force longword alignment */
	char		fid_data[MAXFIDSZ];	/* data (variable length) */
};

/*
 * Export arguments for local filesystem mount calls.
 */
struct export_args {
	int	ex_flags;		/* export related flags */
	uid_t	ex_root;		/* mapping for root uid */
	struct	xucred ex_anon;		/* mapping for anonymous user */
	struct	sockaddr *ex_addr;	/* net address to which exported */
	int	ex_addrlen;		/* and the net address length */
	struct	sockaddr *ex_mask;	/* mask of valid bits in saddr */
	int	ex_masklen;		/* and the smask length */
};

/*
 * Arguments to mount UFS-based filesystems
 */
struct ufs_args {
	char	*fspec;			/* block special device to mount */
	struct	export_args export_info;/* network export information */
	int	flags;			/* ffs specific mount flags */
};

#define	UFSMNT_FFS2		0x01	/* ffs2 in use (set by the kernel) */

/*
 * Arguments to mount MFS
 */
struct mfs_args {
	char	*fspec;			/* name to export for statfs */
	struct	export_args export_info;/* if exported MFSes are supported */
	caddr_t	base;			/* base of file system in memory */
	u_long	size;			/* size of file system */
};

/*
 * Arguments to mount tmpfs file systems
 */
#define TMPFS_ARGS_VERSION	1
struct tmpfs_args {
	int			ta_version;

	/* Size counters. */
	ino_t			ta_nodes_max;
	off_t			ta_size_max;

	/* Root node attributes. */
	uid_t			ta_root_uid;
	gid_t			ta_root_gid;
	mode_t			ta_root_mode;
};

/*
 * Arguments to mount ISO 9660 filesystems.
 */
struct iso_args {
	char	*fspec;			/* block special device to mount */
	struct	export_args export_info;/* network export info */
	int	flags;			/* mounting flags, see below */
	int	sess;			/* start sector of session */
};

#define	ISOFSMNT_NORRIP		0x00000001	/* disable Rock Ridge Ext.*/
#define	ISOFSMNT_GENS		0x00000002	/* enable generation numbers */
#define	ISOFSMNT_EXTATT		0x00000004	/* enable extended attr. */
#define	ISOFSMNT_NOJOLIET	0x00000008	/* disable Joliet Ext.*/
#define	ISOFSMNT_SESS		0x00000010	/* use iso_args.sess */

/*
 * Arguments to mount NFS
 */
#define NFS_ARGSVERSION	4		/* change when nfs_args changes */
struct nfs_args {
	int		version;	/* args structure version number */
	struct sockaddr	*addr;		/* file server address */
	int		addrlen;	/* length of address */
	int		sotype;		/* Socket type */
	int		proto;		/* and Protocol */
	u_char		*fh;		/* File handle to be mounted */
	int		fhsize;		/* Size, in bytes, of fh */
	int		flags;		/* flags */
	int		wsize;		/* write size in bytes */
	int		rsize;		/* read size in bytes */
	int		readdirsize;	/* readdir size in bytes */
	int		timeo;		/* initial timeout in .1 secs */
	int		retrans;	/* times to retry send */
	int		maxgrouplist;	/* Max. size of group list */
	int		readahead;	/* # of blocks to readahead */
	int		leaseterm;	/* Term (sec) of lease */
	int		deadthresh;	/* Retrans threshold */
	char		*hostname;	/* server's name */
	int		acregmin;	/* Attr cache file recently modified */
	int		acregmax;	/* ac file not recently modified */
	int		acdirmin;	/* ac for dir recently modified */
	int		acdirmax;	/* ac for dir not recently modified */
};

/*
 * NFS mount option flags
 */
#define	NFSMNT_RESVPORT		0x00000000  /* always use reserved ports */
#define	NFSMNT_SOFT		0x00000001  /* soft mount (hard is default) */
#define	NFSMNT_WSIZE		0x00000002  /* set write size */
#define	NFSMNT_RSIZE		0x00000004  /* set read size */
#define	NFSMNT_TIMEO		0x00000008  /* set initial timeout */
#define	NFSMNT_RETRANS		0x00000010  /* set number of request retries */
#define	NFSMNT_MAXGRPS		0x00000020  /* set maximum grouplist size */
#define	NFSMNT_INT		0x00000040  /* allow interrupts on hard mount */
#define	NFSMNT_NOCONN		0x00000080  /* Don't Connect the socket */
#define	NFSMNT_NQNFS		0x00000100  /* Use Nqnfs protocol */
#define	NFSMNT_NFSV3		0x00000200  /* Use NFS Version 3 protocol */
#define	NFSMNT_KERB		0x00000400  /* Use Kerberos authentication */
#define	NFSMNT_DUMBTIMR		0x00000800  /* Don't estimate rtt dynamically */
#define	NFSMNT_LEASETERM	0x00001000  /* set lease term (nqnfs) */
#define	NFSMNT_READAHEAD	0x00002000  /* set read ahead */
#define	NFSMNT_DEADTHRESH	0x00004000  /* set dead server retry thresh */
#define	NFSMNT_NOAC		0x00008000  /* disable attribute cache */
#define	NFSMNT_RDIRPLUS		0x00010000  /* Use Readdirplus for V3 */
#define	NFSMNT_READDIRSIZE	0x00020000  /* Set readdir size */

/* Flags valid only in mount syscall arguments */
#define NFSMNT_ACREGMIN		0x00040000  /* acregmin field valid */
#define NFSMNT_ACREGMAX		0x00080000  /* acregmax field valid */
#define NFSMNT_ACDIRMIN		0x00100000  /* acdirmin field valid */
#define NFSMNT_ACDIRMAX		0x00200000  /* acdirmax field valid */

/* Flags valid only in kernel */
#define	NFSMNT_INTERNAL		0xfffc0000  /* Bits set internally */
#define NFSMNT_HASWRITEVERF	0x00040000  /* Has write verifier for V3 */
#define NFSMNT_GOTPATHCONF	0x00080000  /* Got the V3 pathconf info */
#define NFSMNT_GOTFSINFO	0x00100000  /* Got the V3 fsinfo */
#define	NFSMNT_MNTD		0x00200000  /* Mnt server for mnt point */
#define	NFSMNT_DISMINPROG	0x00400000  /* Dismount in progress */
#define	NFSMNT_DISMNT		0x00800000  /* Dismounted */
#define	NFSMNT_SNDLOCK		0x01000000  /* Send socket lock */
#define	NFSMNT_WANTSND		0x02000000  /* Want above */
#define	NFSMNT_RCVLOCK		0x04000000  /* Rcv socket lock */
#define	NFSMNT_WANTRCV		0x08000000  /* Want above */
#define	NFSMNT_WAITAUTH		0x10000000  /* Wait for authentication */
#define	NFSMNT_HASAUTH		0x20000000  /* Has authenticator */
#define	NFSMNT_WANTAUTH		0x40000000  /* Wants an authenticator */
#define	NFSMNT_AUTHERR		0x80000000  /* Authentication error */

/*
 *  Arguments to mount MSDOS filesystems.
 */
struct msdosfs_args {
	char	*fspec;		/* blocks special holding the fs to mount */
	struct	export_args export_info;
				/* network export information */
	uid_t	uid;		/* uid that owns msdosfs files */
	gid_t	gid;		/* gid that owns msdosfs files */
	mode_t  mask;		/* mask to be applied for msdosfs perms */
	int	flags;		/* see below */
};

/*
 * Msdosfs mount options:
 */
#define	MSDOSFSMNT_SHORTNAME	0x01	/* Force old DOS short names only */
#define	MSDOSFSMNT_LONGNAME	0x02	/* Force Win'95 long names */
#define	MSDOSFSMNT_NOWIN95	0x04	/* Completely ignore Win95 entries */
#define MSDOSFSMNT_ALLOWDIRX	0x10	/* dir is mode +x if r */

/* Arguments to mount UDF file systems */
struct udf_args {
	char *fspec; /* Block special device to mount */
	u_int32_t lastblock; /* Special device last block */
};

/*
 * Arguments to mount fusefs filesystems
 */
struct fusefs_args {
	char *name;
	int fd;
	int max_read;
};

/*
 * file system statistics
 */

/* per-filesystem mount options */
union mount_info {
	struct ufs_args ufs_args;
	struct mfs_args mfs_args;
	struct nfs_args nfs_args;
	struct iso_args iso_args;
	struct msdosfs_args msdosfs_args;
	struct tmpfs_args tmpfs_args;
	char __align[160];	/* 64-bit alignment and room to grow */
};

#define	MFSNAMELEN	16	/* length of fs type name, including nul */
#define	MNAMELEN	90	/* length of buffer for returned name */

/* new statfs structure with mount options and statvfs fields */
struct statfs {
	u_int32_t	f_flags;	/* copy of mount flags */
	u_int32_t	f_bsize;	/* file system block size */
	u_int32_t	f_iosize;	/* optimal transfer block size */

					/* unit is f_bsize */
	u_int64_t	f_blocks;	/* total data blocks in file system */
	u_int64_t	f_bfree;	/* free blocks in fs */
	int64_t		f_bavail;	/* free blocks avail to non-superuser */ 

	u_int64_t	f_files;	/* total file nodes in file system */
	u_int64_t	f_ffree;	/* free file nodes in fs */
	int64_t		f_favail;	/* free file nodes avail to non-root */

	u_int64_t	f_syncwrites;	/* count of sync writes since mount */
	u_int64_t	f_syncreads;	/* count of sync reads since mount */
	u_int64_t	f_asyncwrites;	/* count of async writes since mount */
	u_int64_t	f_asyncreads;	/* count of async reads since mount */

	fsid_t		f_fsid;		/* file system id */
	u_int32_t	f_namemax;      /* maximum filename length */
	uid_t	   	f_owner;	/* user that mounted the file system */
	u_int64_t  	f_ctime;	/* last mount [-u] time */

	char f_fstypename[MFSNAMELEN];	/* fs type name */
	char f_mntonname[MNAMELEN];	/* directory on which mounted */
	char f_mntfromname[MNAMELEN];	/* mounted file system */
	char f_mntfromspec[MNAMELEN];	/* special for mount request */
	union mount_info mount_info;	/* per-filesystem mount options */
};

/*
 * File system types.
 */
#define	MOUNT_FFS	"ffs"		/* UNIX "Fast" Filesystem */
#define	MOUNT_NFS	"nfs"		/* Network Filesystem */
#define	MOUNT_MFS	"mfs"		/* Memory Filesystem */
#define	MOUNT_MSDOS	"msdos"		/* MSDOS Filesystem */
#define	MOUNT_CD9660	"cd9660"	/* ISO9660 (aka CDROM) Filesystem */
#define	MOUNT_EXT2FS	"ext2fs"	/* Second Extended Filesystem */
#define	MOUNT_UDF	"udf"		/* UDF */
#define	MOUNT_TMPFS	"tmpfs"		/* tmpfs */
#define	MOUNT_FUSEFS	"fuse"		/* FUSE */

/*
 * Structure per mounted file system.  Each mounted file system has an
 * array of operations and an instance record.  The file systems are
 * put on a doubly linked list.
 */
LIST_HEAD(vnodelst, vnode);

struct mount {
	TAILQ_ENTRY(mount) mnt_list;		/* mount list */
	const struct vfsops *mnt_op;		/* operations on fs */
	struct vfsconf  *mnt_vfc;               /* configuration info */
	struct vnode	*mnt_vnodecovered;	/* vnode we mounted on */
	struct vnode    *mnt_syncer;            /* syncer vnode */
	struct vnodelst	mnt_vnodelist;		/* list of vnodes this mount */
	struct rwlock   mnt_lock;               /* mount structure lock */
	int		mnt_flag;		/* flags */
	int		mnt_maxsymlinklen;	/* max size of short symlink */
	struct statfs	mnt_stat;		/* cache of filesystem stats */
	void		*mnt_data;		/* private data */
	struct wapbl	*mnt_wapbl;		/* log info */
	struct wapbl_replay
			*mnt_wapbl_replay;	/* replay support */
	struct wapbl_ops
			*mnt_wapbl_op;		/* logging ops */
};

/*
 * Mount flags.
 *
 * Unmount uses MNT_FORCE flag.
 */
#define	MNT_RDONLY	0x00000001	/* read only filesystem */
#define	MNT_SYNCHRONOUS	0x00000002	/* file system written synchronously */
#define	MNT_NOEXEC	0x00000004	/* can't exec from filesystem */
#define	MNT_NOSUID	0x00000008	/* don't honor setuid bits on fs */
#define	MNT_NODEV	0x00000010	/* don't interpret special files */
#define	MNT_ASYNC	0x00000040	/* file system written asynchronously */

/*
 * exported mount flags.
 */
#define	MNT_EXRDONLY	0x00000080	/* exported read only */
#define	MNT_EXPORTED	0x00000100	/* file system is exported */
#define	MNT_DEFEXPORTED	0x00000200	/* exported to the world */
#define	MNT_EXPORTANON	0x00000400	/* use anon uid mapping for everyone */
#define	MNT_EXKERB	0x00000800	/* exported with Kerberos uid mapping */

/*
 * Flags set by internal operations.
 */
#define	MNT_LOCAL	0x00001000	/* filesystem is stored locally */
#define	MNT_QUOTA	0x00002000	/* quotas are enabled on filesystem */
#define	MNT_ROOTFS	0x00004000	/* identifies the root filesystem */

/*
 * Extra post 4.4BSD-lite2 mount flags.
 */
#define MNT_NOATIME	0x00008000	/* don't update access times on fs */

/*
 * Mask of flags that are visible to statfs()
 */
#define	MNT_VISFLAGMASK	0x1400ffff

#define	MNT_BITS \
    "\010\001RDONLY\002SYNCHRONOUS\003NOEXEC\004NOSUID\005NODEV" \
    "\007ASYNC\010EXRDONLY\011EXPORTED\012DEFEXPORTED\013EXPORTANON" \
    "\014EXKERB\015LOCAL\016QUOTA\017ROOTFS"

/*
 * Filesystem control flags.
 */
#define	MNT_UPDATE	0x00010000	/* not a real mount, just an update */
#define	MNT_DELEXPORT	0x00020000	/* delete export host lists */
#define	MNT_RELOAD	0x00040000	/* reload filesystem data */
#define	MNT_FORCE	0x00080000	/* force unmount or readonly change */
#define MNT_WANTRDWR	0x02000000	/* want upgrade to read/write */
#define MNT_SOFTDEP     0x04000000      /* soft dependencies being done */
#define MNT_DOOMED	0x08000000	/* device behind filesystem is gone */
#define MNT_LOG		0x10000000	/* use logging */

/*
 * Flags for various system call interfaces.
 *
 * waitfor flags to vfs_sync() and getfsstat()
 */
#define MNT_WAIT	1	/* synchronously wait for I/O to complete */
#define MNT_NOWAIT	2	/* start all I/O, but do not wait for it */
#define MNT_LAZY	3	/* push data not written by filesystem syncer */

/*
 * Generic file handle
 */
struct fhandle {
	fsid_t	fh_fsid;	/* File system id of mount point */
	struct	fid fh_fid;	/* File sys specific id */
};
typedef struct fhandle	fhandle_t;

/*
 * Sysctl CTL_VFS definitions.
 *
 * Second level identifier specifies which filesystem. Second level
 * identifier VFS_GENERIC returns information about all filesystems.
 */
#define	VFS_GENERIC	0	/* generic filesystem information */
/*
 * Third level identifiers for VFS_GENERIC are given below; third
 * level identifiers for specific filesystems are given in their
 * mount specific header files.
 */
#define VFS_MAXTYPENUM	1	/* int: highest defined filesystem type */
#define VFS_CONF	2	/* struct: vfsconf for filesystem given
				   as next argument */
#define VFS_BCACHESTAT	3	/* struct: buffer cache statistics given
				   as next argument */
#define	CTL_VFSGENCTL_NAMES { \
	{ 0, 0 }, \
	{ "maxtypenum", CTLTYPE_INT }, \
	{ "conf", CTLTYPE_NODE }, \
	{ "bcachestat", CTLTYPE_STRUCT } \
}

/*
 * Filesystem configuration information. One of these exists for each
 * type of filesystem supported by the kernel. These are searched at
 * mount time to identify the requested filesystem.
 */
struct vfsconf {
	const struct vfsops *vfc_vfsops; /* filesystem operations vector */
	char	vfc_name[MFSNAMELEN];	/* filesystem type name */
	int	vfc_typenum;		/* historic filesystem type number */
	int	vfc_refcount;		/* number mounted of this type */
	int	vfc_flags;		/* permanent flags */
	struct	vfsconf *vfc_next;	/* next in list */
};

/* buffer cache statistics */
struct bcachestats {
	int64_t numbufs;		/* number of buffers allocated */
	int64_t numbufpages;		/* number of pages in buffer cache */
	int64_t numdirtypages;		/* number of dirty free pages */
	int64_t numcleanpages;		/* number of clean free pages */
	int64_t pendingwrites;		/* number of pending writes */
	int64_t pendingreads;		/* number of pending reads */
	int64_t numwrites;		/* total writes started */
	int64_t numreads;		/* total reads started */
	int64_t cachehits;		/* total reads found in cache */
	int64_t busymapped;		/* number of busy and mapped buffers */
	int64_t dmapages;		/* dma reachable pages in buffer cache */
	int64_t highpages;		/* pages above dma region */
	int64_t delwribufs;		/* delayed write buffers */
	int64_t kvaslots;		/* kva slots total */
	int64_t kvaslots_avail;		/* available kva slots */
};
#ifdef _KERNEL
extern struct bcachestats bcstats;
extern long buflowpages, bufhighpages, bufbackpages;
#define BUFPAGES_DEFICIT (((buflowpages - bcstats.numbufpages) < 0) ? 0 \
    : buflowpages - bcstats.numbufpages)
#define BUFPAGES_INACT (((bcstats.numcleanpages - buflowpages) < 0) ? 0 \
    : bcstats.numcleanpages - buflowpages)
extern int bufcachepercent;
extern void bufadjust(int);
struct uvm_constraint_range;
extern int bufbackoff(struct uvm_constraint_range*, long);

struct wapbl;
struct wapbl_replay;

/*
 * This operations vector is so wapbl can be wrapped into a filesystem lkm.
 * XXX Eventually, we want to move this functionality
 * down into the filesystems themselves so that this isn't needed.
 */
struct wapbl_ops {
	void (*wo_wapbl_discard)(struct wapbl *);
	int (*wo_wapbl_replay_isopen)(struct wapbl_replay *);
	int (*wo_wapbl_replay_can_read)(struct wapbl_replay *, daddr_t, long);
	int (*wo_wapbl_replay_read)(struct wapbl_replay *, void *, daddr_t, long);
	void (*wo_wapbl_add_buf)(struct wapbl *, struct buf *);
	void (*wo_wapbl_remove_buf)(struct wapbl *, struct buf *);
	void (*wo_wapbl_resize_buf)(struct wapbl *, struct buf *, long, long);
	int (*wo_wapbl_begin)(struct wapbl *, const char *, int);
	void (*wo_wapbl_end)(struct wapbl *);
	void (*wo_wapbl_junlock_assert)(struct wapbl *);
	void (*wo_wapbl_biodone)(struct buf *);
};
#define WAPBL_DISCARD(MP)						\
    (*(MP)->mnt_wapbl_op->wo_wapbl_discard)((MP)->mnt_wapbl)
#define WAPBL_REPLAY_ISOPEN(MP)						\
    (*(MP)->mnt_wapbl_op->wo_wapbl_replay_isopen)((MP)->mnt_wapbl_replay)
#define WAPBL_REPLAY_CAN_READ(MP, BLK, LEN)				\
    (*(MP)->mnt_wapbl_op->wo_wapbl_replay_can_read)((MP)->mnt_wapbl_replay, \
    (BLK), (LEN))
#define WAPBL_REPLAY_READ(MP, DATA, BLK, LEN)				\
    (*(MP)->mnt_wapbl_op->wo_wapbl_replay_read)((MP)->mnt_wapbl_replay,	\
    (DATA), (BLK), (LEN))
#define WAPBL_ADD_BUF(MP, BP)						\
    (*(MP)->mnt_wapbl_op->wo_wapbl_add_buf)((MP)->mnt_wapbl, (BP))
#define WAPBL_REMOVE_BUF(MP, BP)					\
    (*(MP)->mnt_wapbl_op->wo_wapbl_remove_buf)((MP)->mnt_wapbl, (BP))
#define WAPBL_RESIZE_BUF(MP, BP, OLDSZ, OLDCNT)				\
    (*(MP)->mnt_wapbl_op->wo_wapbl_resize_buf)((MP)->mnt_wapbl, (BP),	\
    (OLDSZ), (OLDCNT))
#define WAPBL_BEGIN(MP)							\
    (*(MP)->mnt_wapbl_op->wo_wapbl_begin)((MP)->mnt_wapbl,		\
    __FILE__, __LINE__)
#define WAPBL_END(MP)							\
    (*(MP)->mnt_wapbl_op->wo_wapbl_end)((MP)->mnt_wapbl)
#define WAPBL_JUNLOCK_ASSERT(MP)					\
    (*(MP)->mnt_wapbl_op->wo_wapbl_junlock_assert)((MP)->mnt_wapbl)

/*
 * Operations supported on mounted file system.
 */
struct nameidata;
struct mbuf;

extern int maxvfsconf;		/* highest defined filesystem type */
extern struct vfsconf *vfsconf;	/* head of list of filesystem types */

struct vfsops {
	int	(*vfs_mount)(struct mount *mp, const char *path,
				    void *data,
				    struct nameidata *ndp, struct proc *p);
	int	(*vfs_start)(struct mount *mp, int flags,
				    struct proc *p);
	int	(*vfs_unmount)(struct mount *mp, int mntflags,
				    struct proc *p);
	int	(*vfs_root)(struct mount *mp, struct vnode **vpp);
	int	(*vfs_quotactl)(struct mount *mp, int cmds, uid_t uid,
				    caddr_t arg, struct proc *p);
	int	(*vfs_statfs)(struct mount *mp, struct statfs *sbp,
				    struct proc *p);
	int	(*vfs_sync)(struct mount *mp, int waitfor,
				    struct ucred *cred, struct proc *p);
	int	(*vfs_vget)(struct mount *mp, ino_t ino,
				    struct vnode **vpp);
	int	(*vfs_fhtovp)(struct mount *mp, struct fid *fhp,
				     struct vnode **vpp);
	int	(*vfs_vptofh)(struct vnode *vp, struct fid *fhp);
	int	(*vfs_init)(struct vfsconf *);
	int     (*vfs_sysctl)(int *, u_int, void *, size_t *, void *,
				     size_t, struct proc *);
	int	(*vfs_checkexp)(struct mount *mp, struct mbuf *nam,
				    int *extflagsp, struct ucred **credanonp);
};

#define VFS_MOUNT(MP, PATH, DATA, NDP, P) \
	(*(MP)->mnt_op->vfs_mount)(MP, PATH, DATA, NDP, P)
#define VFS_START(MP, FLAGS, P)	  (*(MP)->mnt_op->vfs_start)(MP, FLAGS, P)
#define VFS_UNMOUNT(MP, FORCE, P) (*(MP)->mnt_op->vfs_unmount)(MP, FORCE, P)
#define VFS_ROOT(MP, VPP)	  (*(MP)->mnt_op->vfs_root)(MP, VPP)
#define VFS_QUOTACTL(MP,C,U,A,P)  (*(MP)->mnt_op->vfs_quotactl)(MP, C, U, A, P)
#define VFS_STATFS(MP, SBP, P)	  (*(MP)->mnt_op->vfs_statfs)(MP, SBP, P)
#define VFS_SYNC(MP, WAIT, C, P)  (*(MP)->mnt_op->vfs_sync)(MP, WAIT, C, P)
#define VFS_VGET(MP, INO, VPP)	  (*(MP)->mnt_op->vfs_vget)(MP, INO, VPP)
#define VFS_FHTOVP(MP, FIDP, VPP) \
	(*(MP)->mnt_op->vfs_fhtovp)(MP, FIDP, VPP)
#define	VFS_VPTOFH(VP, FIDP)	  (*(VP)->v_mount->mnt_op->vfs_vptofh)(VP, FIDP)
#define VFS_CHECKEXP(MP, NAM, EXFLG, CRED) \
	(*(MP)->mnt_op->vfs_checkexp)(MP, NAM, EXFLG, CRED)


#include <net/radix.h>
#include <sys/socket.h>		/* XXX for AF_MAX */

/*
 * Network address lookup element
 */
struct netcred {
	struct	radix_node netc_rnodes[2];
	int	netc_exflags;
	struct	ucred netc_anon;
};

/*
 * Network export information
 */
struct netexport {
	struct	netcred ne_defexported;		      /* Default export */
	struct	radix_node_head *ne_rtable[AF_MAX+1]; /* Individual exports */
};

/*
 * exported vnode operations
 */
int	vfs_busy(struct mount *, int);
#define VB_READ		0x01
#define VB_WRITE	0x02
#define VB_NOWAIT	0x04	/* immediately fail on busy lock */
#define VB_WAIT		0x08	/* sleep fail on busy lock */

int     vfs_isbusy(struct mount *);
int     vfs_mount_foreach_vnode(struct mount *, int (*func)(struct vnode *,
				    void *), void *);
void	vfs_getnewfsid(struct mount *);
struct	mount *vfs_getvfs(fsid_t *);
int	vfs_mountedon(struct vnode *);
int	vfs_rootmountalloc(char *, char *, struct mount **);
void	vfs_unbusy(struct mount *);
void	vfs_unmountall(void);
extern	TAILQ_HEAD(mntlist, mount) mountlist;

struct	mount *getvfs(fsid_t *);	    /* return vfs given fsid */
					    /* process mount export info */
int	vfs_export(struct mount *, struct netexport *, struct export_args *);
					    /* lookup host in fs export list */
struct	netcred *vfs_export_lookup(struct mount *, struct netexport *,
	    struct mbuf *);
int	vfs_allocate_syncvnode(struct mount *);
int	speedup_syncer(void);

int	vfs_syncwait(int);	/* sync and wait for complete */
void	vfs_shutdown(void);	/* unmount and sync file systems */
int	dounmount(struct mount *, int, struct proc *, struct vnode *);
void	vfsinit(void);
int	vfs_register(struct vfsconf *);
int	vfs_unregister(struct vfsconf *);
#else /* _KERNEL */
__BEGIN_DECLS
int	fstatfs(int, struct statfs *);
int	getfh(const char *, fhandle_t *);
int	getfsstat(struct statfs *, size_t, int);
int	getmntinfo(struct statfs **, int);
int	mount(const char *, const char *, int, void *);
int	statfs(const char *, struct statfs *);
int	unmount(const char *, int);
#if __BSD_VISIBLE
struct stat;
int	fhopen(const fhandle_t *, int);
int	fhstat(const fhandle_t *, struct stat *);
int	fhstatfs(const fhandle_t *, struct statfs *);
#endif /* __BSD_VISIBLE */
__END_DECLS
#endif /* _KERNEL */
#endif /* !_SYS_MOUNT_H_ */
