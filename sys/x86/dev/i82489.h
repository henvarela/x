#ifndef	X86_DEV_I82489_H
#define	X86_DEV_I82489_H

#define	I82489_BASE	0xfee00000
#define	I82489_ID	0x20
#define	I82489_VER	0x30
#define	I82489_TPR	0x80
#define	I82489_APR	0x90
#define	I82489_PPR	0xa0
#define	I82489_EOI	0xb0
#	define	I82489_EOI_SIG	0
#define	I82489_RRR	0xc0
#define	I82489_LDR	0xd0
#define	I82489_DFR	0xe0
#define	I82489_SVR	0xf0
#	define	I82489_SVR_ENABLE	0x100
#	define	I82489_SVR_FOCUS	0x200
#	define	I82489_SVR_BCAST	0x1000
#define	I82489_ISR	0x100
#define	I82489_TMR	0x180
#define	I82489_IRR	0x200
#define	I82489_ESR	0x280
#define	I82489_LCMCI	0x2f0
#define	I82489_ICR	0x300
#define	I82489_LTR	0x320
#define	I82489_LSR	0x330
#define	I82489_LPMCR	0x340
#define	I82489_LINT	0x350
#	define	I82489_LINT_FIX		0
#	define	I82489_LINT_SMI		0x200
#	define	I82489_LINT_NMI		0x400
#	define	I82489_LINT_INT		0x500
#	define	I82489_LINT_EXT		0x700
#	define	I82489_LINT_PEND	0x1000
#	define	I82489_LINT_POLAR	0x2000
#	define	I82489_LINT_EDGE	0
#	define	I82489_LINT_LEVEL	0x8000
#	define	I82489_LINT_MASK	0x10000
#	define	I82489_LINT_ONESHOT	0
#	define	I82489_LINT_PERIODIC	0x20000
#define	I82489_LERR	0x370
#define	I82489_LTICR	0x380
#define	I82489_LTCCR	0x390
#define	I82489_LTDCR	0x3e0
#	define	I82489_LTDCR_BY1	0xb
#	define	I82489_LTDCR_BY2	0
#	define	I82489_LTDCR_BY4	1
#	define	I82489_LTDCR_BY8	2
#	define	I82489_LTDCR_BY16	3
#	define	I82489_LTDCR_BY32	8
#	define	I82489_LTDCR_BY64	9
#	define	I82489_LTDCR_BY128	0xa

#ifndef	__ASSEMBLER__

#include <sys/types.h>

void	i82489_map(vaddr_t a);

void	i82489c_ack(void);
void	i82489c_init(void);
void	i82489c_mask(int v);
void	i82489c_unmask(int v);

void	i82489t_init(void);
void	i82489t_oshot(int c);
void	i82489t_nshot(int c);

#endif

#endif
