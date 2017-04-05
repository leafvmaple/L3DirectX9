#pragma once
#include <d3dx9.h>

class L3DEntity
{
public:
	L3DEntity();
	~L3DEntity();

	HRESULT LoadEntity(LPCWSTR pcszFileName);
};