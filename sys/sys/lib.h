#ifndef	SYS_LIB_H
#define	SYS_LIB_H

#ifndef	SYS_LIB_C
#include <machine/lib.h>
#endif

#include <sys/types.h>

int	 memcmp(const void *dst, const void *src, size_t len);
void	*memcpy(void *dst, const void *src, size_t len);
void	*memmove(void *dst, const void *src, size_t len);
void	*memset(void *dst, int chr, size_t len);
size_t	 strlen(const void *str);

#endif
