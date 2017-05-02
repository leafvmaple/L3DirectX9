#pragma once
#include "L3DInterface.h"

class L3DENGINE_CLASS ILScene;

class LScene
{
public:
	LScene();
	~LScene();

	HRESULT Create(IL3DEngine* p3DEngine, LPDIRECT3DDEVICE9 p3DDevice, TCHAR* pwszMeshPath);
	HRESULT Display(IL3DEngine* p3DEngine, LPDIRECT3DDEVICE9 p3DDevice, float fDeltaTime);

protected:
	ILScene* m_LpScene;
};