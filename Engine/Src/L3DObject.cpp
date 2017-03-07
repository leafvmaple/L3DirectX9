#include "L3DObject.h"
#include "LAssert.h"

L3DObject::L3DObject()
: m_p3DDevice(NULL)
, m_pTexture(NULL)
, m_ObjectType(LOBJECT_TYPE_INVALID)
, m_fAlpha(1.f)
, m_fScale(1.f)
, m_dwRenderParam(0)
{
	ZeroMemory(&m_DisplaySource, sizeof(m_DisplaySource));
	ZeroMemory(&m_Material, sizeof(m_Material));
	ZeroMemory(m_vTranslation, sizeof(m_vTranslation));
	ZeroMemory(m_qRotation, sizeof(m_qRotation));
	ZeroMemory(m_matTransform, sizeof(m_matTransform));
}

L3DObject::~L3DObject()
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

HRESULT L3DObject::CreateVertex(IDirect3DDevice9* p3DDevice, IDirect3DVertexBuffer9** ppVertexBuffer, IDirect3DIndexBuffer9** ppIndexBuffer)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		hr = p3DDevice->CreateVertexBuffer(
			8 * sizeof(TexVertex), D3DUSAGE_WRITEONLY,
			TEX_VERTEX_FVF , D3DPOOL_MANAGED, ppVertexBuffer, 0);
		HRESULT_ERROR_BREAK(hr);

		p3DDevice->CreateIndexBuffer(
			36 * sizeof(WORD), D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16, D3DPOOL_MANAGED, ppIndexBuffer, 0);
		HRESULT_ERROR_BREAK(hr);

		m_p3DDevice = p3DDevice;
		m_ObjectType = LOBJECT_TYPE_VERTEX;
		m_DisplaySource.LVertex.pVertexBuffer = *ppVertexBuffer;
		m_DisplaySource.LVertex.pIndexBuffer = *ppIndexBuffer;

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT L3DObject::CreateMesh(IDirect3DDevice9* p3DDevice, ID3DXMesh** ppMesh)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		hr = D3DXCreateTeapot(p3DDevice, ppMesh, 0);
		HRESULT_ERROR_BREAK(hr);

		m_p3DDevice = p3DDevice;
		m_ObjectType = LOBJECT_TYPE_MESH;
		m_DisplaySource.LMesh.pMesh = *ppMesh;

		hResult = S_OK;
	} while (0);

	return hResult;
}


HRESULT L3DObject::SetAlpha(float fAlpha)
{
	m_fAlpha = fAlpha;
	m_dwRenderParam |= LOBJECT_RENDER_ALPHA;
	return S_OK;
}

HRESULT L3DObject::SetScale(float fScale)
{
	m_fScale = fScale;
	return S_OK;
}

HRESULT L3DObject::SetTexture(LPCSTR szTexture)
{
	D3DXCreateTextureFromFile(m_p3DDevice, TEXT("res/texture.png"), &m_pTexture);
	m_dwRenderParam |= LOBJECT_RENDER_TEXTURE;
	return S_OK;
}

HRESULT L3DObject::SetMaterial(const D3DMATERIAL9& Material)
{
	m_Material = Material;
	m_dwRenderParam |= LOBJECT_RENDER_MATERIAL;
	return S_OK;
}

HRESULT L3DObject::SetTranslation(const D3DXVECTOR3& vTranslation)
{
	m_vTranslation = vTranslation;
	m_dwRenderParam |= LOBJECT_RENDER_TRANSFORM;
	return S_OK;
}

HRESULT L3DObject::SetRotation(const D3DXQUATERNION& qRotation)
{
	m_qRotation = qRotation;
	m_dwRenderParam |= LOBJECT_RENDER_TRANSFORM;
	return S_OK;
}

HRESULT L3DObject::UpdateDisplay()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		hr = UpdateRenderState();
		HRESULT_ERROR_BREAK(hr);

		hr = UpdateMaterial();
		HRESULT_ERROR_BREAK(hr);

		hr = UpdateTexture();
		HRESULT_ERROR_BREAK(hr);

		hr = UpdateTransform();
		HRESULT_ERROR_BREAK(hr);

		hr = UpdateDraw();
		HRESULT_ERROR_BREAK(hr);

		hr = ResetRendState();
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);

	return S_OK;
}

HRESULT L3DObject::UpdateRenderState()
{
	if (m_dwRenderParam & LOBJECT_RENDER_ALPHA)
		m_p3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	return S_OK;
}

HRESULT L3DObject::UpdateMaterial()
{
	m_Material.Diffuse.a = m_fAlpha;
	m_p3DDevice->SetMaterial(&m_Material);
	return S_OK;
}

HRESULT L3DObject::UpdateTexture()
{
	m_p3DDevice->SetTexture(0, m_pTexture);
	return S_OK;
}

HRESULT L3DObject::UpdateTransform()
{
	D3DXMatrixTransformation(&m_matTransform, NULL, NULL, &D3DXVECTOR3(m_fScale, m_fScale, m_fScale), NULL, &m_qRotation, &m_vTranslation);
	m_p3DDevice->SetTransform(D3DTS_WORLD, &m_matTransform);
	return S_OK;
}

HRESULT L3DObject::UpdateDraw()
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
	case LOBJECT_TYPE_MESH:
		BOOL_ERROR_BREAK(m_DisplaySource.LMesh.pMesh);
		m_DisplaySource.LMesh.pMesh->DrawSubset(0);
		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT L3DObject::ResetRendState()
{
	if (m_dwRenderParam & LOBJECT_RENDER_ALPHA)
		m_p3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	return S_OK;
}