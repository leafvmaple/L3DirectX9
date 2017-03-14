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

struct IDirect3DDevice9;

class LEModel : public ILModel
{
public:
	LEModel();
	~LEModel();

	HRESULT Init(IDirect3DDevice9* p3DDevice, TexVertex* pModelVerteices, UINT nVerteicesCount, WORD* pwModelIndices, UINT nIndicesCount);
	HRESULT Init(IDirect3DDevice9* p3DDevice, ID3DXMesh** ppMesh, LOBJECT_MESH_TYPE eModelType, LPCWSTR pcszFileName);

	virtual HRESULT SetAlpha(float fAlpha);
	virtual HRESULT SetScale(float fScale);
	virtual HRESULT SetTexture(LPCWSTR szTexture);
	virtual HRESULT SetMaterial(const D3DMATERIAL9& Material);
	virtual HRESULT SetTranslation(const D3DXVECTOR3& vTranslation);
	virtual HRESULT SetRotation(const D3DXQUATERNION& qRotation);

public:
	HRESULT UpdateDisplay();

private:
	IDirect3DDevice9* m_p3DDevice;
	ID3DXBuffer* m_pAdjBuffer;
	D3DMATERIAL9* m_pMaterial;
	LPDIRECT3DTEXTURE9* m_ppTexture;
	DWORD m_dwSubsetCount;
	LOBJECT_TYPE m_ObjectType;
	DWORD m_dwRenderParam;

	float m_fAlpha;
	float m_fScale;
	D3DXVECTOR3 m_vTranslation;
	D3DXQUATERNION m_qRotation;
	D3DXMATRIX m_matTransform;

	HRESULT UpdateRenderState();
	HRESULT UpdateTransform();
	HRESULT UpdateMaterial(DWORD uIndex);
	HRESULT UpdateTexture(DWORD uIndex);
	HRESULT UpdateDraw(DWORD uIndex);
	HRESULT ResetRendState();

	union
	{
		struct
		{
			IDirect3DVertexBuffer9* pVertexBuffer;
			IDirect3DIndexBuffer9* pIndexBuffer;
		} LVertex;
		struct
		{
			ID3DXMesh* pMesh;
		} LMesh;
	}m_DisplaySource;
};