#include "L3DObject.h"
#include "LAssert.h"

L3DObject::L3DObject()
: m_ObjectType(LOBJECT_TYPE_INVALID)
{
	ZeroMemory(&m_DisplaySource, sizeof(m_DisplaySource));
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

HRESULT L3DObject::SetTranslation(D3DXVECTOR3& vTranslation)
{
	m_vTranslation = vTranslation;
	return S_OK;
}

HRESULT L3DObject::SetRotation(D3DXQUATERNION& qRotation)
{
	m_qRotation = qRotation;
	return S_OK;
}

HRESULT L3DObject::UpdateTransform(IDirect3DDevice9* p3DDevice)
{
	D3DXMatrixTransformation(&m_matTransform, NULL, NULL, NULL, NULL, &m_qRotation, &m_vTranslation);
	p3DDevice->SetTransform(D3DTS_WORLD, &m_matTransform);
	return S_OK;
}

HRESULT L3DObject::UpdateDisplay(IDirect3DDevice9* p3DDevice)
{
	switch (m_ObjectType)
	{
	case LOBJECT_TYPE_INVALID:
		break;
	case LOBJECT_TYPE_VERTEX:
		p3DDevice->SetStreamSource(0, m_DisplaySource.LVertex.pVertexBuffer, 0, sizeof(TexVertex));
		p3DDevice->SetIndices(m_DisplaySource.LVertex.pIndexBuffer);
		p3DDevice->SetFVF(TEX_VERTEX_FVF);

		//m_p3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		p3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
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

		m_ObjectType = LOBJECT_TYPE_MESH;
		m_DisplaySource.LMesh.pMesh = *ppMesh;

		hResult = S_OK;
	} while (0);

	return hResult;
}