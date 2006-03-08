/*	$OpenBSD: worklist.c,v 1.1 2006/03/08 20:18:41 joris Exp $	*/
/*
 * Copyright (c) 2006 Joris Vink <joris@openbsd.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL  DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "includes.h"

#include "log.h"
#include "worklist.h"
#include "xmalloc.h"

/*
 * adds an path to a worklist.
 */
void
cvs_worklist_add(const char *path, struct cvs_wklhead *worklist)
{
	size_t len;
	struct cvs_worklist *wkl;
	sigset_t old, new;

	wkl = (struct cvs_worklist *)xmalloc(sizeof(*wkl));
	memset(wkl, 0, sizeof(*wkl));

	len = strlcpy(wkl->wkl_path, path, sizeof(wkl->wkl_path));
	if (len >= sizeof(wkl->wkl_path))
		fatal("path truncation in cvs_worklist_add");

	sigfillset(&new);
	sigprocmask(SIG_BLOCK, &new, &old);
	SLIST_INSERT_HEAD(worklist, wkl, wkl_list);
	sigprocmask(SIG_SETMASK, &old, NULL);
}

/*
 * run over the given worklist, calling cb for each element.
 * this is just like cvs_worklist_clean(), except we block signals first.
 */
void
cvs_worklist_run(struct cvs_wklhead *list, void (*cb)(struct cvs_worklist *))
{
	sigset_t old, new;

	sigfillset(&new);
	sigprocmask(SIG_BLOCK, &new, &old);

	cvs_worklist_clean(list, cb);

	sigprocmask(SIG_SETMASK, &old, NULL);
}

/*
 * clean the worklist by passing the elements to the specified callback
 */
void
cvs_worklist_clean(struct cvs_wklhead *list, void (*cb)(struct cvs_worklist *))
{
	struct cvs_worklist *wkl;

	while ((wkl = SLIST_FIRST(list)) != NULL) {
		SLIST_REMOVE_HEAD(list, wkl_list);
		cb(wkl);
		xfree(wkl);
	}
}

void
cvs_worklist_unlink(struct cvs_worklist *wkl)
{
	(void)unlink(wkl->wkl_path);
}
