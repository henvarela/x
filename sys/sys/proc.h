#ifndef	SYS_PROC_H
#define	SYS_PROC_H

#include <machine/proc.h>

#include <sys/queue.h>

struct proc {
	LIST_ENTRY(proc)	 next;
	struct _proc		 p;
	pid_t			 pid;
	const char		*name;
};

extern struct proc *pc;

#endif
