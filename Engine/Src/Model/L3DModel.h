#pragma once

#include <d3dx9.h>
#include "L3DInterface.h"

enum LOBJECT_RENDER_PARAM
{
	LOBJECT_RENDER_NONE,
	LOBJECT_RENDER_ALPHA     = 1,
	LOBJECT_RENDER_TRANSFORM = 2,
	LOBJECT_RENDER_MATERIAL  = 4,
	LOBJECT_RENDER_TEXTURE   = 8,
	LOBJECT_RENDER_COUNT,
};

enum LOBJECT_OPTIMIZE_PARAM
{
	LOBJECT_OPTIMIZE_NONE,
	LOBJECT_OPTIMIZE_FVF          = 1,
	LOBJECT_OPTIMIZE_PROGRESSIVE  = 2,
	LOBJECT_OPTIMIZE_COUNT,
};

struct IDirect3DDevice9;
class L3DMesh;
class L3DMaterial;
class L3DTexture;

class L3DModel : public ILModel
{
public:
	struct LoadModelFunc
	{
		LPCWSTR pwcsFileExt;
		HRESULT (L3DModel::*fnLoadMesh)(IDirect3DDevice9* p3DDevice, LPCWSTR cszFileName);
	};


public:
	L3DModel();
	virtual ~L3DModel();

	HRESULT Init(LPDIRECT3DDEVICE9 p3DDevice, TexVertex* pModelVerteices, UINT nVerteicesCount, WORD* pwModelIndices, UINT nIndicesCount);
	HRESULT Init(LPDIRECT3DDEVICE9 p3DDevice, LOBJECT_MESH_TYPE eModelType, LPCWSTR pcszFileName);

	virtual HRESULT SetAlpha(float fAlpha);
	virtual HRESULT SetScale(float fScale);
	virtual HRESULT SetTexture(LPCWSTR szTexture);
	virtual HRESULT SetMaterial(const D3DMATERIAL9& Material);
	virtual HRESULT SetTranslation(const D3DXVECTOR3& vTranslation);
	virtual HRESULT SetRotation(const D3DXQUATERNION& qRotation);

	HRESULT LoadModel(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR cszFileName);

	HRESULT LoadXMesh(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR cszFileName);
	HRESULT LoadLMesh(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR cszFileName);
	HRESULT LoadLMaterial(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR cszFileName);
	HRESULT LoadLTexture(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR cszFileName);

public:
	HRESULT UpdateDisplay();

private:
	LPDIRECT3DDEVICE9 m_p3DDevice;
	ID3DXBuffer* m_pAdjBuffer;
	D3DMATERIAL9* m_pMaterial;
	LPDIRECT3DTEXTURE9* m_ppTexture;
	DWORD m_dwSubsetCount;
	LOBJECT_TYPE m_ObjectType;
	DWORD m_dwRenderParam;
	DWORD m_dwOptimizeParam;

	L3DMesh* m_pLMesh;
	L3DMaterial* m_pLMaterial;
	L3DTexture* m_pLTexture;

	float m_fAlpha;
	float m_fScale;
	D3DXVECTOR3 m_vTranslation;
	D3DXQUATERNION m_qRotation;
	D3DXMATRIX m_matTransform;

	HRESULT UpdateRenderState();
	HRESULT UpdateTransform();
	HRESULT UpdateLOD();
	HRESULT UpdateMesh(DWORD uIndex);
	HRESULT UpdateMaterial(DWORD uIndex);
	HRESULT UpdateTexture(DWORD uIndex);
	HRESULT UpdateDraw(DWORD uIndex);
	HRESULT ResetRendState();

	static const LoadModelFunc* GetLoadModelFunc(LPCWSTR cszFileName);

	union
	{
		struct
		{
			IDirect3DVertexBuffer9* pVertexBuffer;
			IDirect3DIndexBuffer9* pIndexBuffer;
		} LVertex;
		struct
		{
			ID3DXBaseMesh* pMesh;
		} LMesh;
	}m_DisplaySource;
};

static L3DModel::LoadModelFunc s_LoadModelFunc[] = {
	{TEXT(".x"),    &L3DModel::LoadXMesh},
	{TEXT(".mesh"), &L3DModel::LoadLMesh},
	{TEXT(".mtl"),  &L3DModel::LoadLMaterial},
};