#pragma once

#include <d3dx9.h>
#include <list>
#include "LAssert.h"
#include "Object/LCube.h"
#include "Object/LTeapot.h"

class LModel;
class LScene;
class IL3DEngine;

class LObjectMgr
{
public:
	LObjectMgr();
	~LObjectMgr();

	HRESULT Init(HINSTANCE hInstance, L3DWINDOWPARAM& WindowParam);
	HRESULT Uninit();

	template<typename T>
	T* CreateModel(TCHAR* pwcsMeshPath)
	{
		T* pObject = NULL;
		pObject = new T(pwcsMeshPath);
		m_ObjectList.push_back(pObject);
		pObject->Create(IL3DEngine::Instance(), m_p3DDevice);
		return pObject;
	}

	template<typename T>
	T* CreateModel()
	{
		T* pObject = NULL;
		pObject = new T;
		m_ObjectList.push_back(pObject);
		pObject->Create(IL3DEngine::Instance(), m_p3DDevice);
		return pObject;
	}

	LScene* CreateScene(TCHAR* pwcsMeshPath);

	HRESULT Update(float fDeltaTime);

	BOOL IsActive();

private:
	IDirect3DDevice9* m_p3DDevice;
	std::list<LModel*> m_ObjectList;
	std::list<LScene*> m_SceneList;
	
	ILFont* m_pFont;
};
