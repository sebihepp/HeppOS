/* 
    Copyright (C) 2025  Sebastian Hepp aka sebihepp

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef HEADER_PMM
#define HEADER_PMM

#include <retval.h>
#include <memory/paging.h>
#include <liminestub.h>

class CPMM {
private:
	
	CPMM() = delete;
	~CPMM() = delete;
public:

	static ReturnValue_t PreInit(void) __attribute__ (( nothrow ));
	static ReturnValue_t Init(void) __attribute__ (( nothrow ));
	
};

#endif
