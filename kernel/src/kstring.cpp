
#include <kstring.h>

size_t kstrlen(const char *pString) {
	size_t _Count = 0;
	while (pString[_Count] != 0) {
		++_Count;
	}
	return _Count;
}

char *kstrcpy(const char *pSource, char *pDest) {
	uint64_t i = 0;
	while (pSource[i] != 0) {
		pDest[i] = pSource[i];
		++i;
	}
	pDest[i] = 0;
	return pDest;
}

char *kstrncpy(const char *pSource, char *pDest, size_t pLength) {
	uint64_t i = 0;
	while (i < pLength) {
		pDest[i] = pSource[i];
		if (pSource[i] == 0)
			break;
		++i;
	}
	return pDest;	
}

int32_t kstrcmp(const char *pString1, const char *pString2) {
	size_t i = 0;
	while (true) {
		if (pString1[i] < pString2[i])
			return -1;
		if (pString1[i] > pString2[i])
			return 1;
		if (pString1[i] == 0)
			return 0;
		++i;
	}
	return 0;
}

int32_t kstrncmp(const char *pString1, const char *pString2, size_t pLength) {
	size_t i = 0;
	while (i < pLength) {
		if (pString1[i] < pString2[i])
			return -1;
		if (pString1[i] > pString2[i])
			return 1;
		if (pString1[i] == 0)
			return 0;
		++i;
	}
	return 0;
}

