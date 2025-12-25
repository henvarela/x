#ifndef	VM_VM_H
#define	VM_VM_H

/*
 * permission:
 */
#define	VMPROT_R	(1ULL << 0)
#define	VMPROT_W	(1ULL << 1)
#define	VMPROT_X	(1ULL << 2)
#define	VMPROT_NONE	 0
#define	VMPROT_ALL	(VMPROT_R | VMPROT_W | VMPROT_X)

/*
 * ownership:
 */
#define	VMPROT_K	(1ULL << 3)
#define	VMPROT_U	(1ULL << 4)

#ifndef	__ASSEMBLER__
typedef	int	vmprot_t;
#endif

#endif
