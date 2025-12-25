#ifndef	MACHINE_LIB_H
#define	MACHINE_LIB_H

#include <sys/types.h>

int	 _memcmp(const void *dst, const void *src, size_t len);
void	*_memcpy(void *dst, const void *src, size_t len);
void	*_memmove(void *dst, const void *src, size_t len);
void	*_memset(void *dst, int chr, size_t len);
size_t	 _strlen(const void *str);

#define	memcmp	_memcmp
#define	memcpy	_memcpy
#define	memmove	_memmove
#define	memset	_memset
#define	strlen	_strlen

#endif
