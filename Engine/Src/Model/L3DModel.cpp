#include <atlconv.h>
#include "L3DModel.h"
#include "LAssert.h"
#include "LCommon.h"
#include "IO/LFileReader.h"
#include "L3DMesh.h"
#include "L3DMaterial.h"
#include "L3DParticle.h"

extern LPDIRECT3DDEVICE9 g_p3DDevice;

L3DModel::L3DModel()
: m_pAdjBuffer(NULL)
, m_pMaterial(NULL)
, m_ppTexture(NULL)
, m_dwSubsetCount(0)
, m_ObjectType(LOBJECT_TYPE_INVALID)
, m_fAlpha(1.f)
, m_fScale(1.f)
, m_dwRenderParam(0)
, m_dwOptimizeParam(0)
, m_pLMesh(NULL)
, m_pLMaterial(NULL)
, m_pLTexture(NULL)
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
	case LOBJECT_TYPE_MESH:
		if (m_DisplaySource.LMesh.pMesh)
			m_DisplaySource.LMesh.pMesh->Release();
		break;
	default:
		break;
	}
}

HRESULT L3DModel::Init(TexVertex* pModelVertices, UINT nVerticesCount, WORD* pwModelIndices, UINT nIndicesCount)
{
	HRESULT hr      = E_FAIL;
	HRESULT hResult = E_FAIL;
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer = NULL;
	LPDIRECT3DINDEXBUFFER9  pIndexBuffer  = NULL;
	TexVertex* pVertices = NULL;
	WORD*      pwIndices = NULL;

    hr = g_p3DDevice->CreateVertexBuffer(
        nVerticesCount, D3DUSAGE_WRITEONLY,
        TEX_VERTEX_FVF, D3DPOOL_MANAGED, &pVertexBuffer, 0);
    HRESULT_ERROR_EXIT(hr);

    hr = g_p3DDevice->CreateIndexBuffer(
        nIndicesCount, D3DUSAGE_WRITEONLY,
        D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIndexBuffer, 0);
    HRESULT_ERROR_EXIT(hr);

    pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
    memcpy_s(pVertices, nVerticesCount, pModelVertices, nVerticesCount);
    pVertexBuffer->Unlock();

    pIndexBuffer->Lock(0, 0, (void**)&pwIndices, 0);
    memcpy_s(pwIndices, nIndicesCount, pwModelIndices, nIndicesCount);
    pIndexBuffer->Unlock();

    m_ObjectType = LOBJECT_TYPE_VERTEX;
    m_dwSubsetCount = 1;
    m_DisplaySource.LVertex.pVertexBuffer = pVertexBuffer;
    m_DisplaySource.LVertex.pIndexBuffer = pIndexBuffer;

    m_pMaterial = new D3DMATERIAL9[m_dwSubsetCount];
    BOOL_ERROR_EXIT(m_pMaterial);

    m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCount];
    BOOL_ERROR_EXIT(m_ppTexture);

    ZeroMemory(m_pMaterial, sizeof(D3DMATERIAL9) * m_dwSubsetCount);
    ZeroMemory(m_ppTexture, sizeof(LPDIRECT3DTEXTURE9) * m_dwSubsetCount);

    hResult = S_OK;
Exit0:
	return hResult;
}

