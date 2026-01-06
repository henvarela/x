#define	IOBASE	 0x3d4
#define	COLS	 80
#define	ROWS	 25
#define	SIZE	(ROWS * COLS)
#define	BEG	(uint16_t *)KVADDR(0xb8000)
#define	END	(BEG + SIZE)
#define	WHITE_ON_BLACK 0xf00

#define	PUTCHAR(c) do {							      \
	if (vga && c)							      \
		*vga++ = WHITE_ON_BLACK | c;				      \
	if (vga) {							      \
		OUTB(14, IOBASE);					      \
		OUTB((size_t)(vga - BEG) >> 8, IOBASE+1);		      \
		OUTB(15, IOBASE);					      \
		OUTB((size_t)(vga - BEG), IOBASE+1);			      \
	}								      \
} while (0)

#include <machine/asm.h>
#include <machine/param.h>
#include <machine/pmap.h>

#include <sys/lib.h>
#include <sys/systm.h>
#include <sys/types.h>

#include <x86/dev/vga.h>

static uint16_t *vga;

static void
clear(void)
{
	if (!vga)
		return;

	vga = END;

	while (vga > BEG)
		*--vga = WHITE_ON_BLACK | ' ';

	vputc(NULL);
}

void
vmap(vaddr_t va)
{
	if (!(vga = kpmenter(KPADDR(va))))
		panic("VGA: failed to map %x", va);
}

void
vinit(void)
{
	if (vga)
		return;

	vmap((vaddr_t)BEG);
	clear();
}

void
vputc(int c)
{
	if (vga >= END) {
		memcpy(BEG, BEG + COLS, 2 * SIZE);
		vga = END - COLS;
		PUTCHAR(NULL);
	}

	switch (c) {
	case '\n':
		vga = (void *)
		    ((size_t)BEG + ((size_t)vga & 0xfff) / (COLS * 2) *
		     (COLS * 2) + (COLS * 2));
		PUTCHAR(NULL);
		break;

	case '\t':	PUTCHAR(' ');	break;
	default:	PUTCHAR(c);
	}
}
