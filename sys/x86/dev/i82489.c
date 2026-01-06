#define	READ(s)		 *(uint32_t *)((uint8_t *)i82489 + (s))
#define	WRITE(s, d)	(*(uint32_t *)((uint8_t *)i82489 + (d)) = (s))

#include <machine/intr.h>
#include <machine/param.h>
#include <machine/pmap.h>

#include <sys/systm.h>

#include <x86/dev/i82489.h>

#include <vm/vm.h>

static int routes[] = {
	[IPIT] = I82489_LTR,
	[ISPU] = I82489_SVR
};

static volatile uint32_t *i82489;

void
i82489_map(vaddr_t va)
{
	if (!(i82489 = kpmenter(KPADDR(va))))
		panic("i82489: failed to map %x", va);
}

void
i82489c_ack(void)
{
	if (i82489)
		WRITE(I82489_EOI_SIG, I82489_EOI);
}

void
i82489c_init(void)
{
	i82489_map(KVADDR(I82489_BASE));

	if (i82489)
		WRITE(I82489_SVR_ENABLE | ISPU, I82489_SVR);
}

void
i82489c_mask(int v)
{
	if (i82489)
		WRITE(READ(routes[v]) | I82489_LINT_MASK, routes[v]);
}

void
i82489c_unmask(int v)
{
	if (i82489)
		WRITE(READ(routes[v]) & ~I82489_LINT_MASK, routes[v]);
}

void
i82489t_init(void)
{
	i82489_map(KVADDR(I82489_BASE));
	i82489t_nshot(0xffff);
}

void
i82489t_oshot(int c)
{
	if (!i82489)
		return;

	WRITE(I82489_LTDCR_BY1, I82489_LTDCR);
	WRITE(I82489_LINT_ONESHOT | IPIT, I82489_LTR);
	WRITE(c, I82489_LTICR);
}

void
i82489t_nshot(int c)
{
	if (!i82489)
		return;

	WRITE(I82489_LTDCR_BY1, I82489_LTDCR);
	WRITE(I82489_LINT_PERIODIC | IPIT, I82489_LTR);
	WRITE(c, I82489_LTICR);
}
