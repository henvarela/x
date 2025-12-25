SYSDIR= ../..
include ${SYSDIR}/Makefile.inc

# gmake and bmake interoperability:
VPATH=	${MACHDIR}/dev:${MACHDIR}/lib:${SYSDIR}/kern:${SYSDIR}/lib
.PATH:	${VPATH}

PROG=	kern
OBJS=	${SRCS:.S=.o}
SRCS=	entry.S pmap.c vec.S x86.c\
	i82489.c vga.c\
	libx86.S\
	alloc.c fork.c init.c kern.c printf.c\
	libk.c
OBJS:=	${OBJS:.c=.o}

build:	${PROG}

${PROG}:${OBJS}
	${CC} ${CFLAGS} -E -P -w -xc -U__STDC__ ${MACHDIR}/conf/${MACHINE}.ld|\
	${LD} ${LDFLAGS} -o $@ ${ALLSRCS} -T/dev/stdin
	objcopy -O binary -R .shstrtab $@

clean:
	rm -f ${OBJS} ${PROG}
