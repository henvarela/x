OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH("i386")
ENTRY("0x7c00")

SECTIONS
{
	.	= 0x7c00;

	.text	: { *(.text*) }

	.shstrtab : { *(.shstrtab) }
	/DISCARD/ : { * }
}
