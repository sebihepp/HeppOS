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

enum ReturnValue {
	ReturnValueOk = 0,
	ReturnValueErrorTest = 1,					
	ReturnValueErrorGeneral,
	ReturnValueErrorLimineRevision,
	ReturnValueErrorNoFramebuffer,
	ReturnValueErrorUnsupportedVideoMode,
	ReturnValueErrorNoFXSave,
	ReturnValueErrorNo1GBPages,
	ReturnValueErrorNoGlobalPages,
	ReturnValueErrorCPUIDLevel,
	ReturnValueErrorCPUIDExtLevel,
	ReturnValueErrorNoPAT,
	ReturnValueErrorNoHHDM,
	ReturnValueErrorLiminePagingMode,
	ReturnValueErrorLimineNullPointer,
	ReturnValueErrorPageNotFound,
	ReturnValueErrorPageNotPresent,
	ReturnValueErrorInvalidPageLevel,
	ReturnValueErrorOutOfMemoryPhysical,
	ReturnValueErrorOutOfMemoryVirtual,

	ReturnValueWarningTest = 0x80000000,
	ReturnValueWarningGeneral,
};

#define IS_WARNING(x) ((x) & 0x80000000)
#define IS_ERROR(x) (((x) != ReturnValueOk) && !IS_WARNING(x))
#define IS_SUCCESS(x) ((x) == ReturnValueOk)

const char *GetReturnValueString(const ReturnValue return_value) __attribute__((const, nothrow));
