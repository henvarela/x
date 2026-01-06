#ifndef	SYS_SYSTM_H
#define	SYS_SYSTM_H

#include <sys/stdarg.h>
#include <sys/types.h>

pid_t	fork(void);
void	init(void);
void	printf(const char *fmt, ...);
void	vprintf(const char *fmt, va_list ap);

#endif
