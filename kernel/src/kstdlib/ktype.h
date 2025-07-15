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

#pragma once

#include <stddef.h>
#include <stdint.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

bool kisalnum(char pChar) __attribute__ (( nothrow , const ));
bool kisalpha(char pChar) __attribute__ (( nothrow , const ));
bool kislower(char pChar) __attribute__ (( nothrow , const ));
bool kisupper(char pChar) __attribute__ (( nothrow , const ));
bool kisdigit(char pChar) __attribute__ (( nothrow , const ));
bool kisxdigit(char pChar) __attribute__ (( nothrow , const ));
bool kiscntrl(char pChar) __attribute__ (( nothrow , const ));
bool kisgraph(char pChar) __attribute__ (( nothrow , const ));
bool kisspace(char pChar) __attribute__ (( nothrow , const ));
bool kisblank(char pChar) __attribute__ (( nothrow , const ));
bool kisprint(char pChar) __attribute__ (( nothrow , const ));
bool kispunct(char pChar) __attribute__ (( nothrow , const ));
char ktolower(char pChar) __attribute__ (( nothrow , const ));
char ktoupper(char pChar) __attribute__ (( nothrow , const ));

#ifdef __cplusplus
}
#endif
