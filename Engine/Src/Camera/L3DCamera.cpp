#include "L3DCamera.h"
#include "LAssert.h"
#include "L3DInterface.h"

extern LPDIRECT3DDEVICE9 g_p3DDevice;

L3DCamera::L3DCamera()
: m_fSightDis(0.f)
, m_fYaw(0.f)
, m_fPitch(0.f)
, m_fRoll(0.f)
, m_fWidth(0.f)
, m_fHeight(0.f)
{
	ZeroMemory(&m_vPositon, sizeof(m_vPositon));
	ZeroMemory(&m_vTarget, sizeof(m_vTarget));
	ZeroMemory(&m_vFront, sizeof(m_vFront));

	ZeroMemory(&m_MatrixView, sizeof(m_MatrixView));
	ZeroMemory(&m_MatrixProject, sizeof(m_MatrixProject));
}

L3DCamera::~L3DCamera()
{

}

HRESULT L3DCamera::Init(float fWidth, float fHeight)
{
	m_fSightDis  = 500.f;
	m_vUp        = D3DXVECTOR3(0.f, 1.f, 0.f);
	m_fWidth     = fWidth;
	m_fHeight    = fHeight;

	ComputeViewMatrix();
	ComputePerspectiveMatrix();

	return S_OK;
}

HRESULT L3DCamera::UpdateYawPitchRoll(float fYaw, float fPitch, float fRoll)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		m_fYaw   += fYaw;
		m_fPitch += fPitch;
		m_fRoll  += fRoll;

		hr = ComputeViewMatrix();
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT L3DCamera::UpdateSightDistance(float fSightDis)
{
	m_fSightDis += fSightDis;
	m_fSightDis = max(m_fSightDis, 3.f);
	
	ComputeViewMatrix();

	return S_OK;
}

HRESULT L3DCamera::UpdateCamera()
{
	D3DXMATRIX matCamera;
	D3DXMATRIX matProj;

	do 
	{
		g_p3DDevice->SetTransform(D3DTS_VIEW, &m_MatrixView);
		g_p3DDevice->SetTransform(D3DTS_PROJECTION, &m_MatrixProject);

	} while (0);

	return S_OK;
}

HRESULT L3DCamera::ComputeViewMatrix()
{
	D3DXVECTOR3 vFont(0.f, 0.f, -m_fSightDis);
	D3DXMATRIX matCamera;

	D3DXMatrixRotationYawPitchRoll(&matCamera, m_fYaw, m_fPitch, m_fRoll);
	D3DXVec3TransformNormal(&m_vFront, &vFont, &matCamera);
	m_vPositon = m_vTarget + m_vFront;

	D3DXMatrixLookAtLH(&m_MatrixView, &m_vPositon, &m_vTarget, &m_vUp);
	return S_OK;
}

HRESULT L3DCamera::ComputePerspectiveMatrix()
{
	D3DXMatrixPerspectiveFovLH(&m_MatrixProject, D3DX_PI * 0.5f, m_fWidth / m_fHeight, 1.0f, 100000.0f);
	return S_OK;
}
