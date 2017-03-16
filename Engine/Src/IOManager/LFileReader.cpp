#include <cstdio>
#include "LFileReader.h"
#include "LAssert.h"

HRESULT LFileReader::Reader(const char cszFileName[], BYTE* pBuffer, size_t uMaxLen, size_t* puLen)
{
	HRESULT hResult = E_FAIL;
	FILE* pFile = NULL;
	
	do 
	{
		fopen_s(&pFile, cszFileName, "rb");
		BOOL_ERROR_BREAK(pFile);

		*puLen = fread(pBuffer, 1, uMaxLen, pFile);
		BOOL_ERROR_BREAK(*puLen);

		hResult = S_OK;
	} while (0);

	if (pFile)
		fclose(pFile);

	return hResult;
}

HRESULT LFileReader::LoadMesh(const char cszFileName[], LMESH_DATA* pLMesh)
{
	HRESULT hResult = E_FAIL;
	BYTE* pbyMesh = new BYTE[1000000];
	BYTE* pbyBufferHead = NULL;
	_MeshFileHead* pMeshFileHead = NULL;
	_MeshHead* pMeshHead = NULL;
	size_t uMeshLen = 0;

	do 
	{
		Reader(cszFileName, pbyMesh, 1000000, &uMeshLen);
		pbyBufferHead = pbyMesh;

		pbyMesh = Convert(pbyMesh, pMeshFileHead);
		BOOL_ERROR_BREAK(pMeshFileHead->dwFileMask == 0x4D455348);
		BOOL_ERROR_BREAK(pMeshFileHead->dwVersion == 0);

		pbyMesh = Convert(pbyMesh, pMeshHead);
		pLMesh->dwNumVertices = pMeshHead->dwNumVertices;
		pLMesh->dwNumFaces    = pMeshHead->dwNumFaces;
		pLMesh->dwNumSubset   = pMeshHead->dwNumSubset;

		if (pMeshHead->Blocks.PositionBlock)
		{
			Convert(pbyBufferHead + pMeshHead->Blocks.PositionBlock, pLMesh->pPos, pMeshHead->dwNumVertices);
			pLMesh->dwMeshFVF |= D3DFVF_XYZ;

			//pLMesh->BoundingBox.AddPosition(pMeshData->pPos, pMeshData->dwNumVertices);
		}

		if (pMeshHead->Blocks.NormalBlock)
		{
			Convert(pbyBufferHead + pMeshHead->Blocks.NormalBlock, pLMesh->pNormals, pMeshHead->dwNumVertices);
			pLMesh->dwMeshFVF |= D3DFVF_NORMAL;
		}

		hResult = S_OK;
	} while (0);

	return hResult;
}
