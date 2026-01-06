#define	LEN(a)	(sizeof(a) / sizeof(*a))

#include <machine/param.h>

#include <sys/alloc.h>
#include <sys/systm.h>

/*
 * Manage the page frames in the kernel image (usually 2M) for kernel
 * initialization purposes only. Using this for anything else is a mistake.
 * This should not be a replacement or the wrappee of pmalloc/vmalloc/malloc.
 */
struct pdesc {
	uint8_t	*beg;
	int	 occ;
};

static struct pdesc ppool[64] = {
	[0] = {
		.beg = (void *)KEND,
		.occ = 1
	}
};

void *
palloc(void)
{
	for (struct pdesc *p = ppool; p < ppool + LEN(ppool); p++) {
		if (p->occ)
			continue;

		*p = (struct pdesc){ p[-1].beg + PSIZE, 1 };

		return (p->beg);
	}

	panic("palloc: out of memory");

	return (NULL); /* Make the compiler happy. */
}

void
pfree(void *ptr)
{
	struct pdesc *p;

	if ((size_t)ptr & ~PMASK)
		return;

	for (p = ppool; p < ppool + LEN(ppool) && p->beg != ptr; p++)
		/* nothing */;

	p->occ = 0;
}

void *
malloc(size_t len)
{
	/* TODO */
	return ((void)len, palloc());
}

void
mfree(void *p)
{
	/* TODO */
	pfree(p);
}
