#pragma once
#include <d3dx9.h>
#include <map>

struct LSceneDataClip;

class L3DEntity
{
public:
	L3DEntity();
	~L3DEntity();

	HRESULT LoadEntity(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR pcszFileName);

private:
	LPDIRECT3DDEVICE9 m_p3DDevice;
	std::map<UINT, LSceneDataClip*>  m_EntityInformations;

	HRESULT LoadEntityInfo(LPCWSTR cszDirectory);
};