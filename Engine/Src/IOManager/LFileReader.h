#pragma once
#include "Model/L3DMeshStruct.h"

const VertexFromatOffsetItem* GetVertexFormat(DWORD dwFVF);
DWORD GetVertexStride(DWORD dwFVF);

class LFileReader
{
	LFileReader();
	~LFileReader();

public:
	static HRESULT LoadMesh(LPCWSTR cszFileName, LMESH_DATA* pLMesh);

private:
	static HRESULT Reader(LPCWSTR cszFileName, BYTE* pBuffer, size_t uMaxLen, size_t* puLen);

	template<typename Model>
	static BYTE* Convert(BYTE* pBuffer, Model*& pModel, size_t nCount = 1)
	{
		size_t nLen = sizeof(Model) * nCount;
		pModel = (Model*) pBuffer;
		return pBuffer + nLen;
	}
};