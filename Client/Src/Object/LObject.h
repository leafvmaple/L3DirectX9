#pragma once

#include "L3DInterface.h"
#include "LExports.h"

class L3DENGINE_CLASS ILObject;

class LObject
{
public:
	LObject() { m_pObject = NULL; };
	virtual ~LObject() {};

	virtual HRESULT Setup(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice) = 0;
	virtual HRESULT Display(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice, float fDeltaTime) = 0;

protected:
	ILObject* m_pObject;
};