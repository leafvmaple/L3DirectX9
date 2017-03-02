#pragma once

#include <windows.h>

struct IDirect3DDevice9;

class IAction
{
public:
	virtual HRESULT Setup(IDirect3DDevice9* p3DDevice) = 0;
	virtual HRESULT Display(IDirect3DDevice9* p3DDevice, float fDeltaTime) = 0;
	virtual ~IAction() {};
};