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

class L3DObject : public ILObject
{
public:
	L3DObject();
	~L3DObject();

	virtual HRESULT CreateVertex(IDirect3DDevice9* p3DDevice, IDirect3DVertexBuffer9** ppVertexBuffer, IDirect3DIndexBuffer9** ppIndexBuffer);
	virtual HRESULT CreateMesh(IDirect3DDevice9* p3DDevice, ID3DXMesh** ppMesh);

	virtual HRESULT SetAlpha(float fAlpha);
	virtual HRESULT SetMaterial(D3DMATERIAL9& Material);
	virtual HRESULT SetTexture(LPCSTR szTexture);
	virtual HRESULT SetTranslation(D3DXVECTOR3& vTranslation);
	virtual HRESULT SetRotation(D3DXQUATERNION& qRotation);

public:
	HRESULT UpdateDisplay();

private:
	IDirect3DDevice9* m_p3DDevice;
	IDirect3DTexture9* m_pTexture;
	LOBJECT_TYPE m_ObjectType;
	DWORD m_dwRenderParam;

	float m_fAlpha;
	D3DMATERIAL9 m_Material;
	D3DXVECTOR3 m_vTranslation;
	D3DXQUATERNION m_qRotation;
	D3DXMATRIX m_matTransform;

	HRESULT UpdateRenderState();
	HRESULT UpdateMaterial();
	HRESULT UpdateTexture();
	HRESULT UpdateTransform();
	HRESULT UpdateDraw();

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