SYSDIR=	.
include ${SYSDIR}/Makefile.inc

SUBDIR=	${MACHINE}

build:
	@for d in ${SUBDIR}; do make -C $$d $@; done

clean:
	@for d in ${SUBDIR}; do make -C $$d $@; done
