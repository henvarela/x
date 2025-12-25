#ifndef	X86_DEV_VGA_H
#define	X86_DEV_VGA_H

#include <sys/types.h>

void	vinit(void);
void	vmap(vaddr_t a);
void	vputc(int c);

#endif
