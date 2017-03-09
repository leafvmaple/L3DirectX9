#pragma once

#include <d3dx9.h>
#include <list>
#include "LAssert.h"
#include "Object/LCube.h"
#include "Object/LTeapot.h"

class LModel;
class IL3DEngine;

class LObjectMgr
{
public:
	LObjectMgr();
	~LObjectMgr();

	HRESULT Init(HINSTANCE hInstance, L3DWINDOWPARAM& WindowParam);
	HRESULT Uninit();

	template<typename T>
	HRESULT CreateModel()
	{
		LModel* pObject = NULL;
		pObject = new T;
		m_ObjectList.push_back(pObject);
		return S_OK;
	}

	HRESULT Setup();
	HRESULT Update(float fDeltaTime);

	BOOL IsActive();
	IL3DEngine* GetEngine() const;

private:
	IL3DEngine* m_p3DEngine;
	IDirect3DDevice9* m_p3DDevice;
	std::list<LModel*> m_ObjectList;
	
	ILFont* m_pFont;
};