HRESULT L3DModel::Init(LOBJECT_MESH_TYPE eModelType, LPCWSTR pcszFileName)
{
	HRESULT hr      = E_FAIL;
	HRESULT hResult = E_FAIL;
	LPD3DXBUFFER   pMtlBuffer    = NULL;
	LPD3DXMATERIAL pMtls         = NULL;
	ID3DXPMesh*    pProgressMesh = NULL;
	ID3DXMesh*     pCloneMesh    = NULL;
	ID3DXMesh*     pMesh         = NULL;
	size_t uDirLength = 0;
	WCHAR  wcszDir[LENGIEN_FILENAME_MAX];

	switch (eModelType)
	{
	case LOBJECT_MESH_TEAPOT:
		hr = D3DXCreateTeapot(g_p3DDevice, &pMesh, &m_pAdjBuffer);
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
		hr = D3DXLoadMeshFromX(pcszFileName, D3DXMESH_MANAGED, g_p3DDevice, &m_pAdjBuffer, &pMtlBuffer, NULL, &m_dwSubsetCount, &pMesh);
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

				hr = D3DXCreateTextureFromFile(g_p3DDevice, wcszDir, &m_ppTexture[u]);
				HRESULT_ERROR_BREAK(hr);

				m_dwRenderParam |= LOBJECT_RENDER_TEXTURE;
			}
			m_dwRenderParam |= LOBJECT_RENDER_MATERIAL;
		}

		SAFE_RELEASE(pMtlBuffer);

	case LOBJECT_MESH_LX:
		{
			hr = LoadModel(pcszFileName);
			HRESULT_ERROR_BREAK(hr);

			m_dwSubsetCount = m_pLMesh->GetSubsetCount();
		}
			
		break;
	default:
		break;
	}

	/*if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
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
			m_dwOptimizeParam |= LOBJECT_OPTIMIZE_PROGRESSIVE;
		}
		else
		{
			SAFE_RELEASE(pProgressMesh);
		}
	}*/

	m_ObjectType = LOBJECT_TYPE_MESH;
	m_DisplaySource.LMesh.pMesh = pMesh;

	hResult = S_OK;
Exit0:
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
		D3DXCreateTextureFromFile(g_p3DDevice, szTexture, &m_ppTexture[u]);
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

HRESULT L3DModel::LoadModel(LPCWSTR cszFileName)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

    const LoadModelFunc* pMeshFunc = GetLoadModelFunc(cszFileName);
    BOOL_ERROR_EXIT(pMeshFunc);

    hr = (this->*(pMeshFunc->fnLoadMesh))(cszFileName);
    HRESULT_ERROR_EXIT(hr);
Exit0:
	return S_OK;
}

HRESULT L3DModel::LoadXMesh(LPCWSTR cszFileName)
{
	HRESULT hr = E_FAIL;;
	HRESULT hResult = E_FAIL;

    m_pLMesh = new L3DMesh;
    BOOL_ERROR_EXIT(m_pLMesh);

    hr = m_pLMesh->LoadXMesh(cszFileName);
    HRESULT_ERROR_EXIT(hr);

    hResult = S_OK;
Exit0:
	return hResult;
}

HRESULT L3DModel::LoadLMesh(LPCWSTR cszFileName)
{
	HRESULT hr = E_FAIL;;
	HRESULT hResult = E_FAIL;

    m_pLMesh = new L3DMesh;
    BOOL_ERROR_EXIT(m_pLMesh);

    hr = m_pLMesh->LoadLMesh(cszFileName);
    HRESULT_ERROR_EXIT(hr);

    hResult = S_OK;
Exit0:
	return hResult;
}

HRESULT L3DModel::LoadLMaterial(LPCWSTR cszFileName)
{
	HRESULT hr = E_FAIL;;
	HRESULT hResult = E_FAIL;

	m_pLMaterial = new L3DMaterial;
	BOOL_ERROR_EXIT(m_pLMaterial);

	hr = m_pLMaterial->LoadLMaterial(cszFileName);
	HRESULT_ERROR_EXIT(hr);

	hResult = S_OK;
Exit0:
	return hResult;
}

HRESULT L3DModel::LoadLTexture(LPCWSTR cszFileName)
{
	HRESULT hr = E_FAIL;;
	HRESULT hResult = E_FAIL;

    m_pLTexture = new L3DTexture;
    BOOL_ERROR_EXIT(m_pLMaterial);

    hr = m_pLTexture->LoadLTexture(cszFileName);
    HRESULT_ERROR_EXIT(hr);

    hResult = S_OK;
Exit0:
	return hResult;
}

HRESULT L3DModel::LoadLParticle(LPCWSTR cszFileName)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

    m_pLParticle = new L3DParticle;
    BOOL_ERROR_EXIT(m_pLParticle);

    hr = m_pLParticle->LoadParticle(cszFileName);
    HRESULT_ERROR_EXIT(hr);
