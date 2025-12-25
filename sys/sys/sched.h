#ifndef	SYS_SCHED_H
#define	SYS_SCHED_H

#include <sys/proc.h>
#include <sys/queue.h>

#define	SCHED()								      \
	(pc = LIST_NEXT(pc, next))

#endif
