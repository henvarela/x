/*
 * Boot sector layout:
 * 0                              478   510
 * +------------------------------+-----+------+
 * |           THIS FILE          | GDT | 55AA |
 * +------------------------------+-----+------+
 *
 * Memory layout:
 * +-------+
 * | IVT   | 000000..0003ff
 * +-------+
 * | BDA   | 000400..0004ff
 * +-------+
 * |  ...  |
 * +-------+
 * | EBDA  | 080000..09ffff
 * +-------+
 * | VIDEO | 0a0000..0c7fff
 * +-------+
 * | BIOSX | 0c8000..0effff
 * +-------+
 * | ROM   | 0f0000..0fffff
 * +-------+
 * |  ...  |
 * +-------+
 */

#define	BIT(N)	(1ULL << (N))

#define	CS16	 0
#define	DS16	 CS16
#define	CS32	(gkcs32 - gdtbeg)
#define	DS32	(gkds32 - gdtbeg)
#define	CS64	(gkcs64 - gdtbeg)
#define	DS64	(gkds32 - gdtbeg)
#define	EFER	 0xc0000080

#define	CR0_PG	 BIT(31)
#define	CR0_PM	 BIT(0)
#define	CR4_PAE	 BIT(5)
#define	EFER_LM  BIT(8)

#define	STACK16	 0x7bfe	/* NOTE: layout */
#define	STACK32	 0x7fffc
#define	STACK64	 0x1ffff8

#define	GDT	 478	/* NOTE: layout */

#define	PT4	 0x4000
#define	PT3	 0x3000
#define	PT2	 0x2000	/* huge page */

#define	NOP	 0x90
#define	INB(s)	 movw $(s),%dx; inb %dx,%al
#define	OUTB(s, d) movw $(d),%dx; movb s,%al; outb %al,%dx

#define	ATA_IO	0x1f0
#define	ATA_CNT	0x1f2
#define	ATA_LO	0x1f3
#define	ATA_MID	0x1f4
#define	ATA_HI	0x1f5
#define	ATA_DEV	0x1f6
#	define	ATA_DEV_MASTER	 0
#	define	ATA_DEV_SLAVE	 0xff
#	define	ATA_DEV_FLAGS	(BIT(7) | BIT(6) | BIT(5))
#define	ATA_STAT	0x1f7
#	define	ATA_STAT_BSY	 BIT(7)
#	define	ATA_STAT_READ	 BIT(5)

#include <machine/param.h>

.text

.code16

16:	/* Initialize registers. */
	ljmpw	$CS16,$0f
0:	cli

	/* Align the stack pointers. */
	movw	$STACK16,%sp
	movw	%sp,%bp

	/* Prepare for the 32-bit mode activation. */
	lgdtl	gdtdesc

	/* Enable 32-bit mode. */
	movl	%cr0,%eax
	orb	$CR0_PM,%al
	movl	%eax,%cr0

	/* Turn 32-bit mode on. */
	ljmpl	$CS32,$32f

.code32

.p2align 2,NOP
32:	cli
	/* Select a data descriptor to use from now on. */
	movw	$DS32,%ax
	movw	%ax,%es

	/* Identity map the first 2MB and KVAS. */
	movl	$PT4,%ebx
	movl	$(PT3+3),%es:(%ebx)
	movl	$(PT3+3),%es:(8*PTI4(KVIRT))(%ebx)
	movl	$PT3,%ebx
	movl	$(PT2+3),%es:(%ebx)
	movl	$(PT2+3),%es:(8*PTI3(KVIRT))(%ebx)
	movl	$PT2,%ebx
	movl	$0x83,%es:(%ebx)

	/* Enable 8-bytes long PTEs. */
	movl	%cr4,%eax
	orl	$CR4_PAE,%eax
	movl	%eax,%cr4

	/* Enable 64-bit mode. */
	movl	$EFER,%ecx
	rdmsr
	orl	$EFER_LM,%eax
	wrmsr

	/* Load the root page table. */
	movl	$PT4,%ebx
	movl	%ebx,%cr3

	/* Enable paging and compatibility mode. */
	movl	%cr0,%eax
	orl	$CR0_PG,%eax
	movl	%eax,%cr0

	/* Turn 64-bit mode on. */
	ljmpl	$CS64,$64f

.code64

.p2align 3,NOP
64:	cli
	/* Align the stack. */
	movq	$STACK64,%rsp

	/*
	 * Load the kernel from the disk warmed up by the BIOS.
	 */
	movq	$KPHYS,%rdi	/* destination */
	movq	$KLBA,%rsi	/* source      */
	movq	$KLEN,%r8	/* length      */
0:
	movq	%rsi,%r10;	shrq	$8,%r10
	movq	%rsi,%r11;	shrq	$16,%r11
	movq	%rsi,%r12;	shrq	$24,%r12
	orq	$(ATA_DEV_FLAGS + ATA_DEV_MASTER),%r12
	OUTB($1,	ATA_CNT);
	OUTB(%sil,	ATA_LO);
	OUTB(%r10b,	ATA_MID);
	OUTB(%r11b,	ATA_HI);
	OUTB(%r12b,	ATA_DEV);
	OUTB($ATA_STAT_READ, ATA_STAT);
1:	/* Poll 'til it's ready to load the input data to KPHYS. */
	INB(ATA_STAT);
	andb	$ATA_STAT_BSY,%al
	jnz	1b
	/* Load a kernel block to memory. */
	movw	$ATA_IO,%dx
	movq	$(512 >> 2),%rcx
	rep	insl
	/* Do the same thing for the next block. */
	decq	%r8
	incq	%rsi
	cmpq	$0,%r8
	jne	0b

	/* Yield to the kernel hoping it never RETurns. */
	movabsq	$KVADDR(KPHYS),%rax
	jmpq	*%rax

/*
 * Global Descriptor Table Descriptor:
 */
gdtdesc:
	.2byte	gdtend - gdtbeg - 1
	.4byte	gdtbeg, 0

/*
 * Global Descriptor Table:
 */
.org	GDT
gdtbeg:
0:	.zero	8
gkcs32:	.8byte	0xcf9b000000ffff /* 0..4G, G=4K, A, RWX, P */
gkds32:	.8byte	0xcf93000000ffff /* 0..4G, G=4K, A, RW-, P */
gkcs64:	.8byte	0xaf9b000000ffff /* 0..4G, G=4K, A, RWX, P */
gdtend:

/* Write the boot signature. */
.org	510
bsig:	.2byte	0xaa55
