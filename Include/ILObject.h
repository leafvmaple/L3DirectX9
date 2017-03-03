#pragma once

#include <d3dx9.h>
#include "LExports.h"

struct IDirect3DDevice9;

struct L3DENGINE_CLASS D3DXVECTOR3;
struct L3DENGINE_CLASS D3DXQUATERNION;
struct L3DENGINE_CLASS D3DXMATRIX;

class L3DENGINE_CLASS ILObject
{
public:
	ILObject();

	virtual HRESULT Setup(IDirect3DDevice9* p3DDevice) = 0;
	virtual HRESULT Display(IDirect3DDevice9* p3DDevice, float fDeltaTime) = 0;
	virtual ~ILObject() {};

public:
	HRESULT SetTranslation(D3DXVECTOR3& vTranslation);
	HRESULT SetRotation(D3DXQUATERNION& qRotation);
	HRESULT UpdateTransform(IDirect3DDevice9* p3DDevice);

protected:
	D3DXVECTOR3 m_vTranslation;
	D3DXQUATERNION m_qRotation;
	D3DXMATRIX m_matTransform;
};