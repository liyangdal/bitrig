/*	$OpenBSD: hpux_syscalls.c,v 1.6 2004/07/14 15:55:38 mickey Exp $	*/

/*
 * System call names.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	OpenBSD: syscalls.master,v 1.6 2004/07/14 15:54:56 mickey Exp 
 */

char *hpux_syscallnames[] = {
	"syscall",			/* 0 = syscall */
	"exit",			/* 1 = exit */
	"fork",			/* 2 = fork */
	"read",			/* 3 = read */
	"write",			/* 4 = write */
	"open",			/* 5 = open */
	"close",			/* 6 = close */
	"wait",			/* 7 = wait */
	"creat",			/* 8 = creat */
	"link",			/* 9 = link */
	"unlink",			/* 10 = unlink */
	"execv",			/* 11 = execv */
	"chdir",			/* 12 = chdir */
	"time_6x",			/* 13 = time_6x */
	"mknod",			/* 14 = mknod */
	"chmod",			/* 15 = chmod */
	"chown",			/* 16 = chown */
	"obreak",			/* 17 = obreak */
	"#18 (unimplemented lchmod)",		/* 18 = unimplemented lchmod */
	"lseek",			/* 19 = lseek */
	"getpid",			/* 20 = getpid */
	"#21 (unimplemented mount)",		/* 21 = unimplemented mount */
	"#22 (unimplemented umount)",		/* 22 = unimplemented umount */
	"setuid",			/* 23 = setuid */
	"getuid",			/* 24 = getuid */
	"stime_6x",			/* 25 = stime_6x */
#ifdef PTRACE
	"ptrace",			/* 26 = ptrace */
#else
	"#26 (unimplemented ptrace)",		/* 26 = unimplemented ptrace */
#endif
	"alarm_6x",			/* 27 = alarm_6x */
	"#28 (unimplemented cnx_lw_pmon_read)",		/* 28 = unimplemented cnx_lw_pmon_read */
	"pause_6x",			/* 29 = pause_6x */
	"utime_6x",			/* 30 = utime_6x */
	"stty_6x",			/* 31 = stty_6x */
	"gtty_6x",			/* 32 = gtty_6x */
	"access",			/* 33 = access */
	"nice_6x",			/* 34 = nice_6x */
	"ftime_6x",			/* 35 = ftime_6x */
	"sync",			/* 36 = sync */
	"kill",			/* 37 = kill */
	"stat",			/* 38 = stat */
	"setpgrp_6x",			/* 39 = setpgrp_6x */
	"lstat",			/* 40 = lstat */
	"dup",			/* 41 = dup */
	"opipe",			/* 42 = opipe */
	"times_6x",			/* 43 = times_6x */
	"profil",			/* 44 = profil */
	"#45 (unimplemented ki_syscall)",		/* 45 = unimplemented ki_syscall */
	"setgid",			/* 46 = setgid */
	"getgid",			/* 47 = getgid */
	"#48 (unimplemented)",		/* 48 = unimplemented */
	"#49 (unimplemented reserved for USG)",		/* 49 = unimplemented reserved for USG */
	"#50 (unimplemented reserved for USG)",		/* 50 = unimplemented reserved for USG */
	"#51 (unimplemented acct)",		/* 51 = unimplemented acct */
	"#52 (unimplemented set_userthreadid)",		/* 52 = unimplemented set_userthreadid */
	"#53 (unimplemented lwp_mutex_unlock_2)",		/* 53 = unimplemented lwp_mutex_unlock_2 */
	"ioctl",			/* 54 = ioctl */
	"#55 (unimplemented reboot)",		/* 55 = unimplemented reboot */
	"symlink",			/* 56 = symlink */
	"utssys",			/* 57 = utssys */
	"readlink",			/* 58 = readlink */
	"execve",			/* 59 = execve */
	"umask",			/* 60 = umask */
	"chroot",			/* 61 = chroot */
	"fcntl",			/* 62 = fcntl */
	"ulimit",			/* 63 = ulimit */
	"#64 (unimplemented)",		/* 64 = unimplemented */
	"#65 (unimplemented)",		/* 65 = unimplemented */
	"vfork",			/* 66 = vfork */
	"#67 (unimplemented lwp_getprivate)",		/* 67 = unimplemented lwp_getprivate */
	"#68 (unimplemented lwp_setprivate)",		/* 68 = unimplemented lwp_setprivate */
	"#69 (unimplemented)",		/* 69 = unimplemented */
	"#70 (unimplemented)",		/* 70 = unimplemented */
	"mmap",			/* 71 = mmap */
	"#72 (unimplemented)",		/* 72 = unimplemented */
	"munmap",			/* 73 = munmap */
	"mprotect",			/* 74 = mprotect */
	"madvise",			/* 75 = madvise */
	"#76 (unimplemented vhangup)",		/* 76 = unimplemented vhangup */
	"#77 (unimplemented swapoff)",		/* 77 = unimplemented swapoff */
	"#78 (unimplemented)",		/* 78 = unimplemented */
	"getgroups",			/* 79 = getgroups */
	"setgroups",			/* 80 = setgroups */
	"getpgrp2",			/* 81 = getpgrp2 */
	"setpgrp2",			/* 82 = setpgrp2 */
	"setitimer",			/* 83 = setitimer */
	"wait3",			/* 84 = wait3 */
	"#85 (unimplemented swapon)",		/* 85 = unimplemented swapon */
	"getitimer",			/* 86 = getitimer */
	"#87 (unimplemented)",		/* 87 = unimplemented */
	"#88 (unimplemented)",		/* 88 = unimplemented */
	"#89 (unimplemented)",		/* 89 = unimplemented */
	"dup2",			/* 90 = dup2 */
	"#91 (unimplemented)",		/* 91 = unimplemented */
	"fstat",			/* 92 = fstat */
	"select",			/* 93 = select */
	"#94 (unimplemented)",		/* 94 = unimplemented */
	"fsync",			/* 95 = fsync */
	"setpriority",			/* 96 = setpriority */
	"#97 (unimplemented)",		/* 97 = unimplemented */
	"#98 (unimplemented)",		/* 98 = unimplemented */
	"#99 (unimplemented)",		/* 99 = unimplemented */
	"getpriority",			/* 100 = getpriority */
	"#101 (unimplemented)",		/* 101 = unimplemented */
	"#102 (unimplemented)",		/* 102 = unimplemented */
	"#103 (unimplemented lf_send)",		/* 103 = unimplemented lf_send */
	"#104 (unimplemented)",		/* 104 = unimplemented */
	"#105 (unimplemented)",		/* 105 = unimplemented */
	"#106 (unimplemented)",		/* 106 = unimplemented */
	"#107 (unimplemented)",		/* 107 = unimplemented */
	"sigvec",			/* 108 = sigvec */
	"sigblock",			/* 109 = sigblock */
	"sigsetmask",			/* 110 = sigsetmask */
	"sigpause",			/* 111 = sigpause */
	"sigstack",			/* 112 = sigstack */
	"#113 (unimplemented)",		/* 113 = unimplemented */
	"#114 (unimplemented)",		/* 114 = unimplemented */
	"#115 (unimplemented regctl)",		/* 115 = unimplemented regctl */
	"gettimeofday",			/* 116 = gettimeofday */
	"#117 (unimplemented getrusage)",		/* 117 = unimplemented getrusage */
	"#118 (unimplemented)",		/* 118 = unimplemented */
	"#119 (unimplemented)",		/* 119 = unimplemented */
	"readv",			/* 120 = readv */
	"writev",			/* 121 = writev */
	"settimeofday",			/* 122 = settimeofday */
	"fchown",			/* 123 = fchown */
	"fchmod",			/* 124 = fchmod */
	"#125 (unimplemented)",		/* 125 = unimplemented */
	"setresuid",			/* 126 = setresuid */
	"setresgid",			/* 127 = setresgid */
	"rename",			/* 128 = rename */
	"truncate",			/* 129 = truncate */
	"ftruncate",			/* 130 = ftruncate */
	"#131 (unimplemented)",		/* 131 = unimplemented */
	"sysconf",			/* 132 = sysconf */
	"#133 (unimplemented)",		/* 133 = unimplemented */
	"#134 (unimplemented)",		/* 134 = unimplemented */
	"#135 (unimplemented)",		/* 135 = unimplemented */
	"mkdir",			/* 136 = mkdir */
	"rmdir",			/* 137 = rmdir */
	"#138 (unimplemented)",		/* 138 = unimplemented */
	"#139 (unimplemented sigcleanup)",		/* 139 = unimplemented sigcleanup */
	"#140 (unimplemented setcore)",		/* 140 = unimplemented setcore */
	"#141 (unimplemented)",		/* 141 = unimplemented */
	"#142 (unimplemented)",		/* 142 = unimplemented */
	"#143 (unimplemented)",		/* 143 = unimplemented */
	"getrlimit",			/* 144 = getrlimit */
	"setrlimit",			/* 145 = setrlimit */
	"#146 (unimplemented)",		/* 146 = unimplemented */
	"#147 (unimplemented lwp_self)",		/* 147 = unimplemented lwp_self */
	"#148 (unimplemented quotactl)",		/* 148 = unimplemented quotactl */
	"#149 (unimplemented get_sysinfo)",		/* 149 = unimplemented get_sysinfo */
	"#150 (unimplemented)",		/* 150 = unimplemented */
	"#151 (unimplemented privgrp)",		/* 151 = unimplemented privgrp */
	"rtprio",			/* 152 = rtprio */
	"#153 (unimplemented plock)",		/* 153 = unimplemented plock */
	"#154 (unimplemented lf_next_scn)",		/* 154 = unimplemented lf_next_scn */
	"lockf",			/* 155 = lockf */
#ifdef SYSVSEM
	"semget",			/* 156 = semget */
	"__semctl",			/* 157 = __semctl */
	"semop",			/* 158 = semop */
#else
	"#156 (unimplemented semget)",		/* 156 = unimplemented semget */
	"#157 (unimplemented semctl)",		/* 157 = unimplemented semctl */
	"#158 (unimplemented semop)",		/* 158 = unimplemented semop */
#endif
#ifdef SYSVMSG
	"msgget",			/* 159 = msgget */
	"msgctl",			/* 160 = msgctl */
	"msgsnd",			/* 161 = msgsnd */
	"msgrcv",			/* 162 = msgrcv */
#else
	"#159 (unimplemented msgget)",		/* 159 = unimplemented msgget */
	"#160 (unimplemented msgctl)",		/* 160 = unimplemented msgctl */
	"#161 (unimplemented msgsnd)",		/* 161 = unimplemented msgsnd */
	"#162 (unimplemented msgrcv)",		/* 162 = unimplemented msgrcv */
#endif
#ifdef SYSVSHM
	"shmget",			/* 163 = shmget */
	"shmctl",			/* 164 = shmctl */
	"shmat",			/* 165 = shmat */
	"shmdt",			/* 166 = shmdt */
#else
	"#163 (unimplemented shmget)",		/* 163 = unimplemented shmget */
	"#164 (unimplemented shmctl)",		/* 164 = unimplemented shmctl */
	"#165 (unimplemented shmat)",		/* 165 = unimplemented shmat */
	"#166 (unimplemented shmdt)",		/* 166 = unimplemented shmdt */
#endif
	"#167 (unimplemented set_mem_window)",		/* 167 = unimplemented set_mem_window */
	"#168 (unimplemented nsp_init)",		/* 168 = unimplemented nsp_init */
	"#169 (unimplemented)",		/* 169 = unimplemented */
	"#170 (unimplemented mkrnod)",		/* 170 = unimplemented mkrnod */
	"#171 (unimplemented test)",		/* 171 = unimplemented test */
	"#172 (unimplemented unsp_open)",		/* 172 = unimplemented unsp_open */
	"#173 (unimplemented)",		/* 173 = unimplemented */
	"getcontext",			/* 174 = getcontext */
	"#175 (unimplemented osetcontext)",		/* 175 = unimplemented osetcontext */
	"#176 (unimplemented bigio)",		/* 176 = unimplemented bigio */
	"#177 (unimplemented pipenode)",		/* 177 = unimplemented pipenode */
	"#178 (unimplemented lsync)",		/* 178 = unimplemented lsync */
	"#179 (unimplemented)",		/* 179 = unimplemented */
	"#180 (unimplemented mysite)",		/* 180 = unimplemented mysite */
	"#181 (unimplemented sitels)",		/* 181 = unimplemented sitels */
	"#182 (unimplemented swapclients)",		/* 182 = unimplemented swapclients */
	"#183 (unimplemented rmtprocess)",		/* 183 = unimplemented rmtprocess */
	"#184 (unimplemented dskless_stats)",		/* 184 = unimplemented dskless_stats */
	"sigprocmask",			/* 185 = sigprocmask */
	"sigpending",			/* 186 = sigpending */
	"sigsuspend",			/* 187 = sigsuspend */
	"sigaction",			/* 188 = sigaction */
	"#189 (unimplemented lw_get_thread_times)",		/* 189 = unimplemented lw_get_thread_times */
	"#190 (unimplemented nfssvc)",		/* 190 = unimplemented nfssvc */
	"#191 (unimplemented getfh)",		/* 191 = unimplemented getfh */
	"getdomainname",			/* 192 = getdomainname */
	"#193 (unimplemented setdomainname)",		/* 193 = unimplemented setdomainname */
	"#194 (unimplemented async_daemon)",		/* 194 = unimplemented async_daemon */
	"#195 (unimplemented getdirentries)",		/* 195 = unimplemented getdirentries */
	"#196 (unimplemented statfs)",		/* 196 = unimplemented statfs */
	"#197 (unimplemented fstatfs)",		/* 197 = unimplemented fstatfs */
	"#198 (unimplemented vfsmount)",		/* 198 = unimplemented vfsmount */
	"#199 (unimplemented qmml)",		/* 199 = unimplemented qmml */
	"waitpid",			/* 200 = waitpid */
	"#201 (unimplemented)",		/* 201 = unimplemented */
	"#202 (unimplemented)",		/* 202 = unimplemented */
	"#203 (unimplemented)",		/* 203 = unimplemented */
	"#204 (unimplemented)",		/* 204 = unimplemented */
	"#205 (unimplemented)",		/* 205 = unimplemented */
	"#206 (unimplemented)",		/* 206 = unimplemented */
	"#207 (unimplemented)",		/* 207 = unimplemented */
	"#208 (unimplemented)",		/* 208 = unimplemented */
	"#209 (unimplemented)",		/* 209 = unimplemented */
	"#210 (unimplemented)",		/* 210 = unimplemented */
	"#211 (unimplemented)",		/* 211 = unimplemented */
	"#212 (unimplemented)",		/* 212 = unimplemented */
	"#213 (unimplemented)",		/* 213 = unimplemented */
	"#214 (unimplemented)",		/* 214 = unimplemented */
	"#215 (unimplemented)",		/* 215 = unimplemented */
	"#216 (unimplemented)",		/* 216 = unimplemented */
	"#217 (unimplemented)",		/* 217 = unimplemented */
	"#218 (unimplemented)",		/* 218 = unimplemented */
	"#219 (unimplemented)",		/* 219 = unimplemented */
	"#220 (unimplemented)",		/* 220 = unimplemented */
	"#221 (unimplemented)",		/* 221 = unimplemented */
	"#222 (unimplemented)",		/* 222 = unimplemented */
	"#223 (unimplemented)",		/* 223 = unimplemented */
	"hpux_sigsetreturn",			/* 224 = hpux_sigsetreturn */
	"#225 (unimplemented sigsetstatemask)",		/* 225 = unimplemented sigsetstatemask */
	"#226 (unimplemented)",		/* 226 = unimplemented */
	"#227 (unimplemented cs)",		/* 227 = unimplemented cs */
	"#228 (unimplemented cds)",		/* 228 = unimplemented cds */
	"#229 (unimplemented set_no_trunc)",		/* 229 = unimplemented set_no_trunc */
	"#230 (unimplemented pathconf)",		/* 230 = unimplemented pathconf */
	"#231 (unimplemented fpathconf)",		/* 231 = unimplemented fpathconf */
	"#232 (unimplemented)",		/* 232 = unimplemented */
	"#233 (unimplemented)",		/* 233 = unimplemented */
	"#234 (unimplemented nfs_fcntl)",		/* 234 = unimplemented nfs_fcntl */
	"#235 (unimplemented ogetacl)",		/* 235 = unimplemented ogetacl */
	"#236 (unimplemented ofgetctl)",		/* 236 = unimplemented ofgetctl */
	"#237 (unimplemented osetacl)",		/* 237 = unimplemented osetacl */
	"#238 (unimplemented ofsetacl)",		/* 238 = unimplemented ofsetacl */
	"#239 (unimplemented pstat)",		/* 239 = unimplemented pstat */
	"#240 (unimplemented getaudid)",		/* 240 = unimplemented getaudid */
	"#241 (unimplemented setaudid)",		/* 241 = unimplemented setaudid */
	"#242 (unimplemented getaudproc)",		/* 242 = unimplemented getaudproc */
	"#243 (unimplemented setaudproc)",		/* 243 = unimplemented setaudproc */
	"#244 (unimplemented getevent)",		/* 244 = unimplemented getevent */
	"#245 (unimplemented setevent)",		/* 245 = unimplemented setevent */
	"#246 (unimplemented audwrite)",		/* 246 = unimplemented audwrite */
	"#247 (unimplemented audswitch)",		/* 247 = unimplemented audswitch */
	"#248 (unimplemented audctl)",		/* 248 = unimplemented audctl */
	"#249 (unimplemented ogetaccess)",		/* 249 = unimplemented ogetaccess */
	"#250 (unimplemented fsctl)",		/* 250 = unimplemented fsctl */
	"#251 (unimplemented)",		/* 251 = unimplemented */
	"#252 (unimplemented getmount_entries)",		/* 252 = unimplemented getmount_entries */
	"#253 (unimplemented lwp_mutex_init2)",		/* 253 = unimplemented lwp_mutex_init2 */
	"#254 (unimplemented)",		/* 254 = unimplemented */
	"#255 (unimplemented)",		/* 255 = unimplemented */
	"#256 (unimplemented)",		/* 256 = unimplemented */
	"#257 (unimplemented)",		/* 257 = unimplemented */
	"#258 (unimplemented)",		/* 258 = unimplemented */
	"#259 (unimplemented swapfs)",		/* 259 = unimplemented swapfs */
	"#260 (unimplemented fss)",		/* 260 = unimplemented fss */
	"#261 (unimplemented)",		/* 261 = unimplemented */
	"#262 (unimplemented)",		/* 262 = unimplemented */
	"#263 (unimplemented)",		/* 263 = unimplemented */
	"#264 (unimplemented)",		/* 264 = unimplemented */
	"#265 (unimplemented)",		/* 265 = unimplemented */
	"#266 (unimplemented)",		/* 266 = unimplemented */
	"#267 (unimplemented tsync)",		/* 267 = unimplemented tsync */
	"#268 (unimplemented getnumfds)",		/* 268 = unimplemented getnumfds */
	"poll",			/* 269 = poll */
	"#270 (unimplemented getmsg)",		/* 270 = unimplemented getmsg */
	"#271 (unimplemented putmsg)",		/* 271 = unimplemented putmsg */
	"fchdir",			/* 272 = fchdir */
	"#273 (unimplemented getmount_cnt)",		/* 273 = unimplemented getmount_cnt */
	"#274 (unimplemented getmount_entry)",		/* 274 = unimplemented getmount_entry */
	"accept",			/* 275 = accept */
	"bind",			/* 276 = bind */
	"connect",			/* 277 = connect */
	"getpeername",			/* 278 = getpeername */
	"getsockname",			/* 279 = getsockname */
	"getsockopt",			/* 280 = getsockopt */
	"listen",			/* 281 = listen */
	"recv",			/* 282 = recv */
	"recvfrom",			/* 283 = recvfrom */
	"recvmsg",			/* 284 = recvmsg */
	"send",			/* 285 = send */
	"sendmsg",			/* 286 = sendmsg */
	"sendto",			/* 287 = sendto */
	"setsockopt",			/* 288 = setsockopt */
	"shutdown",			/* 289 = shutdown */
	"socket",			/* 290 = socket */
	"socketpair",			/* 291 = socketpair */
	"#292 (unimplemented proc_open)",		/* 292 = unimplemented proc_open */
	"#293 (unimplemented proc_close)",		/* 293 = unimplemented proc_close */
	"#294 (unimplemented proc_send)",		/* 294 = unimplemented proc_send */
	"#295 (unimplemented proc_recv)",		/* 295 = unimplemented proc_recv */
	"#296 (unimplemented proc_sendrecv)",		/* 296 = unimplemented proc_sendrecv */
	"#297 (unimplemented proc_syscall)",		/* 297 = unimplemented proc_syscall */
	"#298 (unimplemented)",		/* 298 = unimplemented */
	"#299 (unimplemented)",		/* 299 = unimplemented */
	"#300 (unimplemented)",		/* 300 = unimplemented */
	"#301 (unimplemented)",		/* 301 = unimplemented */
	"#302 (unimplemented)",		/* 302 = unimplemented */
	"#303 (unimplemented)",		/* 303 = unimplemented */
	"#304 (unimplemented)",		/* 304 = unimplemented */
	"#305 (unimplemented)",		/* 305 = unimplemented */
	"#306 (unimplemented)",		/* 306 = unimplemented */
	"#307 (unimplemented)",		/* 307 = unimplemented */
	"#308 (unimplemented)",		/* 308 = unimplemented */
	"#309 (unimplemented)",		/* 309 = unimplemented */
	"#310 (unimplemented)",		/* 310 = unimplemented */
	"#311 (unimplemented)",		/* 311 = unimplemented */
#ifdef SYSVSEM
	"nsemctl",			/* 312 = nsemctl */
#else
	"#312 (unimplemented semctl)",		/* 312 = unimplemented semctl */
#endif
#ifdef SYSVMSG
	"nmsgctl",			/* 313 = nmsgctl */
#else
	"#313 (unimplemented msgctl)",		/* 313 = unimplemented msgctl */
#endif
#ifdef SYSVSHM
	"nshmctl",			/* 314 = nshmctl */
#else
	"#314 (unimplemented shmctl)",		/* 314 = unimplemented shmctl */
#endif
	"#315 (unimplemented mpctl)",		/* 315 = unimplemented mpctl */
	"#316 (unimplemented exportfs)",		/* 316 = unimplemented exportfs */
	"#317 (unimplemented getpmsg)",		/* 317 = unimplemented getpmsg */
	"#318 (unimplemented putpmsg)",		/* 318 = unimplemented putpmsg */
	"#319 (unimplemented)",		/* 319 = unimplemented */
	"msync",			/* 320 = msync */
	"#321 (unimplemented msleep)",		/* 321 = unimplemented msleep */
	"#322 (unimplemented mwakeup)",		/* 322 = unimplemented mwakeup */
	"#323 (unimplemented msem_init)",		/* 323 = unimplemented msem_init */
	"#324 (unimplemented msem_remove)",		/* 324 = unimplemented msem_remove */
	"#325 (unimplemented adjtime)",		/* 325 = unimplemented adjtime */
	"#326 (unimplemented kload)",		/* 326 = unimplemented kload */
	"#327 (unimplemented fattach)",		/* 327 = unimplemented fattach */
	"#328 (unimplemented fdetach)",		/* 328 = unimplemented fdetach */
	"#329 (unimplemented serialize)",		/* 329 = unimplemented serialize */
	"#330 (unimplemented statvfs)",		/* 330 = unimplemented statvfs */
	"#331 (unimplemented fstatvfs)",		/* 331 = unimplemented fstatvfs */
	"lchown",			/* 332 = lchown */
	"#333 (unimplemented getsid)",		/* 333 = unimplemented getsid */
	"#334 (unimplemented sysfs)",		/* 334 = unimplemented sysfs */
	"#335 (unimplemented)",		/* 335 = unimplemented */
	"#336 (unimplemented)",		/* 336 = unimplemented */
	"#337 (unimplemented sched_setparam)",		/* 337 = unimplemented sched_setparam */
	"#338 (unimplemented sched_getparam)",		/* 338 = unimplemented sched_getparam */
	"#339 (unimplemented sched_setscheduler)",		/* 339 = unimplemented sched_setscheduler */
	"#340 (unimplemented sched_getscheduler)",		/* 340 = unimplemented sched_getscheduler */
	"#341 (unimplemented sched_yield)",		/* 341 = unimplemented sched_yield */
	"#342 (unimplemented sched_get_priority_max)",		/* 342 = unimplemented sched_get_priority_max */
	"#343 (unimplemented sched_get_priority_min)",		/* 343 = unimplemented sched_get_priority_min */
	"#344 (unimplemented sched_rr_get_interval)",		/* 344 = unimplemented sched_rr_get_interval */
	"#345 (unimplemented clock_settime)",		/* 345 = unimplemented clock_settime */
	"#346 (unimplemented clock_gettime)",		/* 346 = unimplemented clock_gettime */
	"#347 (unimplemented clock_getres)",		/* 347 = unimplemented clock_getres */
	"#348 (unimplemented timer_create)",		/* 348 = unimplemented timer_create */
	"#349 (unimplemented timer_delete)",		/* 349 = unimplemented timer_delete */
	"#350 (unimplemented timer_settime)",		/* 350 = unimplemented timer_settime */
	"#351 (unimplemented timer_gettime)",		/* 351 = unimplemented timer_gettime */
	"#352 (unimplemented timer_getoverrun)",		/* 352 = unimplemented timer_getoverrun */
	"nanosleep",			/* 353 = nanosleep */
	"#354 (unimplemented toolbox)",		/* 354 = unimplemented toolbox */
	"#355 (unimplemented)",		/* 355 = unimplemented */
	"#356 (unimplemented getdents)",		/* 356 = unimplemented getdents */
	"#357 (unimplemented getcontext)",		/* 357 = unimplemented getcontext */
	"#358 (unimplemented sysinfo)",		/* 358 = unimplemented sysinfo */
	"#359 (unimplemented fcntl64)",		/* 359 = unimplemented fcntl64 */
	"#360 (unimplemented ftruncate64)",		/* 360 = unimplemented ftruncate64 */
	"#361 (unimplemented fstat64)",		/* 361 = unimplemented fstat64 */
	"#362 (unimplemented getdirentries64)",		/* 362 = unimplemented getdirentries64 */
	"#363 (unimplemented getrlimit64)",		/* 363 = unimplemented getrlimit64 */
	"#364 (unimplemented lockf64)",		/* 364 = unimplemented lockf64 */
	"#365 (unimplemented lseek64)",		/* 365 = unimplemented lseek64 */
	"#366 (unimplemented lstat64)",		/* 366 = unimplemented lstat64 */
	"#367 (unimplemented mmap64)",		/* 367 = unimplemented mmap64 */
	"#368 (unimplemented setrlimit64)",		/* 368 = unimplemented setrlimit64 */
	"#369 (unimplemented stat64)",		/* 369 = unimplemented stat64 */
	"#370 (unimplemented truncate64)",		/* 370 = unimplemented truncate64 */
	"#371 (unimplemented ulimit64)",		/* 371 = unimplemented ulimit64 */
	"#372 (unimplemented pread)",		/* 372 = unimplemented pread */
	"#373 (unimplemented preadv)",		/* 373 = unimplemented preadv */
	"#374 (unimplemented pwrite)",		/* 374 = unimplemented pwrite */
	"#375 (unimplemented pwritev)",		/* 375 = unimplemented pwritev */
	"#376 (unimplemented pread64)",		/* 376 = unimplemented pread64 */
	"#377 (unimplemented preadv64)",		/* 377 = unimplemented preadv64 */
	"#378 (unimplemented pwrite64)",		/* 378 = unimplemented pwrite64 */
	"#379 (unimplemented pwritev64)",		/* 379 = unimplemented pwritev64 */
	"#380 (unimplemented setcontext)",		/* 380 = unimplemented setcontext */
	"sigaltstack",			/* 381 = sigaltstack */
	"#382 (unimplemented waitid)",		/* 382 = unimplemented waitid */
	"#383 (unimplemented setpgrp)",		/* 383 = unimplemented setpgrp */
	"#384 (unimplemented recvmsg2)",		/* 384 = unimplemented recvmsg2 */
	"#385 (unimplemented sendmsg2)",		/* 385 = unimplemented sendmsg2 */
	"#386 (unimplemented socket2)",		/* 386 = unimplemented socket2 */
	"#387 (unimplemented socketpair2)",		/* 387 = unimplemented socketpair2 */
	"#388 (unimplemented setregid)",		/* 388 = unimplemented setregid */
	"#389 (unimplemented lwp_create)",		/* 389 = unimplemented lwp_create */
	"#390 (unimplemented lwp_terminate)",		/* 390 = unimplemented lwp_terminate */
	"#391 (unimplemented lwp_wait)",		/* 391 = unimplemented lwp_wait */
	"#392 (unimplemented lwp_suspend)",		/* 392 = unimplemented lwp_suspend */
	"#393 (unimplemented lwp_resume)",		/* 393 = unimplemented lwp_resume */
	"#394 (unimplemented)",		/* 394 = unimplemented */
	"#395 (unimplemented lwp_abort_syscall)",		/* 395 = unimplemented lwp_abort_syscall */
	"#396 (unimplemented lwp_info)",		/* 396 = unimplemented lwp_info */
	"#397 (unimplemented lwp_kill)",		/* 397 = unimplemented lwp_kill */
	"#398 (unimplemented ksleep)",		/* 398 = unimplemented ksleep */
	"#399 (unimplemented kwakeup)",		/* 399 = unimplemented kwakeup */
	"#400 (unimplemented)",		/* 400 = unimplemented */
	"#401 (unimplemented pstat_getlwp)",		/* 401 = unimplemented pstat_getlwp */
	"#402 (unimplemented lwp_exit)",		/* 402 = unimplemented lwp_exit */
	"#403 (unimplemented lwp_continue)",		/* 403 = unimplemented lwp_continue */
	"#404 (unimplemented getacl)",		/* 404 = unimplemented getacl */
	"#405 (unimplemented fgetacl)",		/* 405 = unimplemented fgetacl */
	"#406 (unimplemented setacl)",		/* 406 = unimplemented setacl */
	"#407 (unimplemented fsetacl)",		/* 407 = unimplemented fsetacl */
	"#408 (unimplemented getaccess)",		/* 408 = unimplemented getaccess */
	"#409 (unimplemented lwp_mutex_init)",		/* 409 = unimplemented lwp_mutex_init */
	"#410 (unimplemented lwp_mutex_lock_sys)",		/* 410 = unimplemented lwp_mutex_lock_sys */
	"#411 (unimplemented lwp_mutex_unlock)",		/* 411 = unimplemented lwp_mutex_unlock */
	"#412 (unimplemented lwp_cond_init)",		/* 412 = unimplemented lwp_cond_init */
	"#413 (unimplemented lwp_cond_signal)",		/* 413 = unimplemented lwp_cond_signal */
	"#414 (unimplemented lwp_cond_broadcast)",		/* 414 = unimplemented lwp_cond_broadcast */
	"#415 (unimplemented lwp_cond_wait_sys)",		/* 415 = unimplemented lwp_cond_wait_sys */
	"#416 (unimplemented lwp_getscheduler)",		/* 416 = unimplemented lwp_getscheduler */
	"#417 (unimplemented lwp_setscheduler)",		/* 417 = unimplemented lwp_setscheduler */
	"#418 (unimplemented lwp_getstate)",		/* 418 = unimplemented lwp_getstate */
	"#419 (unimplemented lwp_setstate)",		/* 419 = unimplemented lwp_setstate */
	"#420 (unimplemented lwp_detach)",		/* 420 = unimplemented lwp_detach */
	"mlock",			/* 421 = mlock */
	"munlock",			/* 422 = munlock */
	"mlockall",			/* 423 = mlockall */
	"munlockall",			/* 424 = munlockall */
	"#425 (unimplemented shm_open)",		/* 425 = unimplemented shm_open */
	"#426 (unimplemented shm_unlink)",		/* 426 = unimplemented shm_unlink */
	"#427 (unimplemented sigqueue)",		/* 427 = unimplemented sigqueue */
	"#428 (unimplemented sigwaitinfo)",		/* 428 = unimplemented sigwaitinfo */
	"#429 (unimplemented sigtimedwait)",		/* 429 = unimplemented sigtimedwait */
	"#430 (unimplemented sigwait)",		/* 430 = unimplemented sigwait */
	"#431 (unimplemented aio_read)",		/* 431 = unimplemented aio_read */
	"#432 (unimplemented aio_write)",		/* 432 = unimplemented aio_write */
	"#433 (unimplemented lio_listio)",		/* 433 = unimplemented lio_listio */
	"#434 (unimplemented aio_error)",		/* 434 = unimplemented aio_error */
	"#435 (unimplemented aio_return)",		/* 435 = unimplemented aio_return */
	"#436 (unimplemented aio_cancel)",		/* 436 = unimplemented aio_cancel */
	"#437 (unimplemented aio_suspend)",		/* 437 = unimplemented aio_suspend */
	"#438 (unimplemented aio_fsync)",		/* 438 = unimplemented aio_fsync */
	"#439 (unimplemented mq_open)",		/* 439 = unimplemented mq_open */
	"#440 (unimplemented mq_close)",		/* 440 = unimplemented mq_close */
	"#441 (unimplemented mq_unlink)",		/* 441 = unimplemented mq_unlink */
	"#442 (unimplemented mq_send)",		/* 442 = unimplemented mq_send */
	"#443 (unimplemented mq_receive)",		/* 443 = unimplemented mq_receive */
	"#444 (unimplemented mq_notify)",		/* 444 = unimplemented mq_notify */
	"#445 (unimplemented mq_setattr)",		/* 445 = unimplemented mq_setattr */
	"#446 (unimplemented mq_getattr)",		/* 446 = unimplemented mq_getattr */
	"#447 (unimplemented ksem_open)",		/* 447 = unimplemented ksem_open */
	"#448 (unimplemented ksem_unlink)",		/* 448 = unimplemented ksem_unlink */
	"#449 (unimplemented ksem_close)",		/* 449 = unimplemented ksem_close */
	"#450 (unimplemented ksem_post)",		/* 450 = unimplemented ksem_post */
	"#451 (unimplemented ksem_wait)",		/* 451 = unimplemented ksem_wait */
	"#452 (unimplemented ksem_read)",		/* 452 = unimplemented ksem_read */
	"#453 (unimplemented ksem_trywait)",		/* 453 = unimplemented ksem_trywait */
	"#454 (unimplemented lwp_rwlock_init)",		/* 454 = unimplemented lwp_rwlock_init */
	"#455 (unimplemented lwp_rwlock_destroy)",		/* 455 = unimplemented lwp_rwlock_destroy */
	"#456 (unimplemented lwp_rwlock_rdlock_sys)",		/* 456 = unimplemented lwp_rwlock_rdlock_sys */
	"#457 (unimplemented lwp_rwlock_wrlock_sys)",		/* 457 = unimplemented lwp_rwlock_wrlock_sys */
	"#458 (unimplemented lwp_rwlock_tryrdlock)",		/* 458 = unimplemented lwp_rwlock_tryrdlock */
	"#459 (unimplemented lwp_rwlock_trywrlock)",		/* 459 = unimplemented lwp_rwlock_trywrlock */
	"#460 (unimplemented lwp_rwlock_unlock)",		/* 460 = unimplemented lwp_rwlock_unlock */
	"#461 (unimplemented ttrace)",		/* 461 = unimplemented ttrace */
	"#462 (unimplemented ttrace_wait)",		/* 462 = unimplemented ttrace_wait */
	"#463 (unimplemented lf_wire_mem)",		/* 463 = unimplemented lf_wire_mem */
	"#464 (unimplemented lf_unwire_mem)",		/* 464 = unimplemented lf_unwire_mem */
	"#465 (unimplemented lf_send_pin_map)",		/* 465 = unimplemented lf_send_pin_map */
	"#466 (unimplemented lf_free_buf)",		/* 466 = unimplemented lf_free_buf */
	"#467 (unimplemented lf_wait_nq)",		/* 467 = unimplemented lf_wait_nq */
	"#468 (unimplemented lf_wakeup_conn_q)",		/* 468 = unimplemented lf_wakeup_conn_q */
	"#469 (unimplemented lf_unused)",		/* 469 = unimplemented lf_unused */
	"#470 (unimplemented lwp_sema_init)",		/* 470 = unimplemented lwp_sema_init */
	"#471 (unimplemented lwp_sema_post)",		/* 471 = unimplemented lwp_sema_post */
	"#472 (unimplemented lwp_sema_wait)",		/* 472 = unimplemented lwp_sema_wait */
	"#473 (unimplemented lwp_sema_trywait)",		/* 473 = unimplemented lwp_sema_trywait */
	"#474 (unimplemented lwp_sema_destroy)",		/* 474 = unimplemented lwp_sema_destroy */
	"#475 (unimplemented statvfs64)",		/* 475 = unimplemented statvfs64 */
	"#476 (unimplemented fstatvfs64)",		/* 476 = unimplemented fstatvfs64 */
	"#477 (unimplemented msh_register)",		/* 477 = unimplemented msh_register */
	"#478 (unimplemented ptrace64)",		/* 478 = unimplemented ptrace64 */
	"#479 (unimplemented sendfile)",		/* 479 = unimplemented sendfile */
	"#480 (unimplemented sendpath)",		/* 480 = unimplemented sendpath */
	"#481 (unimplemented sendfile64)",		/* 481 = unimplemented sendfile64 */
	"#482 (unimplemented sendpath64)",		/* 482 = unimplemented sendpath64 */
	"#483 (unimplemented modload)",		/* 483 = unimplemented modload */
	"#484 (unimplemented moduload)",		/* 484 = unimplemented moduload */
	"#485 (unimplemented modpath)",		/* 485 = unimplemented modpath */
	"#486 (unimplemented getksym)",		/* 486 = unimplemented getksym */
	"#487 (unimplemented modadm)",		/* 487 = unimplemented modadm */
	"#488 (unimplemented modstat)",		/* 488 = unimplemented modstat */
	"#489 (unimplemented lwp_detached_exit)",		/* 489 = unimplemented lwp_detached_exit */
	"#490 (unimplemented crashconf)",		/* 490 = unimplemented crashconf */
	"#491 (unimplemented siginhibit)",		/* 491 = unimplemented siginhibit */
	"#492 (unimplemented sigenable)",		/* 492 = unimplemented sigenable */
	"#493 (unimplemented spuctl)",		/* 493 = unimplemented spuctl */
	"#494 (unimplemented zerokernelsum)",		/* 494 = unimplemented zerokernelsum */
	"#495 (unimplemented nfs_kstat)",		/* 495 = unimplemented nfs_kstat */
	"#496 (unimplemented aio_read64)",		/* 496 = unimplemented aio_read64 */
	"#497 (unimplemented aio_write64)",		/* 497 = unimplemented aio_write64 */
	"#498 (unimplemented aio_error64)",		/* 498 = unimplemented aio_error64 */
	"#499 (unimplemented aio_return64)",		/* 499 = unimplemented aio_return64 */
	"#500 (unimplemented aio_cancel64)",		/* 500 = unimplemented aio_cancel64 */
	"#501 (unimplemented aio_suspend64)",		/* 501 = unimplemented aio_suspend64 */
	"#502 (unimplemented aio_fsync64)",		/* 502 = unimplemented aio_fsync64 */
	"#503 (unimplemented lio_listio64)",		/* 503 = unimplemented lio_listio64 */
	"#504 (unimplemented recv2)",		/* 504 = unimplemented recv2 */
	"#505 (unimplemented recvfrom2)",		/* 505 = unimplemented recvfrom2 */
	"#506 (unimplemented send2)",		/* 506 = unimplemented send2 */
	"#507 (unimplemented sendto2)",		/* 507 = unimplemented sendto2 */
	"#508 (unimplemented acl)",		/* 508 = unimplemented acl */
	"#509 (unimplemented __cnx_p2p_ctl)",		/* 509 = unimplemented __cnx_p2p_ctl */
	"#510 (unimplemented __cnx_gsched_ctl)",		/* 510 = unimplemented __cnx_gsched_ctl */
	"#511 (unimplemented __cnx_pmon_ctl)",		/* 511 = unimplemented __cnx_pmon_ctl */
	"#512 (unimplemented mem_res_grp)",		/* 512 = unimplemented mem_res_grp */
	"#513 (unimplemented fabric)",		/* 513 = unimplemented fabric */
	"#514 (unimplemented diagsyscall)",		/* 514 = unimplemented diagsyscall */
	"#515 (unimplemented tuneinfo)",		/* 515 = unimplemented tuneinfo */
	"#516 (unimplemented gettune)",		/* 516 = unimplemented gettune */
	"#517 (unimplemented settune)",		/* 517 = unimplemented settune */
	"#518 (unimplemented pset_create)",		/* 518 = unimplemented pset_create */
	"#519 (unimplemented pset_destroy)",		/* 519 = unimplemented pset_destroy */
	"#520 (unimplemented pset_assign)",		/* 520 = unimplemented pset_assign */
	"#521 (unimplemented pset_bind)",		/* 521 = unimplemented pset_bind */
	"#522 (unimplemented pset_getattr)",		/* 522 = unimplemented pset_getattr */
	"#523 (unimplemented pset_setattr)",		/* 523 = unimplemented pset_setattr */
	"#524 (unimplemented pset_ctl)",		/* 524 = unimplemented pset_ctl */
	"#525 (unimplemented pset_rtctl)",		/* 525 = unimplemented pset_rtctl */
};
