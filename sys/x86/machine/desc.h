#ifndef	MACHINE_DESC_H
#define	MACHINE_DESC_H

#define	GKPL	 0
#define	GUPL	 3

#define	GKCS	 1
#define	GKDS	 2
#define	GTSS	 3
#define	GUCS	 5
#define	GUDS	 6
#define	GSEL(n)	((n) << 3)

#define	ITASK	 5
#define	IINTR	 0xe
#define	ITRAP	 0xf

#define	ICOUNT	256

#ifndef	__ASSEMBLER__

#include <sys/types.h>

struct gdesc {
	uint16_t	losize;
	uint32_t	lobase	: 24;
	uint8_t		a	: 1;
	uint8_t		rw	: 1;
	uint8_t		dc	: 1;
	uint8_t		x	: 1;
	uint8_t		t	: 1;
	uint8_t		dpl	: 2;
	uint8_t		p	: 1;
	uint8_t		hisize	: 4;
	uint8_t		/**/	: 1;
	uint8_t		lm	: 1;
	uint8_t		pm	: 1;
	uint8_t		g	: 1;
	uint8_t		hibase;
} __attribute__((packed));

struct idesc {
	uint16_t	lo;
	uint16_t	cs;
	uint8_t		ist	: 3;
	uint8_t		/**/	: 5;
	uint8_t		t	: 4;
	uint8_t		/**/	: 1;
	uint8_t		dpl	: 2;
	uint8_t		p	: 1;
	uint64_t	hi	: 48;
	uint32_t	/**/	: 32;
} __attribute__((packed));

struct rdesc {
	uint16_t	size;
	uint64_t	base;
} __attribute__((packed));

struct tdesc {
	uint32_t	/**/	: 32;
	uint64_t	rsp[3];
	uint64_t	ist[8];
	uint64_t	/**/	: 64;
	uint16_t	/**/	: 16;
	uint16_t	iobase;
} __attribute__((packed));

#endif

#endif
