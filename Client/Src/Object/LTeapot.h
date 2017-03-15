#pragma once

#include "LModel.h"

class LTeapot : public LModel
{
public:
	LTeapot();
	virtual ~LTeapot();
	virtual HRESULT Display(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice, float fDeltaTime);

public:
	HRESULT Create(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice);

private:
	float m_fAngleY;
};