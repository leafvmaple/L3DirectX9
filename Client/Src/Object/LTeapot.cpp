#include "LTeapot.h"
#include "LAssert.h"

LTeapot::LTeapot()
{
	m_fAngleY = 0;
}

LTeapot::~LTeapot()
{

}

HRESULT LTeapot::Setup(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	ID3DXMesh* pMesh = NULL;
	IDirect3DTexture9* pTexture = NULL;

	do 
	{
		BOOL_ERROR_BREAK(p3DDevice);
		BOOL_ERROR_BREAK(p3DEngine);

		hr = CreateLObject(p3DEngine, &m_pObject);
		HRESULT_ERROR_BREAK(hr);

		hr = m_pObject->SetTranslation(D3DXVECTOR3(-1, -1, -1));
		HRESULT_ERROR_BREAK(hr);

		hr = m_pObject->CreateMesh(p3DDevice, &pMesh);

		D3DXCreateTextureFromFile(p3DDevice, TEXT("res/texture.png"), &pTexture);
		BOOL_ERROR_BREAK(pTexture);

		hr = p3DDevice->SetTexture(0, pTexture);
		HRESULT_ERROR_BREAK(hr);

		p3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		p3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		p3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

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

