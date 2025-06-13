
#include <kstring.h>

size_t kstrlen(const char *pString) {
	size_t _Count = 0;
	while (pString[_Count] != 0) {
		++_Count;
	}
	return _Count;
}

char *kstrcpy(char *pDest, const char *pSource) {
	uint64_t i = 0;
	while (pSource[i] != 0) {
		pDest[i] = pSource[i];
		++i;
	}
	pDest[i] = 0;
	return pDest;
}

char *kstrncpy(char *pDest, const char *pSource, size_t pLength) {
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
	uint64_t i = 0;
	while ((pString1[i] == pString2[i]) && (pString1[i] != 0)) {
		++i;
	}
	return pString1[i] - pString2[i];
}

int32_t kstrncmp(const char *pString1, const char *pString2, size_t pLength) {
	uint64_t i = 0;
	while ((pString1[i] == pString2[i]) && (pString1[i] != 0)) {
		if (i >= pLength)
			break;
		++i;
	}
	return pString1[i] - pString2[i];
}

int32_t kstricmp(const char *pString1, const char *pString2) {
	uint64_t i = 0;
	while ((kto_lower(pString1[i]) == kto_lower(pString2[i])) && (pString1[i] != 0)) {
		++i;
	}
	return pString1[i] - pString2[i];
}

int32_t kstrnicmp(const char *pString1, const char *pString2, size_t pLength) {
	uint64_t i = 0;
	while ((kto_lower(pString1[i]) == kto_lower(pString2[i])) && (pString1[i] != 0)) {
		if (i >= pLength)
			break;
		++i;
	}
	return pString1[i] - pString2[i];
}

char kto_lower(char pChar) {
	if ((pChar > 0x40) && (pChar < 0x5B)) {
		return pChar + 0x20;
	}
	return pChar;
}

char kto_upper(char pChar) {
	if ((pChar > 0x60) && (pChar < 0x7B)) {
		return pChar - 0x20;
	}
	return pChar;
}

char *kstrcat(char *pDest, const char *pSource) {
	char *_target = pDest + kstrlen(pDest);
	uint64_t i = 0;
	while (pSource[i] != 0) {
		_target[i] = pSource[i];
		++i;
	}
	_target[i] = 0;
	return pDest;
}

char *kstrncat(char *pDest, const char *pSource, size_t pLength) {
	char *_target = pDest + kstrlen(pDest);
	uint64_t i = 0;
	while ((pSource[i] != 0) && (i < pLength)) {
		_target[i] = pSource[i];
		++i;
	}
	_target[i] = 0;
	return pDest;
}

char *kstrchr(const char *pSource, char pChar) {
	uint64_t i = 0;
	while (pSource[i] != 0) {
		if (pSource[i] == pChar)
			return (char*)&pSource[i];
		++i;
	}
	return NULL;
}
