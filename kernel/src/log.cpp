

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
	
	mSerial.Send(pString);
}

void CLog::PrintF(const char *pString, ...) {
	
	if (mInitialized == false)
		return;
	
	mSerial.Send(pString);
}
