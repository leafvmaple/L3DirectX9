#pragma once

#include "ILObject.h"
#include "L3DEngine.h"
#include "LExports.h"

class L3DENGINE_CLASS ILObject;
class LCube : public ILObject
{
public:
	LCube();
	~LCube();

	virtual HRESULT Setup(IDirect3DDevice9* p3DDevice);
	virtual HRESULT Display(IDirect3DDevice9* p3DDevice, float fDeltaTime);

private:
	IDirect3DVertexBuffer9* m_pVertexBuffer;
	IDirect3DIndexBuffer9* m_pIndexBuffer;

	float m_fAngleX;
	float m_fAngleY;
};