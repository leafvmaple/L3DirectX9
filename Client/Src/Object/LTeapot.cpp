#include "LTeapot.h"
#include "LAssert.h"

LTeapot::LTeapot()
{
	m_pMeshTeapot = NULL;
	m_pVertexBuffer = NULL;
	m_fAngleY = 0;
}

LTeapot::~LTeapot()
{

}

HRESULT LTeapot::Setup(IDirect3DDevice9* p3DDevice)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	IDirect3DTexture9* pTexture = NULL;

	do 
	{
		BOOL_ERROR_BREAK(p3DDevice);

		hr = D3DXCreateTeapot(p3DDevice, &m_pMeshTeapot, 0);
		HRESULT_ERROR_BREAK(hr);

		hr = m_pMeshTeapot->GetVertexBuffer(&m_pVertexBuffer);
		HRESULT_ERROR_BREAK(hr);

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

HRESULT LTeapot::Display(IDirect3DDevice9* p3DDevice, float fDeltaTime)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	D3DXQUATERNION qRotation;

	do 
	{
		D3DXQuaternionRotationYawPitchRoll(&qRotation, m_fAngleY, 0, 0);
		SetRotation(qRotation);

		hr = UpdateTransform(p3DDevice);
		HRESULT_ERROR_BREAK(hr);

		m_fAngleY += fDeltaTime;

		hr = m_pMeshTeapot->DrawSubset(0);
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);

	return hResult;
}


