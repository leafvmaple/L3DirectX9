#pragma once
#include "LFileStruct.h"
#include <wtypes.h>

class LFileReader
{
	LFileReader();
	~LFileReader();

public:
	static HRESULT Reader(LPCWSTR cszFileName, BYTE** ppBuffer, size_t* puLen);
	static BOOL IsExist(LPCWSTR cszFileName);

	template<typename Model>
	static BYTE* Convert(BYTE* pBuffer, Model*& pModel, size_t nCount = 1)
	{
		size_t nLen = sizeof(Model) * nCount;
		pModel = (Model*) pBuffer;
		return pBuffer + nLen;
	}

	template<typename Model>
	static BYTE* Convert(BYTE* pBuffer, Model& pModel)
	{
		pModel = *((Model*) pBuffer);
		return pBuffer + sizeof(Model);
	}

	template<typename Model, int nSize>
	static BYTE* Copy(BYTE* pBuffer, Model (&pModel)[nSize])
	{
		size_t nLen = sizeof(Model) * nSize;
		memcpy_s(&pModel, nLen, pBuffer, nLen);
		return pBuffer + nLen;
	}

	template<typename Model>
	static BYTE* Copy(BYTE* pBuffer, Model*& pModel, size_t nCount = 1)
	{
		size_t nLen = sizeof(Model) * nCount;
		memcpy_s(pModel, nLen, pBuffer, nLen);
		return pBuffer + nLen;
	}
};