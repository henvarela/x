#include <machine/param.h>
#include <machine/systm.h>

#include <sys/alloc.h>
#include <sys/proc.h>
#include <sys/systm.h>

static pid_t	pid;

pid_t
fork(void)
{
	struct proc *c;

	/* This is a mistake. */
	c = malloc(sizeof(*c));
	if (_fork(pc, c))
		return (-1);

	/*
 	 * It would be nicer to setup the child's context so that it returns
	 * straight to work instead of right here.
	 */
	if (pc == c)
		return (0);

	c->pid	= ++pid;
	c->name	= pc->name;
	LIST_INSERT_AFTER(pc, c, next);

	return (c->pid);
}
