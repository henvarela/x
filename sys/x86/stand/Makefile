SYSDIR= ../..
include ${SYSDIR}/Makefile.inc

PROG=	mbr
OBJS=	${PROG}.o
SRCS=	${PROG}.S

CFLAGS:=${CFLAGS:-mcmodel%=-m16}

build:	${PROG}

${PROG}:${OBJS}
	${LD} ${LDFLAGS} -T$@.ld -o $@ ${ALLSRCS}
	objcopy -O binary -R .shstrtab $@

clean:
	rm -f ${OBJS} ${PROG}
