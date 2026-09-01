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

#include <ktype.h>


bool kisalnum(char character) {
	if (kisalpha(character) || kisdigit(character)) 
		return true;
	return false;
}

bool kisalpha(char character) {
	if ((character >= 0x41) && (character <= 0x5A))
		return true;
	if ((character >= 0x61) && (character <= 0x7A))
		return true;
	return false;
}

bool kislower(char character) {
	if ((character >= 0x61) && (character <= 0x7A))
		return true;
	return false;	
}

bool kisupper(char character) {
	if ((character >= 0x41) && (character <= 0x5A))
		return true;
	return false;
}

bool kisdigit(char character) {
	if ((character >= 0x30) && (character <= 0x39))
		return true;
	return false;
}

bool kisxdigit(char character) {
	if ((character >= 0x30) && (character <= 0x39))
		return true;
	if ((character >= 0x41) && (character <= 0x46))
		return true;
	if ((character >= 0x61) && (character <= 0x66))
		return true;
	return false;
}

bool kiscntrl(char character) {
	if ((character >= 0x00) && (character <= 0x1F))
		return true;
	if (character == 0x7F)
		return true;
	return false;	
}

bool kisgraph(char character) {
	if ((character >= 0x21) && (character <= 0x7E))
		return true;
	return false;
}

bool kisspace(char character) {
	if (character == 0x09)
		return true;
	if ((character >= 0x0A) && (character <= 0x0D))
		return true;
	if (character == 0x20)
		return true;
	return false;
}

bool kisblank(char character) {
	if (character == 0x09)
		return true;
	if (character == 0x20)
		return true;
	return false;
}

bool kisprint(char character) {
	if ((character >= 0x20) && (character <= 0x7E))
		return true;
	return false;
}

bool kispunct(char character) {
	if ((character >= 0x21) && (character <= 0x2F))
		return true;
	if ((character >= 0x3A) && (character <= 0x40))
		return true;
	if ((character >= 0x5B) && (character <= 0x60))
		return true;
	if ((character >= 0x7B) && (character <= 0x7E))
		return true;
	return false;	
}

char ktolower(char character) {
	if ((character > 0x40) && (character < 0x5B)) {
		return character + 0x20;
	}
	return character;
}

char ktoupper(char character) {
	if ((character > 0x60) && (character < 0x7B)) {
		return character - 0x20;
	}
	return character;
}


