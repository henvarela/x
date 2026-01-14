include ../config.mk

DISK	= disk.img

all:	img

build:
	make -C .. $@

clean:
	make -C .. $@
	rm -f ${DISK}

disk:	${DISK}

img:	build ${MACHINE}

${DISK}:
	dd if=/dev/zero of=$@ count=4096

x86:	disk
	dd if=../sys/x86/stand/mbr	of=${DISK} conv=notrunc
	dd if=../sys/x86/x86/kern	of=${DISK} conv=notrunc seek=1

.PHONY:	all build clean disk img x86
