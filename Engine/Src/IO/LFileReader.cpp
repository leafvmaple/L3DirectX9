#include <cstdio>
#include "LFileReader.h"
#include "LAssert.h"

HRESULT LFileReader::Reader(LPCWSTR cszFileName, BYTE* pBuffer, size_t uMaxLen, size_t* puLen)
{
	HRESULT hResult = E_FAIL;
	FILE* pFile = NULL;
	
	do 
	{
		_wfopen_s(&pFile, cszFileName, TEXT("rb"));
		BOOL_ERROR_BREAK(pFile);

		*puLen = fread(pBuffer, 1, uMaxLen, pFile);
		BOOL_ERROR_BREAK(*puLen);

		hResult = S_OK;
	} while (0);

	if (pFile)
		fclose(pFile);

	return hResult;
}
