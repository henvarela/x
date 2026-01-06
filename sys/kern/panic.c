#include <machine/systm.h>

#include <sys/systm.h>

void
panic(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);

	_panic();
}
