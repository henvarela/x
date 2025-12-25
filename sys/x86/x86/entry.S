#include <machine/asm.h>
#include <machine/desc.h>
#include <machine/param.h>

.text

/* Setup the kernel stack. */
	movq	%rsp,%rbp
	movabsq	$KBEG,%rsp

/* Reset rFLAGS to its hard-wired value. */
	pushq	$2
	popfq

/* Load a trustworthy page map. */
	movq	$KPADDR(kpt),%rbx
	movq	%rbx,%cr3

/* Load a trustworthy GDT. */
	movabsq	$gdtr,%rbx
	lgdtq	(%rbx)

/* Reset the data segment registers. */
	movw	$GSEL(GKDS),%bx
	movw	%bx,%ds
	movw	%bx,%es
	movw	%bx,%fs
	movw	%bx,%gs
	movw	%bx,%ss

/* Reset the code segment register. */
	pushq	$GSEL(GKCS)
	movabsq	$0f,%rbx
	pushq	%rbx
	lretq

0:	callq	x86
	callq	kern
0:	hlt
	jmp	0b
