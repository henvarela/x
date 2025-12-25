#ifndef	SYS_ALLOC_H
#define	SYS_ALLOC_H

#include <sys/types.h>

void	*malloc(size_t len);
void	 mfree(void *ptr);

void	*palloc(void);
void	 pfree(void *ptr);

#endif
