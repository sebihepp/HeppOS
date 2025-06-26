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


#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint64_t PREINIT_ARRAY_START;
extern uint64_t PREINIT_ARRAY_END;

extern uint64_t INIT_ARRAY_START;
extern uint64_t INIT_ARRAY_END;

typedef void (*Constructor_t)(void);

void _init(void) __attribute(( nothrow ));

void _init(void) {
	
	uint64_t *_Start = 0;
	uint64_t *_End = 0;
	
	_Start = reinterpret_cast<uint64_t*>(&PREINIT_ARRAY_START);
	_End = reinterpret_cast<uint64_t*>(&PREINIT_ARRAY_END);
	for (uint64_t *i = _Start; i < _End; i++) {
		Constructor_t _Func = reinterpret_cast<Constructor_t>(*i);
		(*_Func)();
	}
	
	_Start = reinterpret_cast<uint64_t*>(&INIT_ARRAY_START);
	_End = reinterpret_cast<uint64_t*>(&INIT_ARRAY_END);
	for (uint64_t *i = _Start; i < _End; i++) {
		Constructor_t _Func = reinterpret_cast<Constructor_t>(*i);
		(*_Func)();
	}
	
}


#ifdef __cplusplus
}
#endif

