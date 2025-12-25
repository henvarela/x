#include <machine/asm.h>
#include <machine/param.h>

/*
 * Interface:	int _memcmp(const void *dst, const void *src, size_t len).
 */
GENTRY(_memcmp):
	/* Compare the destination and source operands word by word. */
	movq	%rdx,%rcx
	shrq	$WSHIFT,%rcx
	repe	cmpsq

	jne	0f

	/* Compare for the remaining length byte by byte. */
	movb	%dl,%cl
	andb	$WMASK,%cl
	repe	cmpsb

	/* Compute the difference, then return it. */
	movzbl	-1(%rdi),%eax
	movzbl	-1(%rsi),%edx
	subl	%edx,%eax
	retq

0:	/* Recompare the last word byte by byte. */
	movq	$WSIZE,%rcx
	subq	%rcx,%rdi
	subq	%rcx,%rsi
	repe	cmpsb

	/* Compute the difference, then return it. */
	movzbl	-1(%rdi),%eax
	movzbl	-1(%rsi),%edx
	subl	%edx,%eax
	retq

/*
 * Interface:	void *_memmove(void *dst, const void *src, size_t len).
 */
GENTRY(_memmove):
	/* Check for an overlap between the operands. */
	movq	%rdi,%rax
	subq	%rsi,%rax
	cmpq	%rdx,%rax
	jb	0f

/*
 * Interface:	void *_memcpy(void *dst, const void *src, size_t len).
 */
GENTRY(_memcpy):
	movq	%rdi,%rax

	/* Copy byte by byte 'til length becomes a multiple of word size. */
	movq	%rdx,%rcx
	andq	$WMASK,%rcx
	rep	movsb

	/* Copy any remaining data word by word. */
	movq	%rdx,%rcx
	shrq	$WSHIFT,%rcx
	rep	movsq

	retq

0:	/* Prepare for backwards copy. */
	std
	movq	%rdi,%rax
	addq	%rdx,%rdi;	decq	%rdi
	addq	%rdx,%rsi;	decq	%rsi

	/* Copy byte by byte 'til length becomes a multiple of word size. */
	movq	%rdx,%rcx
	andb	$WMASK,%cl
	rep	movsb

	/* Copy any remaining data word by word. */
	movq	%rdx,%rcx
	shrq	$WSHIFT,%rcx
	subq	$WMASK,%rdi
	subq	$WMASK,%rsi
	rep	movsq

	cld
	retq

/*
 * Interface:	void *_memset(void *dst, int chr, size_t len).
 */
GENTRY(_memset):
	movq	%rdi,%r8

	/* Prepare the source operand for word-sized operations. */
	movb	%sil,%al
	movb	%al,%ah
	movw	%ax,%si
	shll	$16,%eax
	orw	%si,%ax
	movl	%eax,%esi
	shlq	$32,%rax
	orq	%rsi,%rax

	/* Set 'til destination is aligned to word boundaries. */
	movq	%rdi,%rcx
	negq	%rcx
	andq	$WMASK,%rcx
	rep	stosb

	/* Set for the remaining of length word by word. */
	movb	%dl,%cl
	shrb	$WSHIFT,%cl
	rep	stosq

	/* Set any remaining bytes. */
	movb	%dl,%cl
	andb	$WMASK,%cl
	rep	stosb

	movq	%r8,%rax
	retq

/*
 * Interface:	size_t _strlen(const void *str).
 */
.globl	_strlen
_strlen:
	movq	%rdi,%rdx
	xorb	%al,%al
	movq	$-1,%rcx
	repne	scasb

	subq	%rdx,%rdi
	movq	%rdi,%rax
	decq	%rax

	retq
