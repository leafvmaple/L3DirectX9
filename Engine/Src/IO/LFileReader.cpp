#include <cstdio>
#include <io.h>
#include "LFileReader.h"
#include "LAssert.h"

HRESULT LFileReader::Reader(LPCWSTR cszFileName, BYTE** ppBuffer, size_t* puLen)
{
	HRESULT hResult = E_FAIL;
	FILE* pFile = NULL;
	BYTE* pBuffer = NULL;
	size_t uFileLen;
	
	do 
	{
		BOOL_ERROR_BREAK(ppBuffer);

		_wfopen_s(&pFile, cszFileName, TEXT("rb"));
		BOOL_ERROR_BREAK(pFile);

		fseek(pFile, 0, SEEK_END);
		uFileLen = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);

		pBuffer = new BYTE[uFileLen];
		BOOL_ERROR_BREAK(pBuffer);

		*puLen = fread(pBuffer, 1, uFileLen, pFile);
		BOOL_ERROR_BREAK(*puLen);

		*ppBuffer = pBuffer;

		hResult = S_OK;
	} while (0);

	if (pFile)
		fclose(pFile);

	return hResult;
}

BOOL LFileReader::IsExist(LPCWSTR cszFileName)
{
	return _waccess(cszFileName, 0) != -1;
}
