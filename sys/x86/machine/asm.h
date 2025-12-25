#ifndef	MACHINE_ASM_H
#define	MACHINE_ASM_H

#include <machine/param.h>

#ifndef	asm
#	define	asm	__asm__
#endif

#define	CLI()								      \
	asm("cli")

#define	GENTRY(e)							      \
	.align	WSIZE,0x90;/* NOP-filled in case of fallthrough */	      \
	.globl	e;							      \
	e

#define	HLT()								      \
	asm("hlt")

#define	LENTRY(e)							      \
	.align	WSIZE,0x90;/* NOP-filled in case of fallthrough */	      \
	e

#define	LIDT(m)								      \
	asm("lidtq %0" : : "m"(m))

#define	LTR(r)								      \
	asm("ltrw %w0" : : "r"(r))

#define	OUTB(s, d)							      \
	asm("outb %b0, %w1" : : "a"(s), "d"(d))				      \

#define	STI()								      \
	asm("sti")

#endif
