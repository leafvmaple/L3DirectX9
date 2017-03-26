#include <d3dx9.h>
#include "L3DMesh.h"
#include "LAssert.h"
#include "IOManager/LFileReader.h"

const VertexFromatOffsetItem* L3DMesh::GetVertexFormat(DWORD dwFVF)
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

DWORD L3DMesh::GetVertexStride(DWORD dwFVF)
{
	DWORD dwStride = 0;
	const VertexFromatOffsetItem *pVertexFromatItem = GetVertexFormat(dwFVF);
	BOOL_ERROR_RETURN(pVertexFromatItem);

	dwStride = pVertexFromatItem->dwDestOffset[pVertexFromatItem->dwNumElement - 1] + 
		pVertexFromatItem->dwDestStride[pVertexFromatItem->dwNumElement - 1];

	return dwStride;
}


L3DMesh::L3DMesh()
: m_pMeshBase(NULL)
, m_dwNumSubset(0)
{

}

L3DMesh::~L3DMesh()
{

}

HRESULT L3DMesh::LoadXMesh(IDirect3DDevice9* p3DDevice, LPCWSTR cszFileName)
{
	return S_OK;
}

HRESULT L3DMesh::LoadLMesh(IDirect3DDevice9* p3DDevice, LPCWSTR cszFileName)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	LMESH_DATA MeshData;
	ID3DXMesh* pMesh = NULL;
	BYTE* pbyVertices = NULL;
	WORD* pwIndices = NULL;
	DWORD *pwAttributes = NULL;
	DWORD dwVertexStride = 0;
	DWORD dwDestVertexStride = 0;
	const VertexFromatOffsetItem *pVertexFormat = NULL;

	do
	{
		hr = LoadLMeshBuffer(cszFileName, &MeshData);
		HRESULT_ERROR_BREAK(hr);

		if (MeshData.dwNumVertices <= 65535 && MeshData.dwNumFaces <= 65535)
		{
			hr = D3DXCreateMeshFVF(MeshData.dwNumFaces, MeshData.dwNumVertices,  D3DXMESH_SYSTEMMEM, MeshData.dwMeshFVF, p3DDevice, &pMesh);
			HRESULT_ERROR_BREAK(hr);
		}
		else
		{
			hr = D3DXCreateMeshFVF(MeshData.dwNumFaces, MeshData.dwNumVertices,  D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT,  MeshData.dwMeshFVF, p3DDevice, &pMesh);
			HRESULT_ERROR_BREAK(hr);
		}

		dwDestVertexStride = pMesh->GetNumBytesPerVertex();
		BOOL_ERROR_BREAK(dwDestVertexStride);

		hr = pMesh->LockVertexBuffer(0, (void**)(&pbyVertices));
		HRESULT_ERROR_BREAK(hr);

		pVertexFormat = GetVertexFormat(MeshData.dwMeshFVF);
		BOOL_ERROR_BREAK(pVertexFormat);

		dwVertexStride = GetVertexStride(MeshData.dwMeshFVF);

		for (DWORD i = 0; i < MeshData.dwNumVertices; i++)
		{
			BYTE *pCurrentVertexData = pbyVertices + dwDestVertexStride * i;
			for (DWORD j = 0; j < pVertexFormat->dwNumElement; j++)
			{
				const BYTE *pCurrentSrc = *(reinterpret_cast<BYTE* const*>(&MeshData.pPos) + pVertexFormat->dwSrcOffset[j]);
				memcpy(pCurrentVertexData + pVertexFormat->dwDestOffset[j],
					pCurrentSrc + pVertexFormat->dwSrcStride[j] * i,
					pVertexFormat->dwDestStride[j]);
			}
		}

		hr = pMesh->LockIndexBuffer(0, (void**)&pwIndices);
		HRESULT_ERROR_BREAK(hr);

		hr = pMesh->LockAttributeBuffer(0, &pwAttributes);
		HRESULT_ERROR_BREAK(hr);


		for(DWORD i = 0; i < MeshData.dwNumFaces; i++)
		{
			pwIndices[i * 3]     = static_cast<WORD>(MeshData.pFaceIndices[i * 3]);
			pwIndices[i * 3 + 1] = static_cast<WORD>(MeshData.pFaceIndices[i * 3 + 1]);
			pwIndices[i * 3 + 2] = static_cast<WORD>(MeshData.pFaceIndices[i * 3 + 2]);
			pwAttributes[i] = MeshData.pSubsetIndices[i];
		}

		pMesh->UnlockVertexBuffer();
		pMesh->UnlockIndexBuffer();
		pMesh->UnlockAttributeBuffer();

		hResult = S_OK;
	} while (0);

	m_pMeshBase = pMesh;
	m_dwNumSubset = MeshData.dwNumSubset;

	return hResult;
}

