#pragma once

#include "Engine/IAction.h"
#include "Engine/L3DEngine.h"

class Cube : public IAction
{
public:
	Cube();
	~Cube();

	virtual HRESULT Setup(IDirect3DDevice9* p3DDevice);
	virtual HRESULT Display(IDirect3DDevice9* p3DDevice, float fDeltaTime);

private:
	IDirect3DVertexBuffer9* m_pVertexBuffer;
	IDirect3DIndexBuffer9* m_pIndexBuffer;

	float m_fAngleX;
	float m_fAngleY;
};