Exit0:
	return S_OK;
}

HRESULT L3DModel::UpdateDisplay()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

    hr = UpdateRenderState();
    HRESULT_ERROR_EXIT(hr);

    hr = UpdateTransform();
    HRESULT_ERROR_EXIT(hr);

    hr = UpdateLOD();
    HRESULT_ERROR_EXIT(hr);

    for (DWORD u = 0; u < m_dwSubsetCount; u++)
    {
        hr = UpdateMesh(u);
        HRESULT_ERROR_CONTINUE(hr);
    }

    hr = ResetRendState();
    HRESULT_ERROR_EXIT(hr);

    hResult = S_OK;
Exit0:
	return S_OK;
}

HRESULT L3DModel::UpdateRenderState()
{
	if (m_dwRenderParam & LOBJECT_RENDER_ALPHA)
		g_p3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	return S_OK;
}

HRESULT L3DModel::UpdateTransform()
{
	D3DXMatrixTransformation(&m_matTransform, NULL, NULL, &D3DXVECTOR3(m_fScale, m_fScale, m_fScale), NULL, &m_qRotation, &m_vTranslation);
	g_p3DDevice->SetTransform(D3DTS_WORLD, &m_matTransform);
	return S_OK;
}

HRESULT L3DModel::UpdateLOD()
{
	ID3DXPMesh* pProgressMesh = static_cast<ID3DXPMesh*>(m_DisplaySource.LMesh.pMesh);
	if (m_dwOptimizeParam & LOBJECT_OPTIMIZE_PROGRESSIVE)
		pProgressMesh->SetNumFaces(20);
	return S_OK;
}


HRESULT L3DModel::UpdateMesh(DWORD uIndex)
{
	if (m_pLMesh)
		m_pLMesh->UpdateMesh(uIndex);
	return S_OK;
}

HRESULT L3DModel::UpdateMaterial(DWORD uIndex)
{
	//m_pMaterial[uIndex].Diffuse.a = m_fAlpha;
	//m_p3DDevice->SetMaterial(&m_pMaterial[uIndex]);
	if (m_pLMaterial)
		m_pLMaterial->UpdateMaterial(uIndex);
	return S_OK;
}

HRESULT L3DModel::UpdateDraw(DWORD uIndex)
{
	switch (m_ObjectType)
	{
	case LOBJECT_TYPE_INVALID:
		break;
	case LOBJECT_TYPE_VERTEX:
		g_p3DDevice->SetStreamSource(0, m_DisplaySource.LVertex.pVertexBuffer, 0, sizeof(TexVertex));
		g_p3DDevice->SetIndices(m_DisplaySource.LVertex.pIndexBuffer);
		g_p3DDevice->SetFVF(TEX_VERTEX_FVF);

		//m_p3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		g_p3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
		break;
	case LOBJECT_TYPE_MESH:
		BOOL_ERROR_BREAK(m_pLMesh);
		m_pLMesh->UpdateMesh(0);
		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT L3DModel::ResetRendState()
{
	if (m_dwRenderParam & LOBJECT_RENDER_ALPHA)
		g_p3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	return S_OK;
}

const L3DModel::LoadModelFunc* L3DModel::GetLoadModelFunc(LPCWSTR cszFileName)
{
	TCHAR wszExt[FILENAME_MAX];
	_wsplitpath_s(cszFileName, NULL, 0, NULL, 0, NULL, 0, wszExt, FILENAME_MAX);

	const LoadModelFunc *pReturn = NULL;
	size_t uSize = sizeof(s_ModelLoadFunc) / sizeof(LoadModelFunc);
	size_t i = 0;
	for (; i < uSize; i++)
	{
		if (!wcscmp(s_ModelLoadFunc[i].pwcsFileExt, wszExt))
			break;
	}
	BOOL_ERROR_RETURN(i != uSize);

	pReturn = &s_ModelLoadFunc[i];
	return pReturn;
}
