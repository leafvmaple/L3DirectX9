#pragma once

#include <d3dx9.h>
#include "L3DInterface.h"

struct IDirect3DDevice9;

class L3DObject : public ILObject
{
public:
	L3DObject();
	~L3DObject();

	virtual HRESULT SetTranslation(D3DXVECTOR3& vTranslation);
	virtual HRESULT SetRotation(D3DXQUATERNION& qRotation);
	virtual HRESULT CreateVertex(IDirect3DDevice9* p3DDevice, IDirect3DVertexBuffer9** ppVertexBuffer, IDirect3DIndexBuffer9** ppIndexBuffer);
	virtual HRESULT CreateMesh(IDirect3DDevice9* p3DDevice, ID3DXMesh** ppMesh);

public:
	HRESULT UpdateTransform(IDirect3DDevice9* p3DDevice);
	HRESULT UpdateDisplay(IDirect3DDevice9* p3DDevice);

private:
	D3DXVECTOR3 m_vTranslation;
	D3DXQUATERNION m_qRotation;
	D3DXMATRIX m_matTransform;

	LOBJECT_TYPE m_ObjectType;

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