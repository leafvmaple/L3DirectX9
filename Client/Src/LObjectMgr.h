#pragma once

#include <d3dx9.h>
#include <list>
#include "LAssert.h"
#include "Object/LCube.h"
#include "Object/LTeapot.h"

class LObject;
class IL3DEngine;

class LObjectMgr
{
public:
	LObjectMgr();
	~LObjectMgr();

	HRESULT Init(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice);
	HRESULT Uninit();

	template<typename T>
	HRESULT CreateObject()
	{
		LObject* pObject = NULL;
		pObject = new T;
		m_ObjectList.push_back(pObject);
		return S_OK;
	}

	HRESULT Setup();
	HRESULT Active();

private:
	float m_fLastTime;
	IL3DEngine* m_p3DEngine;
	IDirect3DDevice9* m_p3DDevice;
	std::list<LObject*> m_ObjectList;
};
