#pragma once

#include <d3dx9.h>
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
	L3DENGINE_API D3DLIGHT9 InitPointLight(const D3DXVECTOR3& vPosition, const D3DXCOLOR& color);
	L3DENGINE_API D3DLIGHT9 InitSpotLight(const D3DXVECTOR3& vPosition, const D3DXVECTOR3& vDirection, const D3DXCOLOR& color);
}

struct ADAPTERMODE
{
	UINT uWidth;
	UINT uHeight;
	UINT uRefreshRates[32];
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

enum LOBJECT_TYPE
{
	LOBJECT_TYPE_INVALID,
	LOBJECT_TYPE_VERTEX,
	LOBJECT_TYPE_MESH,
	LOBJECT_TYPE_COUNT,
};

class L3DENGINE_CLASS ILObject
{
public:
	ILObject() {};
	virtual ~ILObject() {};
	
	virtual HRESULT CreateVertex(IDirect3DDevice9* p3DDevice, IDirect3DVertexBuffer9** ppVertexBuffer, IDirect3DIndexBuffer9** ppIndexBuffer) = 0;
	virtual HRESULT CreateMesh(IDirect3DDevice9* p3DDevice, ID3DXMesh** ppMesh) = 0;

	virtual HRESULT SetMaterial(D3DMATERIAL9& Material) = 0;
	virtual HRESULT SetTexture(LPCSTR szTexture) = 0;
	virtual HRESULT SetTranslation(D3DXVECTOR3& vTranslation) = 0;
	virtual HRESULT SetRotation(D3DXQUATERNION& qRotation) = 0;
};

class L3DENGINE_CLASS IL3DEngine
{
public:
	IL3DEngine() {};
	virtual ~IL3DEngine() {};

	virtual HRESULT Init(HINSTANCE hInstance, L3DWINDOWPARAM& WindowParam) = 0;
	virtual HRESULT Uninit() = 0;

	virtual HRESULT AttachObject(ILObject* pAction) = 0;
	virtual HRESULT Update(float fDeltaTime) = 0;

	virtual BOOL IsActive() = 0;

	virtual HRESULT GetDevice(IDirect3DDevice9** pp3DDevice) = 0;
};

L3DENGINE_API HRESULT CreateL3DEngine(IL3DEngine** ppL3DEngine);
L3DENGINE_API HRESULT CreateLObject(IL3DEngine* pL3DEngie, ILObject** ppObject);