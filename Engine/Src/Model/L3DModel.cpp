#include <atlconv.h>
#include "L3DModel.h"
#include "LAssert.h"
#include "LCommon.h"
#include "IOManager/LFileReader.h"
#include "Model/L3DMesh.h"

L3DModel::L3DModel()
: m_p3DDevice(NULL)
, m_pAdjBuffer(NULL)
, m_pMaterial(NULL)
, m_ppTexture(NULL)
, m_dwSubsetCount(0)
, m_ObjectType(LOBJECT_TYPE_INVALID)
, m_fAlpha(1.f)
, m_fScale(1.f)
, m_dwRenderParam(0)
, m_dwOptimizeParam(0)
{
	ZeroMemory(&m_DisplaySource, sizeof(m_DisplaySource));
	ZeroMemory(m_vTranslation, sizeof(m_vTranslation));
	ZeroMemory(m_qRotation, sizeof(m_qRotation));
	ZeroMemory(m_matTransform, sizeof(m_matTransform));
}

L3DModel::~L3DModel()
{
	switch (m_ObjectType)
	{
	case LOBJECT_TYPE_INVALID:
		break;
	case LOBJECT_TYPE_VERTEX:
		if (m_DisplaySource.LVertex.pVertexBuffer)
			m_DisplaySource.LVertex.pVertexBuffer->Release();
		if (m_DisplaySource.LVertex.pIndexBuffer)
			m_DisplaySource.LVertex.pIndexBuffer->Release();
		break;
	case LOBJECT_TYPE_X:
		if (m_DisplaySource.LMesh.pMesh)
			m_DisplaySource.LMesh.pMesh->Release();
		break;
	default:
		break;
	}
}

