#pragma once

#include "LModel.h"

class LCube : public LModel
{
public:
	LCube();
	virtual ~LCube();

	virtual HRESULT Setup(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice);
	virtual HRESULT Display(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice, float fDeltaTime);

private:
	float m_fAngleX;
	float m_fAngleY;
};