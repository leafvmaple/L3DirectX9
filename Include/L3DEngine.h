#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include "LExports.h"

#define L3DX_2PI    ((FLOAT)  6.283185308f)

#define GRAPHICS_LEVEL_COUNT 2
#define GRAPHICS_LEVEL_MIN 0
#define GRAPHICS_LEVEL_MAX 1

#define VERTEX_FVF       D3DFVF_XYZ
#define LIGHT_VERTEX_FVF VERTEX_FVF       | D3DFVF_NORMAL
#define COLOR_VERTEX_FVF LIGHT_VERTEX_FVF | D3DFVF_DIFFUSE
#define TEX_VERTEX_FVF   COLOR_VERTEX_FVF | D3DFVF_TEX1

static const BOOL GRAPHICS_LEVEL = 1;

class ILObject;

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
};

struct LightVertex : Vertex
{
	LightVertex() {}
	LightVertex(float x, float y, float z) : Vertex(x, y, z) {}
	void SetNormal(D3DXVECTOR3 vNormal)
	{
		_nx = vNormal.x;
		_ny = vNormal.y;
		_nz = vNormal.z;
	}
	float _nx, _ny, _nz;
	D3DXVECTOR3 operator - (CONST LightVertex& v) const {return D3DXVECTOR3(_nx - v._nx, _nx - v._nx, _nz - v._nz);}
};

struct ColorVertex : LightVertex
{
	ColorVertex(){}
	ColorVertex(float x, float y, float z, D3DCOLOR color) : LightVertex(x, y, z)
	{
		_color = color;
	}
	D3DCOLOR _color;
};

struct TexVertex : ColorVertex
{
	TexVertex(){}
	TexVertex(float x, float y, float z, D3DCOLOR color, float u, float v) : ColorVertex(x, y, z, color)
	{
		_u = u;
		_v = v;
	}
	float _u, _v;
};

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

	L3DENGINE_API void InitVertexNormal(LightVertex* pVertexs);
	L3DENGINE_API D3DLIGHT9 InitDirectionalLight(const D3DXVECTOR3& vDirection, const D3DXCOLOR& color);
}

class L3DENGINE_CLASS L3DEngine
{
public:
	L3DEngine();
	~L3DEngine();

	HRESULT Init(HINSTANCE hInstance, L3DWINDOWPARAM& WindowParam);
	HRESULT Uninit();

	HRESULT AddAction(ILObject* pAction);
	HRESULT Setup();
	HRESULT Active();

private:
	struct ADAPTERMODE
	{
		UINT uWidth;
		UINT uHeight;
		UINT uRefreshRates[32];
	};

	struct SampFilter
	{
		DWORD dwAnisotropy;
		int nMipFilter;
		int nMinFilter;
		int nMagFilter;
	};

	IDirect3D9* m_p3D9;
	IDirect3DDevice9* m_p3DDevice;

	float m_fLastTime;

	L3DWINDOWPARAM m_WindowParam;
	SampFilter m_SampFilter[GRAPHICS_LEVEL_COUNT];
	SampFilter& m_CurSampFilter;

	D3DCAPS9 m_Caps9;
	D3DPRESENT_PARAMETERS m_PresentParam;
	std::vector<ADAPTERMODE> m_AdapterModes;
	std::list<ILObject*> m_ActionList;

private:
	static LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HRESULT InitPresentParam(HWND hWnd);
	HRESULT InitSamplerFilter(UINT uAdapter, D3DDEVTYPE eDeviceType);
	HRESULT InitTransform();

	HRESULT GetL3DAdapter(PUINT puAdapter, D3DDEVTYPE* pDeviceType);
	HRESULT GetL3DAdapterMode(UINT uAdapter);
	HRESULT CreateL3DWindow(HWND* pWnd, HINSTANCE hInstance);
	HRESULT CreateL3DDevice(UINT uAdapter, D3DDEVTYPE eDeviceType, HWND hWnd);

	HRESULT EnterMsgLoop();
};

L3DENGINE_API HRESULT CreateL3DEngine(L3DEngine** ppL3DEngine);