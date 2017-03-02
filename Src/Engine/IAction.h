#pragma once

#include <d3dx9.h>

struct IDirect3DDevice9;

class IAction
{
public:
	IAction();

	virtual HRESULT Setup(IDirect3DDevice9* p3DDevice) = 0;
	virtual HRESULT Display(IDirect3DDevice9* p3DDevice, float fDeltaTime) = 0;
	virtual ~IAction() {};

public:
	HRESULT SetTranslation(D3DXVECTOR3& vTranslation);
	HRESULT SetRotation(D3DXQUATERNION& qRotation);
	HRESULT UpdateTransform(IDirect3DDevice9* p3DDevice);

protected:
	D3DXVECTOR3 m_vTranslation;
	D3DXQUATERNION m_qRotation;
	D3DXMATRIX m_matTransform;
};