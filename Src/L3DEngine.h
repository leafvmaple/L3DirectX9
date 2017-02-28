#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

namespace L3D
{
	const D3DXCOLOR WHITE  (D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR BLACK  (D3DCOLOR_XRGB(0,   0,   0));
	const D3DXCOLOR RED    (D3DCOLOR_XRGB(255, 0,   0));
	const D3DXCOLOR GREEN  (D3DCOLOR_XRGB(0,   255, 0));
	const D3DXCOLOR BLUE   (D3DCOLOR_XRGB(0,   0,   255));
	const D3DXCOLOR YELLOW (D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR CYAN   (D3DCOLOR_XRGB(0,   255, 255));
	const D3DXCOLOR MAGENTA(D3DCOLOR_XRGB(255, 0,   255));

	const D3DMATERIAL9 WHITE_MTL  = {WHITE,  WHITE,  WHITE,  BLACK, 8.0f};
	const D3DMATERIAL9 RED_MTL    = {RED,    RED,    RED,    BLACK, 8.0f};
	const D3DMATERIAL9 GREEN_MTL  = {GREEN,  GREEN,  GREEN,  BLACK, 8.0f};
	const D3DMATERIAL9 BLUE_MTL   = {BLUE,   BLUE,   BLUE,   BLACK, 8.0f};
	const D3DMATERIAL9 YELLOW_MTL = {YELLOW, YELLOW, YELLOW, BLACK, 8.0f};
}

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
	static DWORD VERTEX_FVF;
};

struct ColorVertex : Vertex
{
	ColorVertex(){}
	ColorVertex(float x, float y, float z, D3DCOLOR color) : Vertex(x, y, z)
	{
		_color = color;
	}
	D3DCOLOR _color;
	static DWORD COLOR_VERTEX_FVF;
};

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

	IDirect3D9* m_p3D9;
	IDirect3DDevice9* m_p3DDevice;
	IDirect3DVertexBuffer9* m_pVertexBuffer;
	IDirect3DIndexBuffer9* m_pIndexBuffer;

	float m_fLastTime;
	float m_fAngleX;
	float m_fAngleY;
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