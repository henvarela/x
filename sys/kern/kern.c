#include <sys/proc.h>
#include <sys/systm.h>
#include <sys/types.h>

void
kern(void)
{
	pid_t pid;

	if (!(pid = fork())) {
		pc->name = "init";
		init();
	} else if (pid == -1)
		panic("failed to fork %s", pc->name);
}
