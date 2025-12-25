#ifndef	MACHINE_PMAP_H
#define	MACHINE_PMAP_H

#define	PV	(1ULL << 0)	/* level 1..4 */
#define	PRW	(1ULL << 1)	/* level 1..4 */
#define	PU	(1ULL << 2)	/* level 1..4 */
#define	PWT	(1ULL << 3)	/* level 1..4 */
#define	PCD	(1ULL << 4)	/* level 1..4 */
#define	PA	(1ULL << 5)	/* level 1..4 */
#define	PD	(1ULL << 6)	/* level 1    */
#define	PS	(1ULL << 7)	/* level 2..3 */
#define	PAT	(1ULL << 7)	/* level 1    */
#define	PG	(1ULL << 8)	/* level 1    */
#define	PAVL	(7ULL << 9)	/* level 1..4 */

#include <sys/queue.h>
#include <sys/types.h>

#include <vm/vm.h>

typedef struct pmap {
	pte_t *pt;

	struct map {
		LIST_ENTRY(map) next;
		vaddr_t	va;
		paddr_t	pa;
		size_t	len;
	} map;
} pmap_t;

extern pmap_t kpm;

void	*kpmenter(paddr_t pa);
void	 kpmremove(vaddr_t va);

pmap_t	 pmcopy(const pmap_t *src);
void	*pmenter(pmap_t *pm, vaddr_t va, paddr_t pa, vmprot_t prot, int flags);
void	 pmremove(pmap_t *pm, vaddr_t va);

#endif
