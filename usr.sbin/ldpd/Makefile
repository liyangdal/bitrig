#	$OpenBSD: Makefile,v 1.5 2013/06/04 02:25:28 claudio Exp $

PROG=	ldpd
SRCS=	accept.c adjacency.c address.c control.c hello.c init.c interface.c \
	keepalive.c kroute.c labelmapping.c lde.c lde_lib.c ldpd.c ldpe.c \
	log.c neighbor.c notification.c packet.c parse.y printconf.c

MAN=	ldpd.8 ldpd.conf.5

CFLAGS+= -Wall -I${.CURDIR}
CFLAGS+= -Wstrict-prototypes -Wmissing-prototypes
CFLAGS+= -Wmissing-declarations
CFLAGS+= -Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+= -Wsign-compare
YFLAGS=
LDADD+=	-levent -lutil
DPADD+= ${LIBEVENT} ${LIBUTIL}

.include <bsd.prog.mk>
