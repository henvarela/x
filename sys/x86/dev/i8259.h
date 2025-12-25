#ifndef	X86_DEV_I8259_H
#define	X86_DEV_I8259_H

#include <machine/asm.h>

#define	I8259_MASTER	0x20
#	define	I8259_MASTER_OCW1	0xff
#define	I8259_SLAVE	0xa0
#	define	I8259_SLAVE_OCW1	0xff

#define	I8259_DISABLE() do {						      \
	OUTB(I8259_MASTER_OCW1, I8259_MASTER+1);			      \
	OUTB(I8259_SLAVE_OCW1, I8259_SLAVE+1);				      \
} while (0)

#endif
