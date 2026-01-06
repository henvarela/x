#define	PHYSALLOC()							      \
	KPADDR(palloc())
#define	PHYSFREE(p)							      \
	pfree((void *)KVADDR(p));

#include <machine/param.h>
#include <machine/pmap.h>

#include <sys/alloc.h>

/*
 * Map machine-independent protection flags to machine-dependent ones.
 */
static const uint16_t prots[] = {
	[VMPROT_NONE] = 0,

	/* permission */
	[VMPROT_R]		= 0,
	[VMPROT_W]		= PRW,
	[VMPROT_X]		= 0,
	[VMPROT_R | VMPROT_W]	= PRW,
	[VMPROT_R | VMPROT_X]	= 0,
	[VMPROT_W | VMPROT_X]	= PRW,
	[VMPROT_ALL]		= PRW,

	/* ownership */
	[VMPROT_K] = 0,
	[VMPROT_U] = PU,

	/* permission | ownership */
	[VMPROT_R | VMPROT_K]			= 0,
	[VMPROT_W | VMPROT_K]			= PRW,
	[VMPROT_X | VMPROT_K]			= 0,
	[VMPROT_R | VMPROT_W | VMPROT_K]	= PRW,
	[VMPROT_R | VMPROT_X | VMPROT_K]	= 0,
	[VMPROT_W | VMPROT_X | VMPROT_K]	= PRW,
	[VMPROT_ALL | VMPROT_K]			= PRW,

	/* permission | ownership */
	[VMPROT_R | VMPROT_U]			= PU,
	[VMPROT_W | VMPROT_U]			= PRW | PU,
	[VMPROT_X | VMPROT_U]			= PU,
	[VMPROT_R | VMPROT_W | VMPROT_U]	= PRW | PU,
	[VMPROT_R | VMPROT_X | VMPROT_U]	= PU,
	[VMPROT_W | VMPROT_X | VMPROT_U]	= PRW | PU,
	[VMPROT_ALL | VMPROT_U]			= PRW | PU,
};

/*
 * Find a way to the PTE of a given VA from the 4th level of a map.
 */
static pte_t *
pte4(pmap_t *pm, vaddr_t va, vmprot_t _prot)
{
	pte_t *pt4, pte;
	int prot;

	if (!pm)
		return (NULL);
	if (!pm->pt)
		pm->pt = (void *)palloc();

	pt4  = (void *)pm->pt;
	pte  = pt4[PTI4(va)];
	prot = prots[_prot] | PV;

	if (!pte)
		pte = pt4[PTI4(va)] = PHYSALLOC() | prot;

	return ((void *)KVADDR(pte));
}

/*
 * Find a way to the PTE of a given VA from the 3rd level of a map.
 */
static pte_t *
pte3(pmap_t *pm, vaddr_t va, vmprot_t _prot)
{
	pte_t *pt3, pte;
	int prot;

	pt3 = (void *)((size_t)pte4(pm, va, _prot) & ~PMASK);

	if (!pt3)
		return (NULL);

	pte  = pt3[PTI3(va)];
	prot = prots[_prot] | PV;

	if (!pte)
		pte = pt3[PTI3(va)] = PHYSALLOC() | prot;

	return ((void *)KVADDR(pte));
}

/*
 * Find a way to the PTE of a given VA from the 2nd level of a map.
 */
static pte_t *
pte2(pmap_t *pm, vaddr_t va, vmprot_t _prot)
{
	pte_t *pt2, pte;
	int prot;

	pt2 = (void *)pte3(pm, va, _prot);

	if (!pt2)
		return (NULL);

	pt2 = (void *)((size_t)pt2 & ~PMASK);
	pte = pt2[PTI2(va)];
	prot= prots[_prot] | PV;

	if (!pte)
		pte = pt2[PTI2(va)] = PHYSALLOC() | prot;

	return ((void *)KVADDR(pte));
}

/*
 * Get the PTE of a given VA from the 1st level of a map.
 */
