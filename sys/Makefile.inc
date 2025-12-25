#
# Usage:
#
# 	SYSDIR	= .
# 	include ${SYSDIR}/Makefile.inc
#
# 	build: ...
#		... # Prefer ALLSRCS over either $> or $^
# 	clean: ...
# 		...
#

MACHDIR=${SYSDIR}/${MACHINE}

INCS=-I${SYSDIR}/include -I${MACHDIR}/include

CFLAGS=-W -Wall -Werror -Wno-parentheses -pedantic -std=c99 -ffreestanding
CFLAGS+=${INCS}

#ALLSRCS=$> $^# old bmake and gmake interoperability
ALLSRCS=$^

all:	build
.PHONY:	all build clean

include ${SYSDIR}/../config.mk
include ${MACHDIR}/conf/Makefile.inc
