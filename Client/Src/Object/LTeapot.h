#pragma once

#include "LObject.h"

class L3DENGINE_CLASS ILObject;
class LTeapot : public LObject
{
public:
	LTeapot();
	virtual ~LTeapot();

	virtual HRESULT Setup(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice);
	virtual HRESULT Display(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice, float fDeltaTime);

private:
	float m_fAngleY;
};