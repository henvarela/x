#include <dev/cons.h>

#include <sys/stdarg.h>
#include <sys/systm.h>
#include <sys/types.h>

static void
xtoa(size_t x, uint8_t *b)
{
	for (int s = 60; s >= 0; s -= 4)
		*b++ = (x >> s & 0xf) > 9 ?
		    (x >> s & 0xf) + '0' + 7 :
		    (x >> s & 0xf) + '0';
}

void
vprintf(const char *fmt, va_list ap)
{
	char c, *s;
	size_t x;
	uint8_t buf[17] = { [16] = NULL };

	while (*fmt) {
		switch (*fmt) {
		case '%':
			fmt++;

			if (*fmt == 'c') {
				c = va_arg(ap, int);
				buf[0] = c, buf[1] = NULL;
				printf((void *)buf);

			} else if (*fmt == 's') {
				s = va_arg(ap, char *);
				printf(s);

			} else if (*fmt == 'x') {
				x = va_arg(ap, size_t);
				xtoa(x, buf);
				printf("0x%s", buf);
			}

			fmt++;
			break;

		default: cc->putc(*fmt++);
		}
	}
}

void
printf(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}
