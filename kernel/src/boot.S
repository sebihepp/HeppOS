

.global _kstart
.type _kstart, @function

.section ".text"
_kstart:
	
	#call _init
	call kmain
_error:
	cli
1:
	hlt
	jmp 1b
