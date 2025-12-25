#define	SYS_LIB_C

#include <machine/param.h>

#include <sys/lib.h>

int
memcmp(const void *_dst, const void *_src, size_t len)
{
	const byte_t *dst, *src;

	dst = _dst;
	src = _src;

	if (dst == src || len == 0)
		return (0);

	/* Test the alignability of both of the operands. */
	if (((size_t)dst ^ (size_t)src) & WMASK)
		goto tail;

	/* Align the operands. */
	while (len > 0 && (size_t)dst & WMASK) {
		if (*dst != *src)
			return (*dst - *src);
		dst++, src++, len--;
	}

	/* Compare word by word. */
	while (len >= WSIZE) {
		if (*(const word_t *)dst != *(const word_t *)src)
			break;
		dst += WSIZE, src += WSIZE, len -= WSIZE;
	}

tail:	/* Compare byte by byte. */
	while (len > 0) {
		if (*dst != *src)
			return (*dst - *src);
		dst++, src++, len--;
	}

	return (0);
}

void *
memcpy(void *_dst, const void *_src, size_t len)
{
	byte_t *dst;
	const byte_t *src;

	dst = _dst;
	src = _src;

	if (src == dst || len == 0)
		return (dst);

	/* Test the alignability of both of the operands. */
	if (((size_t)dst ^ (size_t)src) & WMASK)
		goto tail;

	/* Align the operands. */
	while (len > 0 && (size_t)dst & WMASK) {
		*dst = *src;
		dst++, src++, len--;
	}

	/* Copy word by word. */
	while (len >= WSIZE) {
		*(word_t *)dst = *(const word_t *)src;
		dst += WSIZE, src += WSIZE, len -= WSIZE;
	}

tail:	/* Copy byte by byte. */
	while (len--)
		*dst++ = *src++;

	return (_dst);
}

void *
memmove(void *_dst, const void *_src, size_t len)
{
	byte_t *dst;
	const byte_t *src;

	/* Memcpy() when defined. */
	if ((size_t)_src + len < (size_t)_dst)
		return (memcpy(_dst, _src, len));

	dst = (byte_t *)_dst + len - 1;
	src = (byte_t *)_src + len - 1;

	/* Test the alignability of both of the operands. */
	if (((size_t)dst ^ (size_t)src) & WMASK)
		goto tail;

	/* Align the operands. */
	while (len > 0 && (size_t)dst & WMASK) {
		*dst = *src;
		dst--, src--, len--;
	}

	/* Copy word by word. */
	while (len >= WSIZE) {
		*(word_t *)dst = *(const word_t *)src;
		dst -= WSIZE, src -= WSIZE, len -= WSIZE;
	}

tail:	/* Copy byte by byte. */
	while (len--)
		*dst-- = *src--;

	return (_dst);
}

void *
memset(void *_dst, int _chr, size_t len)
{
	byte_t *dst, chr;
	word_t wrd;

	dst = _dst;
	chr = _chr;

	/* Align the operand. */
	while (len > 0 && (size_t)dst & WMASK) {
		*dst = chr;
		dst++, len--;
	}

	/* Prepare the source operand for word-sized copies. */
	wrd = 0;
	for (size_t i = 0; i < WSIZE; i++)
		wrd |= (word_t)chr << (i * 8);

	/* Set word by word. */
	while (len >= WSIZE) {
		*(word_t *)dst = wrd;
		dst += WSIZE, len -= WSIZE;
	}

	/* Set byte by byte. */
	while (len--)
		*dst++ = chr;

	return (_dst);
}

size_t
strlen(const void *_str)
{
	const byte_t *str;

	str = _str;

	/* Align the operand. */
	while (*str && (size_t)str & WMASK)
		str++;

	/* Test for a null byte word by word. */
	while (!(*(const word_t *)str - 0x0101010101010101 &
	    ~*(const word_t *)str & 0x8080808080808080))
		str += WSIZE;

	/* Test byte by byte. */
	while (*str)
		str++;

	return ((size_t)str - (size_t)_str);
}
