#pragma once

#include "Engine/IAction.h"
#include "Engine/L3DEngine.h"

class Teapot : public IAction
{
public:
	Teapot();
	~Teapot();

	virtual HRESULT Setup(IDirect3DDevice9* p3DDevice);
	virtual HRESULT Display(IDirect3DDevice9* p3DDevice, float fDeltaTime);

private:
	ID3DXMesh* m_pMeshTeapot;
	IDirect3DVertexBuffer9* m_pVertexBuffer;

	float m_fAngleY;
};