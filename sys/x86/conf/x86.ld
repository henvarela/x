#include <machine/param.h>

OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH("i386:x86-64")
ENTRY(KBEG)

MEMORY
{
	PHYS	: org = KPHYS,		l = PSIZE2
	VIRT	: org = KVIRT + KPHYS,	l = PSIZE2
}

SECTIONS
{
	.	= ORIGIN(VIRT);
	KBEG	= .;

	.text	ALIGN(PSIZE) : ALIGN(PSIZE) { *(.*text*)       } >VIRT AT >PHYS
	.rodata	ALIGN(PSIZE) : ALIGN(PSIZE) { *(.*rodata*)     } >VIRT AT >PHYS
	.data	ALIGN(PSIZE) : ALIGN(PSIZE) { *(.*data* .*got*)} >VIRT AT >PHYS
	.bss	ALIGN(PSIZE) : ALIGN(PSIZE) { *(.*bss* COMMON) } >VIRT AT >PHYS

	KEND	= ALIGN(PSIZE);

	.shstrtab : { *(.shstrtab) }
	/DISCARD/ : { * }
}
