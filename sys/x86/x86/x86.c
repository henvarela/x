#include <machine/asm.h>
#include <machine/desc.h>
#include <machine/frame.h>
#include <machine/intr.h>
#include <machine/param.h>
#include <machine/pmap.h>
#include <machine/trap.h>

#include <sys/alloc.h>
#include <sys/lib.h>
#include <sys/proc.h>
#include <sys/sched.h>
#include <sys/systm.h>
#include <sys/types.h>

#include <x86/dev/i82489.h>
#include <x86/dev/i8259.h>
#include <x86/dev/vga.h>

#include <dev/cons.h>
#include <dev/pic.h>
#include <dev/pit.h>

struct gdesc gdt[] = {
	[GKCS] = {
		.a	= 1,
		.rw	= 1,
		.x	= 1,
		.t	= 1,
		.dpl	= GKPL,
		.p	= 1,
		.lm	= 1,
		.g	= 1
	}, [GKDS] = {
		.losize	= 0xffff,
		.a	= 1,
		.rw	= 1,
		.t	= 1,
		.dpl	= GKPL,
		.p	= 1,
		.hisize	= 0xf,
		.lm	= 1,
		.g	= 1
	}, [GTSS] = {
		.losize	= sizeof(struct tdesc) - 1,
		.a	= 1,
		.x	= 1,
		.dpl	= GKPL,
		.p	= 1
		/* ginit() */
	}, [GTSS + 1] = {
		/* ginit() */0
	}, [GUCS] = {
		.a	= 1,
		.rw	= 1,
		.x	= 1,
		.t	= 1,
		.dpl	= GUPL,
		.p	= 1,
		.lm	= 1,
		.g	= 1
	}, [GUDS] = {
		.losize	= 0xffff,
		.a	= 1,
		.rw	= 1,
		.t	= 1,
		.dpl	= GUPL,
		.p	= 1,
		.hisize	= 0xf,
		.lm	= 1,
		.g	= 1
	}
};

struct idesc	idt[ICOUNT];
struct rdesc	gdtr	= { sizeof(gdt) - 1, (uint64_t)gdt };
struct rdesc	idtr	= { sizeof(idt) - 1, (uint64_t)idt };
struct tdesc	tss	= { .rsp[0] = KEND + PSIZE };

extern void (*const vecs[ICOUNT])(void);

static const char *tmsg[TCOUNT] = {
	[TDE]	= "Divide error",
	[TDB]	= "Debug exception",
	[TNMI]	= "Nonmaskable interrupt",
	[TBP]	= "Breakpoint",
	[TOF]	= "Overflow",
	[TBR]	= "Bounds check",
	[TUD]	= "Invalid opcode",
	[TNM]	= "Device unavailable",
	[TDF]	= "Double fault",
	[TTS]	= "Invalid TSS",
	[TNP]	= "Segment not present",
	[TSS]	= "Stack fault",
	[TGP]	= "General protection",
	[TPF]	= "Page fault",
	[TMF]	= "Floating-point exception",
	[TAC]	= "Alignment check",
	[TMC]	= "Machine check",
	[TXM]	= "SIMD exception",
	[TVE]	= "Virtualization exception",
	[TSC]	= "System call"
};

pte_t						__attribute__((aligned(PSIZE)))
pt2[PSIZE / sizeof(pte_t)] = {
	[PTI2(KVIRT)] = PS + PRW + PV
}, pt3[PSIZE / sizeof(pte_t)] = {
	[PTI3(KVIRT)] = KPADDR(pt2) + PRW + PV
}, kpt[PSIZE / sizeof(pte_t)] = {
	[PTI4(KVIRT)] = KPADDR(pt3) + PRW + PV
};

pmap_t kpm = {
	.pt	= kpt,
	.map	= {
		.va	= KVIRT,
		.pa	= 0, /* not to be mistaken by KPHYS */
		.len	= PSIZE2
	}
};

struct proc kp = {
	/* kpinit() */
	.next	= { &kp, NULL },
	.name	= "kern"
}, *pc = &kp;

struct cons vga = {
	.init	= vinit,
	.map	= vmap,
	.putc	= vputc,
	.hw	= "ISA VGA"
}, *cc = &vga;

struct pic i82489c = {
	.ack	= i82489c_ack,
	.init	= i82489c_init,
	.map	= i82489_map,
	.mask	= i82489c_mask,
	.unmask	= i82489c_unmask,
	.hw	= "i82489"
}, *pic = &i82489c;

struct pit i82489t = {
	.init	= i82489t_init,
	.map	= i82489_map,
	.oshot	= i82489t_oshot,
	.nshot	= i82489t_nshot,
	.hw	= "i82489"
}, *pit = &i82489t;

static void
iset(void (*const v)(void), int i, int s, int t, int p)
{
	idt[i] = (struct idesc){
		.lo	= (uint16_t)(uint64_t)(v),
		.cs	= s,
		.t	= t,
		.dpl	= p,
		.p	= 1,
		.hi	= (uint64_t)(v) >> 16
	};
}

static void
ginit(void)
{
	gdt[GTSS].lobase = (size_t)&tss & 0xffffff;
	gdt[GTSS].hibase = (size_t)&tss >> 24 & 0xff;

	gdt[GTSS + 1].losize = (size_t)&tss >> 32 & 0xffff;
	gdt[GTSS + 1].lobase = (size_t)&tss >> 48 & 0xffff;
}

static void
iinit(void)
{
	int i;

	for (i = 0; i < TCOUNT; i++) /* traps */
		iset(vecs[i], i, GSEL(GKCS), ITRAP, GKPL);

	for (; i < ICOUNT; i++) /* interrupts */
		iset(vecs[i], i, GSEL(GKCS), IINTR, GKPL);

	iset(vecs[TSC], TSC, GSEL(GKCS), ITRAP, GUPL); /* syscall */
}

static void
kpinit(void)
{
	kp.p.pm = kpm;
}

void
hand(struct frame *f)
{
	if (f->vec == IPIT) {
		/* Switch context. */
		pc->p.ctx = *f;
		SCHED();
		*f = pc->p.ctx;

		pic->ack();

	} else { /* panic */
		printf(tmsg[f->vec]);
		CLI();
		HLT();
	}
}

int
_fork(const struct proc *p, struct proc *c)
{
	if (!(p && c))
		return (-1);

	/* Fork this context. */
	HLT();
	c->p.ctx = p->p.ctx;

	/*
	 * It would be nicer to setup the child's context so that it returns
	 * straight to work instead of right here.
	 */
	if (pc == c)
		return (0);

	/* Fork the stack. */
	c->p.ctx.rsp = (register_t)malloc(PSIZE);
	if (!c->p.ctx.rsp)
		return (-1);
	memcpy((void *)c->p.ctx.rsp, (void *)p->p.ctx.rsp, PSIZE);

	/*
	 * Fork the memory address space.
	 *
	 * TODO: pmcopy() is not pmfork() XXX
	 * NOTE: Processes that share all physical memory behave like threads.
	 */
	c->p.pm = pmcopy(&kpm);
	if (!c->p.pm.pt)
		return (-1);
	c->p.ctx.cr3 = (register_t)KPADDR(c->p.pm.pt);

	return (0);
}

void
x86(void)
{
	ginit();
	LTR(GSEL(GTSS));
	LIDT(idtr);
	iinit();
	kpinit();
	I8259_DISABLE();
	cc->init();
	pic->init();
	pit->init();
	STI();
	HLT();/* Initialize this process' context. */
}
