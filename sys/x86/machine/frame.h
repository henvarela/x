#ifndef	MACHINE_FRAME_H
#define	MACHINE_FRAME_H

#include <sys/types.h>

struct frame {
	/* software-defined */

	register_t	rdi;
	register_t	rsi;
	register_t	rdx;
	register_t	rcx;
	register_t	r8;
	register_t	r9;

	register_t	rax;
	register_t	rbx;
	register_t	r10;
	register_t	r11;
	register_t	r12;
	register_t	r13;
	register_t	r14;
	register_t	r15;

	register_t	gs;
	register_t	fs;
	register_t	es;
	register_t	ds;

	register_t	cr3;

	register_t	rbp;

	/* hardware-defined */
	word_t		vec;	/* optional */
	word_t		err;
	register_t	rip;
	register_t	cs;
	register_t	rflags;
	register_t	rsp;
	register_t	ss;
};

#endif
