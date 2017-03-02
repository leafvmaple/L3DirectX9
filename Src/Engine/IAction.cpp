#include "IAction.h"
#include "LAssert.h"

IAction::IAction()
{
	ZeroMemory(m_vTranslation, sizeof(m_vTranslation));
	ZeroMemory(m_qRotation, sizeof(m_qRotation));
	ZeroMemory(m_matTransform, sizeof(m_matTransform));
}

HRESULT IAction::SetTranslation(D3DXVECTOR3& vTranslation)
{
	m_vTranslation = vTranslation;
	return S_OK;
}

HRESULT IAction::SetRotation(D3DXQUATERNION& qRotation)
{
	m_qRotation = qRotation;
	return S_OK;
}

HRESULT IAction::UpdateTransform(IDirect3DDevice9* p3DDevice)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		BOOL_ERROR_BREAK(p3DDevice);

		D3DXMatrixTransformation(&m_matTransform, NULL, NULL, NULL, NULL, &m_qRotation, &m_vTranslation);

		hr = p3DDevice->SetTransform(D3DTS_WORLD, &m_matTransform);
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);
	
	return hResult;
}


