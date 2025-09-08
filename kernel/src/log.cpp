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

#include <stdarg.h>
#include <kstring.h>

#include <video/console.h>


CSerial CLog::mSerial;
bool CLog::mUseSerial = false;
bool CLog::mUseConsole = false;

ReturnValue_t CLog::Init(void) {

	ReturnValue_t _RetVal = RETVAL_ERROR_GENERAL;
	
	if (IS_SUCCESS(CConsole::Init(CLimine::GetFramebufferResponse()))) {
		mUseConsole = true;
	}
	
	// Try COM1 - 0x3F8
	_RetVal = mSerial.Init(0x3F8, 9600, 8, SERIAL_STOPSIZE_1, SERIAL_PARITY_NONE);
	if (IS_SUCCESS(_RetVal)) {
		mUseSerial = true;
	} else {	
		// Try COM2 - 0x2F8
		_RetVal = mSerial.Init(0x2F8, 9600, 8, SERIAL_STOPSIZE_1, SERIAL_PARITY_NONE);
		if (IS_SUCCESS(_RetVal)) {
			mUseSerial = true;
		} else {
			// Try COM3 - 0x3E8
			_RetVal = mSerial.Init(0x3E8, 9600, 8, SERIAL_STOPSIZE_1, SERIAL_PARITY_NONE);
			if (IS_SUCCESS(_RetVal)) {
				mUseSerial = true;
			} else {
				// Try COM4 - 0x2E8
				_RetVal = mSerial.Init(0x2E8, 9600, 8, SERIAL_STOPSIZE_1, SERIAL_PARITY_NONE);
				if (IS_SUCCESS(_RetVal)) {
					mUseSerial = true;
				}
			}
		}
	}

	if (mUseSerial ||mUseConsole)
		return RETVAL_OK;
	return RETVAL_ERROR_GENERAL;
	
}

void CLog::Print(const char *pString) {
	if (pString == NULL)
		return;
	if (mUseSerial)
		mSerial.Send(pString);
	if (mUseConsole)
		CConsole::Print(pString);

}

void CLog::PrintF(const char *pString, ...) {
	
	if (pString == NULL)
		return;

	va_list _ap;
	va_start(_ap, pString);
	
	static char _Buffer[4096] __attribute__ (( aligned (4096) ));
	kvsprintf(_Buffer, pString, _ap);
	
	if (mUseSerial)
		mSerial.Send(_Buffer);
	if (mUseConsole)
		CConsole::Print(_Buffer);
	
	va_end(_ap);
}
