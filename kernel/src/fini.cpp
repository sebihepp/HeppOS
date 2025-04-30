
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint64_t FINI_ARRAY_START;
extern uint64_t FINI_ARRAY_END;

typedef void (*Destructor_t)(void);

void _fini(void) __attribute(( nothrow ));

void _fini(void) {
	
	uint64_t *_Start = 0;
	uint64_t *_End = 0;
	
	_Start = reinterpret_cast<uint64_t*>(&FINI_ARRAY_START);
	_End = reinterpret_cast<uint64_t*>(&FINI_ARRAY_END);
	_End--;
	for (uint64_t *i = _End; i >= _Start; i--) {
		Destructor_t _Func = reinterpret_cast<Destructor_t>(*i);
		(*_Func)();
	}

	
}


#ifdef __cplusplus
}
#endif

