#ifndef	DEV_PIC_H
#define	DEV_PIC_H

#include <sys/types.h>

struct pic {
	void	(*ack)(void);
	void	(*init)(void);
	void	(*map)(vaddr_t);
	void	(*mask)(int);
	void	(*unmask)(int);

	const char *hw;
};

extern struct pic *pic;

#endif
