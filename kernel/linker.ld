
OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_kstart)

PHDRS
{
	text		PT_LOAD;
	rodata		PT_LOAD;
	init_array	PT_LOAD;
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
		PREINIT_ARRAY_START = .;
		KEEP(*(.preinit_array*)) 
		PREINIT_ARRAY_END = .;

		INIT_ARRAY_START = .;
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))	
		PROVIDE(INIT_ARRAY_END = .);
		
		FINI_ARRAY_START = .;
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))	
		PROVIDE(FINI_ARRAY_END = .);
		
		*(.rodata)
		*(.rodata.*)
		*(.eh_frame)
		*(.eh_frame.*)
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
		*(.note .note.*)
		
		*(.fini_array)
		*(.fini_array.*)
	}
	
}


