OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_kstart)

SECTIONS
{
    . = SIZEOF_HEADERS;

/**********************************************/
	/* TEXT and RODATA */

	.text :
	{
		*(.text)
		*(.text.*)
	} /*:ktext*/
	
	.rodata :
	{
		*(.rodata)
		*(.rodata.*)
	} /*:krodata*/

/**********************************************/
	/* DATA and BSS */

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
		*(.data.*)
		
		KEEP(*(.requests_start_marker))
		KEEP(*(.requests))
		KEEP(*(.requests_end_marker))
		
	} /*:kdata*/
        .preinit_array :
        {
            PREINIT_ARRAY_START = .;
            *(.preinit_array.*)
            *(.preinit_array)
            PREINIT_ARRAY_END = .;
        }

        .init_array :
        {
		INIT_ARRAY_START = .;
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))	
		PROVIDE(INIT_ARRAY_END = .);
        }
        
        .fini_array :
        {
		FINI_ARRAY_START = .;
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))	
		PROVIDE(FINI_ARRAY_END = .);
        }



	.bss :
	{
		*(COMMON)
		*(.bss)
		*(.bss.*)
	} /*:kdata*/

	/DISCARD/ :
	{
		*(.eh_frame)
		*(.eh_frame.*)
		*(.comment)
		*(.note .note.*)
	}

}


