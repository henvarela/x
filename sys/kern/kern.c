#include <sys/proc.h>
#include <sys/systm.h>

void
kern(void)
{
	if (!fork()) {
		pc->name = "init";
		init();
	}
}