static pte_t *
pte1(pmap_t *pm, vaddr_t va, vmprot_t prot)
{
	pte_t *pt1, pte;

	pt1 = (void *)((size_t)pte2(pm, va, prot) & ~PMASK);

	if (!pt1)
		return (NULL);

	pte = (pte_t)&pt1[PTI1(va)];

	return ((void *)pte);
}

/*
 * Check for an existing virtual to physical mapping.
 */
static int
checkmap(pmap_t *pm, vaddr_t va, paddr_t pa)
{
	if (!pm)
		return (0);

	LIST_HEAD(, map) b;
	struct map *p;

	LIST_FIRST(&b) = &pm->map;

	LIST_FOREACH(p, &b, next) {
		if (va >= p->va && va < p->va + p->len &&
		    pa >= p->pa && pa < p->pa + p->len)
			return (1);
	}

	return (0);
}

/*
 * Register a virtual to physical mapping pair.
 */
static void
registmap(pmap_t *pm, vaddr_t va, paddr_t pa, size_t len)
{
	struct map *p;

	if (!pm || checkmap(pm, va, pa))
		return;

	p = malloc(sizeof(*p));
	*p = (struct map){ .va = va, .pa = pa, .len = len };

	LIST_INSERT_AFTER(&pm->map, p, next);
}

/*
 * Deregister a virtual to physical mapping pair.
 */
static void
deregistmap(pmap_t *pm, vaddr_t va)
{
	if (!pm)
		return;

	LIST_HEAD(, map) b;
	struct map *p;

	LIST_FIRST(&b) = &pm->map;

	LIST_FOREACH(p, &b, next) {
		if (va >= p->va && va < p->va + p->len) {
			LIST_REMOVE(p, next);
			mfree(p);
		}
	}
}

void *
kpmenter(paddr_t pa)
{
	return (pmenter(&kpm, KVADDR(pa), pa, VMPROT_K | VMPROT_ALL, 0));
}

void
kpmremove(vaddr_t va)
{
	pmremove(&kpm, va);
}

pmap_t
pmcopy(const pmap_t *src)
{
	pmap_t dst;
	LIST_HEAD(, map) b;
	struct map *p;

	if (!src)
		return ((pmap_t){ .pt = NULL });

	dst = (pmap_t){ .pt = NULL };
	LIST_FIRST(&b) = (void *)&src->map;

	LIST_FOREACH(p, &b, next)
		pmenter(&dst, p->va, p->pa, VMPROT_ALL, p->len > PSIZE ? PS:0);

	return (dst);
}

void *
pmenter(pmap_t *pm, vaddr_t _va, paddr_t _pa, vmprot_t _prot, int _flags)
{
	vaddr_t	 va;
	paddr_t	 pa;
	int	 prot;
	int	 flags;
	pte_t	*pte;

	if (checkmap(pm, _va, _pa))
		return ((void *)_va);

	va	= _flags & PS ? _va & ~PMASK2 : _va & ~PMASK;
	pa	= _flags & PS ? _pa & ~PMASK2 : _pa & ~PMASK;
	prot	= prots[_prot];
	flags	= _flags | PV;
	pte	= (void *)
	         ((size_t)(flags & PS ? pte3(pm, va, _prot) :
	          pte1(pm, va, _prot)) & ~PMASK);

	if (!pte)
		return (NULL);

	pte += flags & PS ? PTI2(va) : PTI1(va);
	*pte = pa | prot | flags;

	registmap(pm, va, pa, flags & PS ? PSIZE2 : PSIZE);

	return ((void *)_va);
}

void
pmremove(pmap_t *pm, vaddr_t va)
{
	pte_t *pte;

	pte = pte2(pm, va, VMPROT_ALL);

	if ((size_t)pte & PS)
		pte = (pte_t *)
		     ((size_t)pte3(pm, va, VMPROT_ALL) & ~PMASK) + PTI2(va);
	else
		pte = pte1(pm, va, VMPROT_ALL);

	if (!pte)
		return;

	*pte = 0;
	PHYSFREE(pte);

	deregistmap(pm, va);
}
