#ifndef	DEV_CONS_H
#define	DEV_CONS_H

#include <sys/types.h>

struct cons {
	void	(*init)(void);
	void	(*map)(vaddr_t);
	void	(*putc)(int);

	const char *hw;
};

extern struct cons *cc;

#endif
