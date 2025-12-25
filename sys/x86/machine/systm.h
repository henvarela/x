#ifndef	MACHINE_SYSTM_H
#define	MACHINE_SYSTM_H

#include <sys/proc.h>

void _fork(const struct proc *p, struct proc *c);

#endif
