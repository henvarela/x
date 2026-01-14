SUBDIR=	sys

all build:
	@for d in ${SUBDIR}; do make -C $$d $@; done

clean:
	@for d in ${SUBDIR}; do make -C $$d $@; done

.PHONY:	all build clean
