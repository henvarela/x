#ifndef	MACHINE_PARAM_H
#define	MACHINE_PARAM_H

#define	WSHIFT		 3
#define	WSIZE		 8
#define	WMASK		(WSIZE - 1)

#define	PSHIFT1		 12
#define	PSHIFT2		 21
#define	PSHIFT3		 30
#define	PSHIFT4		 39
#define	PSIZE		(1 << PSHIFT1)
#define	PSIZE2		(1 << PSHIFT2)
#define	PMASK		(PSIZE  - 1)
#define	PMASK2		(PSIZE2 - 1)

#define	KLBA		 1
#define	KLEN		(KISIZE >> 9)	/* in blocks */
#define	KSIZE		(KEND - KBEG)
#define	KISIZE		(PSIZE2 - KPHYS)
#define	KPHYS		 0x110000
#define	KVIRT		 0xffffffff00000000

#ifdef	__ASSEMBLER__
	#define	KPADDR(va)	((va) - KVIRT)
	#define	KVADDR(pa)	((pa) + KVIRT)

	#define	PTI1(va)	((va) >> PSHIFT1 & 0x1ff)
	#define	PTI2(va)	((va) >> PSHIFT2 & 0x1ff)
	#define	PTI3(va)	((va) >> PSHIFT3 & 0x1ff)
	#define	PTI4(va)	((va) >> PSHIFT4 & 0x1ff)
#elif	__STDC__
	#include <sys/types.h>

	extern uint8_t KBEG[];
	extern uint8_t KEND[];

	#define	KBEG		((vaddr_t)KBEG)
	#define	KEND		((vaddr_t)KEND)

	#define	KPADDR(va)	((vaddr_t)(va) - KVIRT)
	#define	KVADDR(pa)	((paddr_t)(pa) + KVIRT)

	#define	PTI1(va)	((size_t)(va) >> PSHIFT1 & 0x1ff)
	#define	PTI2(va)	((size_t)(va) >> PSHIFT2 & 0x1ff)
	#define	PTI3(va)	((size_t)(va) >> PSHIFT3 & 0x1ff)
	#define	PTI4(va)	((size_t)(va) >> PSHIFT4 & 0x1ff)
#endif

#endif
