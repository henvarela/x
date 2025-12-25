#include <machine/asm.h>
#include <machine/intr.h>
#include <machine/trap.h>

.text

/*
 * Call handler, passing the machine frame as the argument.
 */
GENTRY(fcall):
	pushq	%rbp

	movq	%cr3,%rbp;
	subq	$8,%rsp
	movl	%ebp,(%rsp)
	movl	$0,4(%rsp)

	movq	%ds,%rbp;	pushq	%rbp
	movq	%es,%rbp;	pushq	%rbp
	movq	%fs,%rbp;	pushq	%rbp
	movq	%gs,%rbp;	pushq	%rbp

	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%r11
	pushq	%r10
	pushq	%rbx
	pushq	%rax

	pushq	%r9
	pushq	%r8
	pushq	%rcx
	pushq	%rdx
	pushq	%rsi
	pushq	%rdi

	movq	%rsp,%rdi
	callq	hand

/*
 * Return from passing the machine frame as the argument.
 */
LENTRY(fret):
	popq	%rdi
	popq	%rsi
	popq	%rdx
	popq	%rcx
	popq	%r8
	popq	%r9

	popq	%rax
	popq	%rbx
	popq	%r10
	popq	%r11
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15

	popq	%rbp;	movq	%rbp,%gs
	popq	%rbp;	movq	%rbp,%fs
	popq	%rbp;	movq	%rbp,%es
	popq	%rbp;	movq	%rbp,%ds

	popq	%rbp;	movq	%rbp,%cr3

	popq	%rbp

	addq	$16,%rsp

	iretq

#define	TRAP0(n)	LENTRY(n): pushq $0; pushq $n; jmp fcall;
#define	TRAP_(n)	LENTRY(n): /*******/ pushq $n; jmp fcall;
#define	INTR(n)		TRAP0(n)

/*
 * Local vector entries:
 */
TRAP0(TDE)	TRAP0(TDB)	TRAP0(TNMI)	TRAP0(TBP)
TRAP0(TOF)	TRAP0(TBR)	TRAP0(TUD)	TRAP0(TNM)
TRAP_(TDF)	/*  09  */	TRAP_(TTS)	TRAP_(TNP)
TRAP_(TSS)	TRAP_(TGP)	TRAP_(TPF)	/*  15  */
TRAP0(TMF)	TRAP_(TAC)	TRAP0(TMC)	TRAP0(TXM)
TRAP0(TVE)	/*  21  */	/*  22  */	/*  23  */
/*  24  */	/*  25  */	/*  26  */	/*  27  */
/*  28  */	/*  29  */	/*  30  */	/*  31  */
TRAP0(TSC)	/*  ... */	INTR(IPIT)	INTR(ISPU)

.section .rodata

#define	_CAT(a, b)	a##b
#define	CAT(a, b)	_CAT(a, b)
#define	_VEC(n)		CAT(n, b)
#define	VEC(n)		.org vecs + ((n) << 3);	.8byte _VEC(n);

/*
 * Interface:	void (*const vecs[ICOUNT])(void).
 */
GENTRY(vecs):
	VEC(TDE)	VEC(TDB)	VEC(TNMI)	VEC(TBP)
	VEC(TOF)	VEC(TBR)	VEC(TUD)	VEC(TNM)
	VEC(TDF)	/* 09 */	VEC(TTS)	VEC(TNP)
	VEC(TSS)	VEC(TGP)	VEC(TPF)	/* 15 */
	VEC(TMF)	VEC(TAC)	VEC(TMC)	VEC(TXM)
	VEC(TVE)	/* 21 */	/* 22 */	/* 23 */
	/* 24 */	/* 25 */	/* 26 */	/* 27 */
	/* 28 */	/* 29 */	/* 30 */	/* 31 */
	VEC(TSC)	/* ... */	VEC(IPIT)	VEC(ISPU)
