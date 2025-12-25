SYSDIR=	sys
include ${SYSDIR}/Makefile.inc

DISK=	disk.img
SUBDIR=	sys

build:
	@for d in ${SUBDIR}; do make -C $$d $@; done

clean:
	@for d in ${SUBDIR}; do make -C $$d $@; done
	rm -f ${DISK}

${DISK}:
	dd if=/dev/zero of=$@ count=4096

img:	build ${DISK} ${MACHINE}

x86:	build ${DISK}
	dd if=${MACHDIR}/stand/mbr	of=${DISK} conv=notrunc
	dd if=${MACHDIR}/x86/kern	of=${DISK} conv=notrunc seek=1

.PHONY:	img x86
