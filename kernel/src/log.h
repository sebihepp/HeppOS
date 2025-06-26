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

#ifndef HEADER_LOG
#define HEADER_LOG

#include <stddef.h>
#include <stdint.h>

#include <retval.h>
#include <serial.h>


class CLog {
private:

	static CSerial mSerial;
	static bool mInitialized;
	
	CLog() = delete;
	~CLog() = delete;

public:
	
	static ReturnValue_t Init(void) __attribute__ (( nothrow ));
	
	static void Print(const char *pString) __attribute__ (( nothrow ));
	static void PrintF(const char *pString, ...) __attribute__ (( nothrow ));
	
};

#endif
