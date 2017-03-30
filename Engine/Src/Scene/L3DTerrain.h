#pragma once
#include <d3dx9.h>

class L3DTerrain
{
public:
	L3DTerrain();
	~L3DTerrain();

	HRESULT LoadTerrain(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR cszFileName);

private:
	LPDIRECT3DDEVICE9 m_p3DDevice;
	BOOL m_bEnableTerrainConverMap;
};