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

#include <log.h>

CSerial CLog::mSerial;
bool CLog::mInitialized = false;

ReturnValue_t CLog::Init(void) {

	ReturnValue_t _RetVal = RETVAL_ERROR_GENERAL;
	
	// Try COM1 - 0x3F8
	_RetVal = mSerial.Init(0x3F8, 9600, 8, SERIAL_STOPSIZE_1, SERIAL_PARITY_NONE);
	if (IS_SUCCESS(_RetVal)) {
		mInitialized = true;
		return RETVAL_OK;
	}
	
	// Try COM2 - 0x2F8
	_RetVal = mSerial.Init(0x2F8, 9600, 8, SERIAL_STOPSIZE_1, SERIAL_PARITY_NONE);
	if (IS_SUCCESS(_RetVal)) {
		mInitialized = true;
		return RETVAL_OK;
	}

	// Try COM3 - 0x3E8
	_RetVal = mSerial.Init(0x3E8, 9600, 8, SERIAL_STOPSIZE_1, SERIAL_PARITY_NONE);
	if (IS_SUCCESS(_RetVal)) {
		mInitialized = true;
		return RETVAL_OK;
	}
	
	// Try COM4 - 0x2E8
	_RetVal = mSerial.Init(0x2E8, 9600, 8, SERIAL_STOPSIZE_1, SERIAL_PARITY_NONE);
	if (IS_SUCCESS(_RetVal)) {
		mInitialized = true;
		return RETVAL_OK;
	}
	
	mInitialized = false;
	return _RetVal;
	
}

void CLog::Print(const char *pString) {
	if (mInitialized == false)
		return;
	if (pString == NULL)
		return;

	mSerial.Send(pString);
}

void CLog::PrintF(const char *pString, ...) {
	
	if (mInitialized == false)
		return;
	if (pString == NULL)
		return;

	
	mSerial.Send(pString);
}
