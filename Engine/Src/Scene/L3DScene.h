#pragma once
#include "L3DInterface.h"

class L3DTerrain;

class L3DScene : public ILScene
{
public:
	L3DScene();
	virtual ~L3DScene();

	virtual HRESULT LoadScene(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR pcszFileName);

	L3DTerrain* m_pLTerrain;
};