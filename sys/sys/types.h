#ifndef	SYS_TYPES_H
#define	SYS_TYPES_H

#define	NULL			0

#include <machine/types.h>

typedef	_int8_t		int8_t;
typedef	_int16_t	int16_t;
typedef	_int32_t	int32_t;
typedef	_int64_t	int64_t;

typedef	_uint8_t	uint8_t;
typedef	_uint16_t	uint16_t;
typedef	_uint32_t	uint32_t;
typedef	_uint64_t	uint64_t;

typedef	_ssize_t	ssize_t;
typedef	_size_t		size_t;

typedef	_register_t	register_t;

typedef	_paddr_t	paddr_t;
typedef	_vaddr_t	vaddr_t;
typedef	_pte_t		pte_t;

typedef	_byte_t		byte_t;
typedef	_word_t		word_t;

typedef	int32_t		pid_t;

#endif
