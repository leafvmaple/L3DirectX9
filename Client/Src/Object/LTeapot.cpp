#include "LTeapot.h"
#include "LAssert.h"

LTeapot::LTeapot()
{
	m_fAngleY = 0;
}

LTeapot::~LTeapot()
{

}

HRESULT LTeapot::Create(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	ID3DXBaseMesh* pMesh = NULL;

	do 
	{
		BOOL_ERROR_BREAK(p3DDevice);
		BOOL_ERROR_BREAK(p3DEngine);

		hr = ILModel::Create(p3DEngine, &pMesh, LOBJECT_MESH_TEAPOT, NULL, &m_pObject);
		HRESULT_ERROR_BREAK(hr);

		hr = m_pObject->SetTranslation(D3DXVECTOR3(-1, -1, -1));
		HRESULT_ERROR_BREAK(hr);

		hr = m_pObject->SetMaterial(L3D::RED_MTL);
		HRESULT_ERROR_BREAK(hr);

		hr = m_pObject->SetAlpha(0.5f);
		HRESULT_ERROR_BREAK(hr);

		//hr = m_pObject->SetTexture("res/texture.png");
		//HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT LTeapot::Display(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice, float fDeltaTime)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	D3DXQUATERNION qRotation;

	do 
	{
		D3DXQuaternionRotationYawPitchRoll(&qRotation, m_fAngleY, 0, 0);
		m_pObject->SetRotation(qRotation);

		m_fAngleY += fDeltaTime;

		hResult = S_OK;
	} while (0);

	return hResult;
}


