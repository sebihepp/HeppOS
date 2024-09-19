
OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_kstart)

PHDRS
{
	text		PT_LOAD;
	rodata		PT_LOAD;
	data		PT_LOAD;
	dynamic		PT_DYNAMIC;
}

SECTIONS
{


	. = 0xFFFFFFFF80000000;
	
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
		*(.text.*)
	} :text
	
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
		*(.rodata.*)
	} :rodata

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
		*(.data.*)
		
		KEEP(*(.requests_start_marker))
		KEEP(*(.requests))
		KEEP(*(.requests_end_marker))
		
	} :data

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
		*(.bss.*)
	} :data

	/DISCARD/ :
	{
		*(.eh_frame*)
		*(.note .note.*)
	}
	
}

