#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include "L3DInterface.h"

class L3DInput;
class L3DCamera;

class L3DEngine : public IL3DEngine
{
	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	L3DEngine();
	virtual ~L3DEngine();

	virtual HRESULT Init(HINSTANCE hInstance, L3DWINDOWPARAM& WindowParam);
	virtual HRESULT Uninit();
	
	virtual HRESULT Update(float fDeltaTime);

	virtual BOOL IsActive();
	virtual HRESULT GetDevice(IDirect3DDevice9** pp3DDevice);

public:
	HRESULT AttachObject(ILModel* pAction);
	HRESULT AttachFont(ILFont* pFont);

private:
	struct SampFilter
	{
		DWORD dwAnisotropy;
		int nMipFilter;
		int nMinFilter;
		int nMagFilter;
	};
	
	IDirect3D9* m_p3D9;
	IDirect3DDevice9* m_p3DDevice;
	L3DInput* m_pLInput;
	L3DCamera* m_pLCamera;

	L3DWINDOWPARAM m_WindowParam;
	SampFilter m_SampFilter[GRAPHICS_LEVEL_COUNT];
	SampFilter& m_CurSampFilter;

	BOOL m_bActive;
	D3DCAPS9 m_Caps9;
	D3DPRESENT_PARAMETERS m_PresentParam;
	std::vector<ADAPTERMODE> m_AdapterModes;
	std::list<ILModel*> m_ModelList;
	std::list<ILFont*> m_FontList;

private:
	LRESULT	MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HRESULT InitPresentParam(HWND hWnd);
	HRESULT InitSamplerFilter(UINT uAdapter, D3DDEVTYPE eDeviceType);
	HRESULT InitInput(HWND hWnd, HINSTANCE hInstance);
	HRESULT InitCamera(IDirect3DDevice9* p3DDevice, float fWidth, float fHeight);

	HRESULT GetL3DAdapter(PUINT puAdapter, D3DDEVTYPE* pDeviceType);
	HRESULT GetL3DAdapterMode(UINT uAdapter);
	HRESULT CreateL3DWindow(HWND* pWnd, HINSTANCE hInstance);
	HRESULT CreateL3DDevice(UINT uAdapter, D3DDEVTYPE eDeviceType, HWND hWnd);

	HRESULT UpdateMessage();
	HRESULT UpdateInput();
	HRESULT UpdateCamera(float fDeltaTime);
};