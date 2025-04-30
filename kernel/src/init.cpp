
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

