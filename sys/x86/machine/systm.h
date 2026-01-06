#ifndef	MACHINE_SYSTM_H
#define	MACHINE_SYSTM_H

#include <sys/proc.h>

int	_fork(const struct proc *p, struct proc *c);
void	_panic(void);

#endif
