#ifndef	DEV_PIT_H
#define	DEV_PIT_H

#include <sys/types.h>

struct pit {
	void	(*init)(void);
	void	(*map)(vaddr_t);
	void	(*oshot)(int);
	void	(*nshot)(int);

	const char *hw;
};

extern struct pit *pit;

#endif
