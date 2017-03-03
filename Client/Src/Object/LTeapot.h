#pragma once

#include "ILObject.h"
#include "L3DEngine.h"
#include "LExports.h"

class L3DENGINE_CLASS ILObject;
class LTeapot : public ILObject
{
public:
	LTeapot();
	~LTeapot();

	virtual HRESULT Setup(IDirect3DDevice9* p3DDevice);
	virtual HRESULT Display(IDirect3DDevice9* p3DDevice, float fDeltaTime);

private:
	ID3DXMesh* m_pMeshTeapot;
	IDirect3DVertexBuffer9* m_pVertexBuffer;

	float m_fAngleY;
};