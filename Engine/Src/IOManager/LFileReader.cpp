#include <cstdio>
#include "LFileReader.h"
#include "LAssert.h"

const VertexFromatOffsetItem* GetVertexFormat(DWORD dwFVF)
{
	const VertexFromatOffsetItem *pReturn = NULL;
	size_t uSize = sizeof(s_VertexFormat) / sizeof(VertexFromatOffsetItem);
	size_t i = 0;
	for (; i < uSize; i++)
	{
		if (s_VertexFormat[i].dwFVF == dwFVF)
			break;
	}
	BOOL_ERROR_RETURN(i != uSize);

	pReturn = &s_VertexFormat[i];
	return pReturn;
}

DWORD GetVertexStride(DWORD dwFVF)
{
	DWORD dwStride = 0;
	const VertexFromatOffsetItem *pVertexFromatItem = GetVertexFormat(dwFVF);
	BOOL_ERROR_RETURN(pVertexFromatItem);

	dwStride = pVertexFromatItem->dwDestOffset[pVertexFromatItem->dwNumElement - 1] + 
		pVertexFromatItem->dwDestStride[pVertexFromatItem->dwNumElement - 1];

	return dwStride;
}


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

HRESULT LFileReader::LoadMesh(LPCWSTR cszFileName, LMESH_DATA* pLMesh)
{
	HRESULT hResult = E_FAIL;
	BYTE* pbyMesh = new BYTE[1000000];
	BYTE* pbyBufferHead = NULL;
	_MeshFileHead* pMeshFileHead = NULL;
	_MeshHead* pMeshHead = NULL;
	size_t uMeshLen = 0;

	ZeroMemory(pLMesh, sizeof(LMESH_DATA));

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

		if (pMeshHead->Blocks.DiffuseBlock)
		{
			Convert(pbyBufferHead + pMeshHead->Blocks.DiffuseBlock, pLMesh->pDiffuse, pMeshHead->dwNumVertices);
			pLMesh->dwMeshFVF |= D3DFVF_DIFFUSE;
		}

		if (pMeshHead->Blocks.TextureUVW1Block)
		{
			Convert(pbyBufferHead + pMeshHead->Blocks.TextureUVW1Block, pLMesh->pUV1, pMeshHead->dwNumVertices);
			pLMesh->dwMeshFVF |= D3DFVF_TEX1;

			//¼ÆËãUVSizeMax
			//BBox UVBox;
			//UVBox.AddPosition(pMeshData->pUV1, pMeshHead->dwNumVertices);
			//pMeshData->pUVSizeMax[0].x = fabs(UVBox.A.x) > fabs(UVBox.B.x) ? fabs(UVBox.A.x) : fabs(UVBox.B.x);
			//pMeshData->pUVSizeMax[0].y = fabs(UVBox.A.y) > fabs(UVBox.B.y) ? fabs(UVBox.A.y) : fabs(UVBox.B.y);
		}

		if (pMeshHead->Blocks.TextureUVW2Block)
		{
			Convert(pbyBufferHead + pMeshHead->Blocks.TextureUVW2Block, pLMesh->pUV2, pMeshHead->dwNumVertices);
			pLMesh->dwMeshFVF |= D3DFVF_TEX2;
			pLMesh->dwMeshFVF &= ~(D3DFVF_TEX1);

			//¼ÆËãUVSizeMax
			//BBox UVBox;
			//UVBox.AddPosition(pMeshData->pUV2, pMeshHead->dwNumVertices);
			//pMeshData->pUVSizeMax[1].x = fabs(UVBox.A.x) > fabs(UVBox.B.x) ? fabs(UVBox.A.x) : fabs(UVBox.B.x);
			//pMeshData->pUVSizeMax[1].y = fabs(UVBox.A.y) > fabs(UVBox.B.y) ? fabs(UVBox.A.y) : fabs(UVBox.B.y);
		}

		if (pMeshHead->Blocks.TextureUVW3Block)
		{
			Convert(pbyBufferHead + pMeshHead->Blocks.TextureUVW3Block, pLMesh->pUV3, pMeshHead->dwNumVertices);
			pLMesh->dwMeshFVF |= D3DFVF_TEX3;
			pLMesh->dwMeshFVF &= ~(D3DFVF_TEX1 | D3DFVF_TEX2);

			//¼ÆËãUVSizeMax
			//BBox UVBox;
			//UVBox.AddPosition(pMeshData->pUV3, pMeshHead->dwNumVertices);
			//pMeshData->pUVSizeMax[2].x = fabs(UVBox.A.x) > fabs(UVBox.B.x) ? fabs(UVBox.A.x) : fabs(UVBox.B.x);
			//pMeshData->pUVSizeMax[2].y = fabs(UVBox.A.y) > fabs(UVBox.B.y) ? fabs(UVBox.A.y) : fabs(UVBox.B.y);
		}

		if (pMeshHead->Blocks.FacesIndexBlock)
		{
			Convert(pbyBufferHead + pMeshHead->Blocks.FacesIndexBlock, pLMesh->pFaceIndices, pMeshHead->dwNumVertices);
		}

		if (pMeshHead->Blocks.SubsetIndexBlock)
		{
			Convert(pbyBufferHead + pMeshHead->Blocks.SubsetIndexBlock, pLMesh->pSubsetIndices, pMeshHead->dwNumVertices);
		}

		if (pMeshHead->Blocks.SkinInfoBlock)
		{
			//Convert(pbyBufferHead + pMeshHead->Blocks.SubsetIndexBlock, pLMesh->pSubsetIndices, pMeshHead->dwNumVertices);

			if (pMeshHead->Blocks.BBoxBlock)
			{
				pLMesh->dwOptionEx |= MESH_SAVE_BONEBBOX;
			}

			// load skininfo
			//hrRetCode = _LoadSkinInfo(&BufferReader, pMeshData);
			//KGLOG_COM_PROCESS_ERROR(hrRetCode);

			// load socket
			//hrRetCode = _LoadSocket(&BufferReader, pMeshData);
			//KGLOG_COM_PROCESS_ERROR(hrRetCode);
		}

		pLMesh->pbyFileBuffer = pbyBufferHead;

		hResult = S_OK;
	} while (0);

	return hResult;
}
