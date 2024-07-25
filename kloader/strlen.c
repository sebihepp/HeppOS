
#include "cxxstub.h"

size_t strlen(const char *s)
{
	size_t _length = 0;
	while (s[_length] != 0)
	{
		_length++;
	}
	return _length;
}
