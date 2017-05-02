#pragma once
#include <d3dx9.h>

class L3DCamera
{
public:
	L3DCamera();
	~L3DCamera();

	HRESULT Init(float fWidth, float fHeight);
	HRESULT UpdateYawPitchRoll(float fYaw, float fPitch, float fRoll);
	HRESULT UpdateSightDistance(float fSightDis);

	HRESULT UpdateCamera();

private:
	float m_fSightDis;
	float m_fYaw;
	float m_fPitch;
	float m_fRoll;
	float m_fWidth;
	float m_fHeight;
	D3DXVECTOR3 m_vPositon;
	D3DXVECTOR3 m_vTarget;
	D3DXVECTOR3 m_vUp;
	D3DXVECTOR3 m_vFront;
	
	D3DXMATRIX m_MatrixView;
	D3DXMATRIX m_MatrixProject;

private:
	HRESULT ComputeViewMatrix();
	HRESULT ComputePerspectiveMatrix();
};