#ifndef	SYS_SYSTM_H
#define	SYS_SYSTM_H

#include <sys/types.h>

pid_t	fork(void);
void	init(void);
void	printf(const char *fmt, ...);

#endif
