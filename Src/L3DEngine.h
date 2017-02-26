#pragma once

#include <d3d9.h>
#include <vector>

class L3DEngine
{
public:
	struct L3DWINDOWPARAM
	{
		int x;
		int y;
		int Width;
		int Height;
		BOOL bWindow;
		LPCWSTR lpszClassName;
		LPCWSTR lpszWindowName;
	};

public:
	L3DEngine();
	~L3DEngine();

	HRESULT Init(HINSTANCE hInstance, L3DWINDOWPARAM& WindowParam);
	HRESULT Setup();
	HRESULT Run();
	HRESULT Uninit();

private:
	struct ADAPTERMODE
	{
		UINT uWidth;
		UINT uHeight;
		UINT uRefreshRates[32];
	};

	struct Vertex
	{
		Vertex(){}
		Vertex(float x, float y, float z)
		{
			_x = x;
			_y = y;
			_z = z;
		}
		float _x, _y, _z;
		static const DWORD FVF;
	};

	IDirect3D9* m_p3D9;
	IDirect3DDevice9* m_p3DDevice;
	IDirect3DVertexBuffer9* m_pVertexBuffer;
	IDirect3DIndexBuffer9* m_pIndexBuffer;

	float m_fLastTime;
	L3DWINDOWPARAM m_WindowParam;
	std::vector<ADAPTERMODE> m_AdapterModes;

private:
	static LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HRESULT GetL3DAdapter(PUINT puAdapter, D3DDEVTYPE* pDeviceType);
	HRESULT GetL3DAdapterMode(UINT uAdapter);
	HRESULT CreateL3DWindow(HWND* pWnd, HINSTANCE hInstance);
	HRESULT CreateL3DDevice(UINT uAdapter, D3DDEVTYPE eDeviceType, HWND hWnd);

	HRESULT Display(float fDeltaTime);
	HRESULT EnterMsgLoop(HRESULT (L3DEngine::* pfnDisplay)(float fDeltaTime));
};

