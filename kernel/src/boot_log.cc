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

#include <boot_log.h>

#include <stdarg.h>
#include <kstring.h>

#include <video/console.h>


BootSerial BootLog::serial_;
bool BootLog::use_serial_ = false;
bool BootLog::use_console_ = false;

ReturnValue BootLog::Init(void) {

	ReturnValue return_value = ReturnValueErrorGeneral;
	
	if (IS_SUCCESS(CConsole::Init(LimineStub::GetFramebufferResponse()))) {
		use_console_ = true;
	}
	
	// Try COM1 - 0x3F8
	return_value = serial_.Init(0x3F8, 9600, 8, BootSerialStopSize1, 
		BootSerialParityNone);
	if (IS_SUCCESS(return_value)) {
		use_serial_ = true;
	} else {	
		// Try COM2 - 0x2F8
		return_value = serial_.Init(0x2F8, 9600, 8, BootSerialStopSize1, 
			BootSerialParityNone);
		if (IS_SUCCESS(return_value)) {
			use_serial_ = true;
		} else {
			// Try COM3 - 0x3E8
			return_value = serial_.Init(0x3E8, 9600, 8, BootSerialStopSize1, 
				BootSerialParityNone);
			if (IS_SUCCESS(return_value)) {
				use_serial_ = true;
			} else {
				// Try COM4 - 0x2E8
				return_value = serial_.Init(0x2E8, 9600, 8, BootSerialStopSize1, 
					BootSerialParityNone);
				if (IS_SUCCESS(return_value)) {
					use_serial_ = true;
				}
			}
		}
	}

	if (use_serial_ || use_console_)
		return ReturnValueOk;
	return ReturnValueErrorGeneral;
	
}

void BootLog::Print(const char *string) {
	if (string == NULL)
		return;
	if (use_serial_)
		serial_.Send(string);
	if (use_console_)
		CConsole::Print(string);

}

void BootLog::PrintF(const char *string, ...) {
	
	va_list ap;
	static char buffer[BUFFER_SIZE] __attribute__((aligned(4096)));
	
	if (string == NULL)
		return;

	va_start(ap, string);

	kvsnprintf(buffer, BUFFER_SIZE, string, ap);
	
	if (use_serial_)
		serial_.Send(buffer);
	if (use_console_)
		CConsole::Print(buffer);
	
	va_end(ap);
}
