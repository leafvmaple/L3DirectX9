#include "Teapot.h"
#include "Engine/LAssert.h"

Teapot::Teapot()
{
	m_pMeshTeapot = NULL;
	m_fAngleY = 0;
}

Teapot::~Teapot()
{

}

HRESULT Teapot::Setup(IDirect3DDevice9* p3DDevice)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		BOOL_ERROR_BREAK(p3DDevice);

		hr = D3DXCreateTeapot(p3DDevice, &m_pMeshTeapot, 0);
		HRESULT_ERROR_BREAK(hr)

		p3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT Teapot::Display(IDirect3DDevice9* p3DDevice, float fDeltaTime)
{
	D3DXMATRIX matY;
	D3DXMatrixRotationY(&matY, m_fAngleY);

	m_fAngleY += fDeltaTime;
	if(m_fAngleY >= L3DX_2PI)
		m_fAngleY = 0.0f;

	p3DDevice->SetTransform(D3DTS_WORLD, &matY);

	p3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	p3DDevice->BeginScene();

	m_pMeshTeapot->DrawSubset(0);

	p3DDevice->EndScene();
	p3DDevice->Present(0, 0, 0, 0);

	return S_OK;
}


