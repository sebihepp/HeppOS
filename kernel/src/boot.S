
    # Copyright (C) 2025  Sebastian Hepp aka sebihepp

    # This program is free software: you can redistribute it and/or modify
    # it under the terms of the GNU General Public License as published by
    # the Free Software Foundation, either version 3 of the License, or
    # (at your option) any later version.

    # This program is distributed in the hope that it will be useful,
    # but WITHOUT ANY WARRANTY; without even the implied warranty of
    # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    # GNU General Public License for more details.

    # You should have received a copy of the GNU General Public License
    # along with this program.  If not, see <https://www.gnu.org/licenses/>.


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

