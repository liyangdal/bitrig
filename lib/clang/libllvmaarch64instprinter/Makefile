# $FreeBSD$

.include <bsd.own.mk>

LIB=	llvmaarch64instprinter

SRCDIR=	lib/Target/AArch64/InstPrinter
INCDIR=	lib/Target/AArch64
SRCS=	AArch64InstPrinter.cpp

TGHDRS=	AArch64GenAsmWriter \
	AArch64GenInstrInfo \
	AArch64GenRegisterInfo \
	AArch64GenSubtargetInfo

.include "../clang.lib.mk"
