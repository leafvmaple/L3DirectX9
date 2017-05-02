#pragma once
#include <d3dx9.h>
#include <map>

struct LSceneDataClip;

class L3DEntity
{
public:
	L3DEntity();
	~L3DEntity();

	HRESULT LoadEntity(LPCWSTR pcszFileName);

private:
	std::map<UINT, LSceneDataClip*>  m_EntityInformations;

	HRESULT LoadEntityInfo(LPCWSTR cszDirectory);
};