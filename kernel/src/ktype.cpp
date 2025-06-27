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


bool kisalnum(char pChar) {
	if (kisalpha(pChar) || kisdigit(pChar))
		return true;
	return false;
}

bool kisalpha(char pChar) {
	if ((pChar >= 0x41) && (pChar <= 0x5A))
		return true;
	if ((pChar >= 0x61) && (pChar <= 0x7A))
		return true;
	return false;
}

bool kislower(char pChar) {
	if ((pChar >= 0x61) && (pChar <= 0x7A))
		return true;
	return false;	
}

bool kisupper(char pChar) {
	if ((pChar >= 0x41) && (pChar <= 0x5A))
		return true;
	return false;
}

bool kisdigit(char pChar) {
	if ((pChar >= 0x30) && (pChar <= 0x39))
		return true;
	return false;
}

bool kisxdigit(char pChar) {
	if ((pChar >= 0x30) && (pChar <= 0x39))
		return true;
	if ((pChar >= 0x41) && (pChar <= 0x46))
		return true;
	if ((pChar >= 0x61) && (pChar <= 0x66))
		return true;
	return false;
}

bool kiscntrl(char pChar) {
	if ((pChar >= 0x00) && (pChar <= 0x1F))
		return true;
	if (pChar == 0x7F)
		return true;
	return false;	
}

bool kisgraph(char pChar) {
	if ((pChar >= 0x21) && (pChar <= 0x7E))
		return true;
	return false;
}

bool kisspace(char pChar) {
	if (pChar == 0x09)
		return true;
	if ((pChar >= 0x0A) && (pChar <= 0x0D))
		return true;
	if (pChar == 0x20)
		return true;
	return false;
}

bool kisblank(char pChar) {
	if (pChar == 0x09)
		return true;
	if (pChar == 0x20)
		return true;
	return false;
}

bool kisprint(char pChar) {
	if ((pChar >= 0x20) && (pChar <= 0x7E))
		return true;
	return false;
}

bool kispunct(char pChar) {
	if ((pChar >= 0x21) && (pChar <= 0x2F))
		return true;
	if ((pChar >= 0x3A) && (pChar <= 0x40))
		return true;
	if ((pChar >= 0x5B) && (pChar <= 0x60))
		return true;
	if ((pChar >= 0x7B) && (pChar <= 0x7E))
		return true;
	return false;	
}

char ktolower(char pChar) {
	if ((pChar > 0x40) && (pChar < 0x5B)) {
		return pChar + 0x20;
	}
	return pChar;
}

char ktoupper(char pChar) {
	if ((pChar > 0x60) && (pChar < 0x7B)) {
		return pChar - 0x20;
	}
	return pChar;
}


