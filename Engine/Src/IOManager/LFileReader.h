#pragma once
#include "LFileStruct.h"
#include <wtypes.h>

class LFileReader
{
	LFileReader();
	~LFileReader();

public:
	static HRESULT Reader(LPCWSTR cszFileName, BYTE* pBuffer, size_t uMaxLen, size_t* puLen);

	template<typename Model>
	static BYTE* Convert(BYTE* pBuffer, Model*& pModel, size_t nCount = 1)
	{
		size_t nLen = sizeof(Model) * nCount;
		pModel = (Model*) pBuffer;
		return pBuffer + nLen;
	}
};