HRESULT L3DMesh::UpdateMesh(DWORD dwSubMesh)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		hr = m_pMeshBase->DrawSubset(dwSubMesh);
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);

	return hResult;
}

DWORD L3DMesh::GetSubsetCount() const
{
	return m_dwNumSubset;
}

HRESULT L3DMesh::LoadLMeshBuffer(LPCWSTR cszFileName, LMESH_DATA* pLMeshData)
{
	HRESULT hResult = E_FAIL;
	BYTE* pbyMesh = new BYTE[1000000];
	BYTE* pbyBufferHead = NULL;
	_MeshFileHead* pMeshFileHead = NULL;
	_MeshHead* pMeshHead = NULL;
	size_t uMeshLen = 0;

	do 
	{
		ZeroMemory(pLMeshData, sizeof(LMESH_DATA));

		LFileReader::Reader(cszFileName, pbyMesh, 1000000, &uMeshLen);
		pbyBufferHead = pbyMesh;

		pbyMesh = LFileReader::Convert(pbyMesh, pMeshFileHead);
		BOOL_ERROR_BREAK(pMeshFileHead->dwFileMask == 0x4D455348);
		BOOL_ERROR_BREAK(pMeshFileHead->dwVersion == 0);

		pbyMesh = LFileReader::Convert(pbyMesh, pMeshHead);
		pLMeshData->dwNumVertices = pMeshHead->dwNumVertices;
		pLMeshData->dwNumFaces    = pMeshHead->dwNumFaces;
		pLMeshData->dwNumSubset   = pMeshHead->dwNumSubset;

		if (pMeshHead->Blocks.PositionBlock)
		{
			LFileReader::Convert(pbyBufferHead + pMeshHead->Blocks.PositionBlock, pLMeshData->pPos, pMeshHead->dwNumVertices);
			pLMeshData->dwMeshFVF |= D3DFVF_XYZ;

			//pLMesh->BoundingBox.AddPosition(pMeshData->pPos, pMeshData->dwNumVertices);
		}

		if (pMeshHead->Blocks.NormalBlock)
		{
			LFileReader::Convert(pbyBufferHead + pMeshHead->Blocks.NormalBlock, pLMeshData->pNormals, pMeshHead->dwNumVertices);
			pLMeshData->dwMeshFVF |= D3DFVF_NORMAL;
		}

		if (pMeshHead->Blocks.DiffuseBlock)
		{
			LFileReader::Convert(pbyBufferHead + pMeshHead->Blocks.DiffuseBlock, pLMeshData->pDiffuse, pMeshHead->dwNumVertices);
			pLMeshData->dwMeshFVF |= D3DFVF_DIFFUSE;
		}

		if (pMeshHead->Blocks.TextureUVW1Block)
		{
			LFileReader::Convert(pbyBufferHead + pMeshHead->Blocks.TextureUVW1Block, pLMeshData->pUV1, pMeshHead->dwNumVertices);
			pLMeshData->dwMeshFVF |= D3DFVF_TEX1;

			//¼ÆËãUVSizeMax
			//BBox UVBox;
			//UVBox.AddPosition(pMeshData->pUV1, pMeshHead->dwNumVertices);
			//pMeshData->pUVSizeMax[0].x = fabs(UVBox.A.x) > fabs(UVBox.B.x) ? fabs(UVBox.A.x) : fabs(UVBox.B.x);
			//pMeshData->pUVSizeMax[0].y = fabs(UVBox.A.y) > fabs(UVBox.B.y) ? fabs(UVBox.A.y) : fabs(UVBox.B.y);
		}

		if (pMeshHead->Blocks.TextureUVW2Block)
		{
			LFileReader::Convert(pbyBufferHead + pMeshHead->Blocks.TextureUVW2Block, pLMeshData->pUV2, pMeshHead->dwNumVertices);
			pLMeshData->dwMeshFVF |= D3DFVF_TEX2;
			pLMeshData->dwMeshFVF &= ~(D3DFVF_TEX1);

			//¼ÆËãUVSizeMax
			//BBox UVBox;
			//UVBox.AddPosition(pMeshData->pUV2, pMeshHead->dwNumVertices);
			//pMeshData->pUVSizeMax[1].x = fabs(UVBox.A.x) > fabs(UVBox.B.x) ? fabs(UVBox.A.x) : fabs(UVBox.B.x);
			//pMeshData->pUVSizeMax[1].y = fabs(UVBox.A.y) > fabs(UVBox.B.y) ? fabs(UVBox.A.y) : fabs(UVBox.B.y);
		}

		if (pMeshHead->Blocks.TextureUVW3Block)
		{
			LFileReader::Convert(pbyBufferHead + pMeshHead->Blocks.TextureUVW3Block, pLMeshData->pUV3, pMeshHead->dwNumVertices);
			pLMeshData->dwMeshFVF |= D3DFVF_TEX3;
			pLMeshData->dwMeshFVF &= ~(D3DFVF_TEX1 | D3DFVF_TEX2);

			//¼ÆËãUVSizeMax
			//BBox UVBox;
			//UVBox.AddPosition(pMeshData->pUV3, pMeshHead->dwNumVertices);
			//pMeshData->pUVSizeMax[2].x = fabs(UVBox.A.x) > fabs(UVBox.B.x) ? fabs(UVBox.A.x) : fabs(UVBox.B.x);
			//pMeshData->pUVSizeMax[2].y = fabs(UVBox.A.y) > fabs(UVBox.B.y) ? fabs(UVBox.A.y) : fabs(UVBox.B.y);
		}

		if (pMeshHead->Blocks.FacesIndexBlock)
		{
			LFileReader::Convert(pbyBufferHead + pMeshHead->Blocks.FacesIndexBlock, pLMeshData->pFaceIndices, 3 * pMeshHead->dwNumFaces);
		}

		if (pMeshHead->Blocks.SubsetIndexBlock)
		{
			LFileReader::Convert(pbyBufferHead + pMeshHead->Blocks.SubsetIndexBlock, pLMeshData->pSubsetIndices, pMeshHead->dwNumFaces);
		}

		if (pMeshHead->Blocks.SkinInfoBlock)
		{
			//Convert(pbyBufferHead + pMeshHead->Blocks.SubsetIndexBlock, pLMesh->pSubsetIndices, pMeshHead->dwNumVertices);

			if (pMeshHead->Blocks.BBoxBlock)
			{
				pLMeshData->dwOptionEx |= MESH_SAVE_BONEBBOX;
			}

			// load skininfo
			//hrRetCode = _LoadSkinInfo(&BufferReader, pMeshData);
			//KGLOG_COM_PROCESS_ERROR(hrRetCode);

			// load socket
			//hrRetCode = _LoadSocket(&BufferReader, pMeshData);
			//KGLOG_COM_PROCESS_ERROR(hrRetCode);
		}

		pLMeshData->pbyFileBuffer = pbyBufferHead;

		hResult = S_OK;
	} while (0);

	return hResult;
}