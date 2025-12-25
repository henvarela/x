#ifndef	MACHINE_PROC_H
#define	MACHINE_PROC_H

#include <machine/frame.h>
#include <machine/pmap.h>

struct _proc {
	struct frame	ctx;
	pmap_t		pm;
};

#endif