HRESULT L3DModel::Init(IDirect3DDevice9* p3DDevice, TexVertex* pModelVerteices, UINT nVerteicesCount, WORD* pwModelIndices, UINT nIndicesCount)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	IDirect3DVertexBuffer9* pVertexBuffer = NULL;
	IDirect3DIndexBuffer9* pIndexBuffer = NULL;
	TexVertex* pVertices = NULL;
	WORD* pwIndices = NULL;

	do
	{
		hr = p3DDevice->CreateVertexBuffer(
			nVerteicesCount, D3DUSAGE_WRITEONLY,
			TEX_VERTEX_FVF , D3DPOOL_MANAGED, &pVertexBuffer, 0);
		HRESULT_ERROR_BREAK(hr);

		hr = p3DDevice->CreateIndexBuffer(
			nIndicesCount, D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIndexBuffer, 0);
		HRESULT_ERROR_BREAK(hr);

		pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
		memcpy_s(pVertices, nVerteicesCount, pModelVerteices, nVerteicesCount);
		pVertexBuffer->Unlock();

		pIndexBuffer->Lock(0, 0, (void**)&pwIndices, 0);
		memcpy_s(pwIndices, nIndicesCount, pwModelIndices, nIndicesCount);
		pIndexBuffer->Unlock();

		m_p3DDevice = p3DDevice;
		m_ObjectType = LOBJECT_TYPE_VERTEX;
		m_dwSubsetCount = 1;
		m_DisplaySource.LVertex.pVertexBuffer = pVertexBuffer;
		m_DisplaySource.LVertex.pIndexBuffer = pIndexBuffer;

		m_pMaterial = new D3DMATERIAL9[m_dwSubsetCount];
		BOOL_ERROR_BREAK(m_pMaterial);

		m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCount];
		BOOL_ERROR_BREAK(m_ppTexture);

		ZeroMemory(m_pMaterial, sizeof(D3DMATERIAL9) * m_dwSubsetCount);
		ZeroMemory(m_ppTexture, sizeof(LPDIRECT3DTEXTURE9) * m_dwSubsetCount);

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT L3DModel::Init(IDirect3DDevice9* p3DDevice, ID3DXBaseMesh** ppMesh, LOBJECT_MESH_TYPE eModelType, LPCWSTR pcszFileName)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	ID3DXBuffer* pMtlBuffer = NULL;
	LPD3DXMATERIAL pMtls = NULL;
	ID3DXPMesh* pProgressMesh = NULL;
	ID3DXMesh* pCloneMesh = NULL;
	ID3DXMesh* pMesh = NULL;
	size_t uDirLength = 0;
	WCHAR wcszDir[LENGIEN_FONT_STRING_MAX];

	do 
	{
		switch (eModelType)
		{
		case LOBJECT_MESH_TEAPOT:
			hr = D3DXCreateTeapot(p3DDevice, &pMesh, &m_pAdjBuffer);
			HRESULT_ERROR_BREAK(hr);

			m_dwSubsetCount = 1;
			m_pMaterial = new D3DMATERIAL9[m_dwSubsetCount];
			BOOL_ERROR_BREAK(m_pMaterial);

			m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCount];
			BOOL_ERROR_BREAK(m_ppTexture);

			ZeroMemory(m_pMaterial, sizeof(D3DMATERIAL9) * m_dwSubsetCount);
			ZeroMemory(m_ppTexture, sizeof(LPDIRECT3DTEXTURE9) * m_dwSubsetCount);

			break;
		case LOBJECT_MESH_DX:
			hr = D3DXLoadMeshFromX(pcszFileName, D3DXMESH_MANAGED, p3DDevice, &m_pAdjBuffer, &pMtlBuffer, NULL, &m_dwSubsetCount, &pMesh);
			HRESULT_ERROR_BREAK(hr);

			pMtls = (LPD3DXMATERIAL)pMtlBuffer->GetBufferPointer();
			BOOL_ERROR_BREAK(pMtls);

			m_pMaterial = new D3DMATERIAL9[m_dwSubsetCount];
			BOOL_ERROR_BREAK(m_pMaterial);

			m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCount];
			BOOL_ERROR_BREAK(m_ppTexture);

			ZeroMemory(m_pMaterial, sizeof(D3DMATERIAL9) * m_dwSubsetCount);
			ZeroMemory(m_ppTexture, sizeof(LPDIRECT3DTEXTURE9) * m_dwSubsetCount);

			for (DWORD u = 0; u < m_dwSubsetCount; u++)
			{
				m_pMaterial[u] = pMtls[u].MatD3D;
				m_pMaterial[u].Ambient = m_pMaterial[u].Diffuse;

				if (pMtls[u].pTextureFilename)
				{
					USES_CONVERSION;

					uDirLength = L3D::GetPathDir(pcszFileName, wcszDir);
					BOOL_ERROR_BREAK(uDirLength);

					L3D::GetFullPath(A2CW(pMtls[u].pTextureFilename), wcszDir);

					hr = D3DXCreateTextureFromFile(p3DDevice, wcszDir, &m_ppTexture[u]);
					HRESULT_ERROR_BREAK(hr);

					m_dwRenderParam |= LOBJECT_RENDER_TEXTURE;
				}
				m_dwRenderParam |= LOBJECT_RENDER_MATERIAL;
			}

			SAFE_RELEASE(pMtlBuffer);

		case LOBJECT_MESH_LX:
			{
				L3DMesh lMesh;
				lMesh.LoadMesh(p3DDevice, pcszFileName);
			}
			
			break;
		default:
			break;
		}

		if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
		{
			hr = pMesh->CloneMeshFVF(D3DXMESH_MANAGED, pMesh->GetFVF() | D3DFVF_NORMAL, p3DDevice, &pCloneMesh);
			HRESULT_ERROR_BREAK(hr);

			hr = D3DXComputeNormals(pCloneMesh, (DWORD*)m_pAdjBuffer->GetBufferPointer());
			if (hr == S_OK)
			{
				SAFE_RELEASE(pMesh);
				pMesh = pCloneMesh;
				m_dwOptimizeParam |= LOBJECT_OPTIMIZE_FVF;
			}
			else
			{
				SAFE_RELEASE(pCloneMesh);
			}
		}

		if (m_pAdjBuffer)
		{
			hr = D3DXGeneratePMesh(pMesh, (DWORD*)m_pAdjBuffer->GetBufferPointer(), 0, 0, 1, D3DXMESHSIMP_FACE, &pProgressMesh);
			if (hr == S_OK)
			{
				SAFE_RELEASE(pMesh);
				*ppMesh = pProgressMesh;
				m_dwOptimizeParam |= LOBJECT_OPTIMIZE_PROGRESSIVE;
			}
			else
			{
				SAFE_RELEASE(pProgressMesh);
			}
		}

		m_p3DDevice = p3DDevice;
		m_ObjectType = LOBJECT_TYPE_X;
		m_DisplaySource.LMesh.pMesh = pMesh;
		*ppMesh = pMesh;

		hResult = S_OK;
	} while (0);

	return hResult;
}


HRESULT L3DModel::SetAlpha(float fAlpha)
{
	m_fAlpha = fAlpha;
	m_dwRenderParam |= LOBJECT_RENDER_ALPHA;
	return S_OK;
}

