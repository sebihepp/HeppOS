

.global _kstart
.type _kstart, @function

.section ".text"
_kstart:
	
	# Call global constructors
	call _init
	
	# Call kmain
	call kmain
	
	# Not necessary to call destructors, as the OS is dying if it reaches this point
	call _fini
	
_error:
	cli
1:
	hlt
	jmp 1b

