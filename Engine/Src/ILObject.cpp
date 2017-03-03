#include "ILObject.h"

ILObject::ILObject()
{
	ZeroMemory(m_vTranslation, sizeof(m_vTranslation));
	ZeroMemory(m_qRotation, sizeof(m_qRotation));
	ZeroMemory(m_matTransform, sizeof(m_matTransform));
}

HRESULT ILObject::SetTranslation(D3DXVECTOR3& vTranslation)
{
	m_vTranslation = vTranslation;
	return S_OK;
}

HRESULT ILObject::SetRotation(D3DXQUATERNION& qRotation)
{
	m_qRotation = qRotation;
	return S_OK;
}

HRESULT ILObject::UpdateTransform(IDirect3DDevice9* p3DDevice)
{
	D3DXMatrixTransformation(&m_matTransform, NULL, NULL, NULL, NULL, &m_qRotation, &m_vTranslation);
	p3DDevice->SetTransform(D3DTS_WORLD, &m_matTransform);
	return S_OK;
}