HRESULT L3DModel::SetScale(float fScale)
{
	m_fScale = fScale;
	return S_OK;
}

HRESULT L3DModel::SetTexture(LPCWSTR szTexture)
{
	for (DWORD u = 0; u < m_dwSubsetCount; u++)
		D3DXCreateTextureFromFile(m_p3DDevice, szTexture, &m_ppTexture[u]);
	m_dwRenderParam |= LOBJECT_RENDER_TEXTURE;
	return S_OK;
}

HRESULT L3DModel::SetMaterial(const D3DMATERIAL9& Material)
{
	for (DWORD u = 0; u < m_dwSubsetCount; u++)
		memcpy_s(&m_pMaterial[u], sizeof(D3DMATERIAL9), &Material, sizeof(D3DMATERIAL9));
	m_dwRenderParam |= LOBJECT_RENDER_MATERIAL;
	return S_OK;
}

HRESULT L3DModel::SetTranslation(const D3DXVECTOR3& vTranslation)
{
	m_vTranslation = vTranslation;
	m_dwRenderParam |= LOBJECT_RENDER_TRANSFORM;
	return S_OK;
}

HRESULT L3DModel::SetRotation(const D3DXQUATERNION& qRotation)
{
	m_qRotation = qRotation;
	m_dwRenderParam |= LOBJECT_RENDER_TRANSFORM;
	return S_OK;
}

HRESULT L3DModel::UpdateDisplay()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		hr = UpdateRenderState();
		HRESULT_ERROR_BREAK(hr);

		hr = UpdateTransform();
		HRESULT_ERROR_BREAK(hr);

		hr = UpdateLOD();
		HRESULT_ERROR_BREAK(hr);

		for (DWORD u = 0; u < m_dwSubsetCount; u++)
		{
			hr = UpdateMaterial(u);
			HRESULT_ERROR_BREAK(hr);

			hr = UpdateTexture(u);
			HRESULT_ERROR_BREAK(hr);

			hr = UpdateDraw(u);
			HRESULT_ERROR_BREAK(hr);
		}

		hr = ResetRendState();
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);

	return S_OK;
}

HRESULT L3DModel::UpdateRenderState()
{
	if (m_dwRenderParam & LOBJECT_RENDER_ALPHA)
		m_p3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	return S_OK;
}

HRESULT L3DModel::UpdateTransform()
{
	D3DXMatrixTransformation(&m_matTransform, NULL, NULL, &D3DXVECTOR3(m_fScale, m_fScale, m_fScale), NULL, &m_qRotation, &m_vTranslation);
	m_p3DDevice->SetTransform(D3DTS_WORLD, &m_matTransform);
	return S_OK;
}

HRESULT L3DModel::UpdateLOD()
{
	ID3DXPMesh* pProgressMesh = static_cast<ID3DXPMesh*>(m_DisplaySource.LMesh.pMesh);
	if (m_dwOptimizeParam & LOBJECT_OPTIMIZE_PROGRESSIVE)
		pProgressMesh->SetNumFaces(20);
	return S_OK;
}

HRESULT L3DModel::UpdateMaterial(DWORD uIndex)
{
	m_pMaterial[uIndex].Diffuse.a = m_fAlpha;
	m_p3DDevice->SetMaterial(&m_pMaterial[uIndex]);
	
	return S_OK;
}

HRESULT L3DModel::UpdateTexture(DWORD uIndex)
{
	m_p3DDevice->SetTexture(0, m_ppTexture[uIndex]);
	return S_OK;
}

HRESULT L3DModel::UpdateDraw(DWORD uIndex)
{
	switch (m_ObjectType)
	{
	case LOBJECT_TYPE_INVALID:
		break;
	case LOBJECT_TYPE_VERTEX:
		m_p3DDevice->SetStreamSource(0, m_DisplaySource.LVertex.pVertexBuffer, 0, sizeof(TexVertex));
		m_p3DDevice->SetIndices(m_DisplaySource.LVertex.pIndexBuffer);
		m_p3DDevice->SetFVF(TEX_VERTEX_FVF);

		//m_p3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		m_p3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
		break;
	case LOBJECT_TYPE_X:
		BOOL_ERROR_BREAK(m_DisplaySource.LMesh.pMesh);
		m_DisplaySource.LMesh.pMesh->DrawSubset(uIndex);
		break;
	case LOBJECT_TYPE_MESH:

		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT L3DModel::ResetRendState()
{
	if (m_dwRenderParam & LOBJECT_RENDER_ALPHA)
		m_p3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	return S_OK;
}