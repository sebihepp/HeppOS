
OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_kstart)

PHDRS
{
	ktext		PT_LOAD;
	krodata		PT_LOAD;
	kdata		PT_LOAD;
	kdynamic	PT_DYNAMIC;
	kinterp		PT_INTERP;
	kgot		PT_LOAD;
}

SECTIONS
{

	/* Use this only with no-pie*/
	/*. = 0xFFFFFFFF80000000;*/

/**********************************************/
	/* TEXT and RODATA */

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
		*(.text.*)
	} :ktext
	
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
	} :krodata


/**********************************************/
	/* DATA and BSS */

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
		*(.data.*)
		
		KEEP(*(.requests_start_marker))
		KEEP(*(.requests))
		KEEP(*(.requests_end_marker))
		
	} :kdata

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
		*(.bss.*)
	} :kdata


/**********************************************/
	/* PT_DYNAMIC */

	.dynamic BLOCK(4K) : ALIGN(4K) 
	{
		*(.dynamic)
	} :kdynamic

/**********************************************/
	/* PT_INTERP */

	.interp BLOCK(4K) : ALIGN(4K) 
	{
		*(.interp)
	} :kinterp

	.dynsym BLOCK(4K) : ALIGN(4K) 
	{
		*(.dynsym)
	} :kinterp

	.dynstr BLOCK(4K) : ALIGN(4K) 
	{
		*(.dynstr)
	} :kinterp

	.rela.dyn BLOCK(4K) : ALIGN(4K) 
	{
		*(.rela.dyn)
	} :kinterp
	
	.rela.plt BLOCK(4K) : ALIGN(4K) 
	{
		*(.rela.plt)
	} :kinterp


/**********************************************/
	/* GOT */

	.got BLOCK(4K) : ALIGN(4K) 
	{
		*(.got)
	} :kgot

	.got.plt BLOCK(4K) : ALIGN(4K) 
	{
		*(.got.plt)
	} :kgot

/**********************************************/
	/* DISCARD */

	/DISCARD/ :
	{
		*(.eh_frame)
		*(.eh_frame.*)
		*(.comment)
		*(.note .note.*)
	}

}


