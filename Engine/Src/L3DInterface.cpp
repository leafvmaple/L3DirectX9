#include "LAssert.h"
#include "L3DInterface.h"
#include "L3DEngine.h"
#include "L3DObject.h"

L3DENGINE_API void L3D::InitVertexNormal(LightVertex* pVertexs)
{
	D3DXVECTOR3 vNormal;
	D3DXVECTOR3 u = pVertexs[0] - pVertexs[1];
	D3DXVECTOR3 v = pVertexs[1] - pVertexs[2];
	D3DXVec3Cross(&vNormal, &u, &v);
	D3DXVec3Normalize(&vNormal, &vNormal);
	pVertexs[0].SetNormal(vNormal);
	pVertexs[1].SetNormal(vNormal);
	pVertexs[2].SetNormal(vNormal);
}

L3DENGINE_API D3DLIGHT9 L3D::InitDirectionalLight(const D3DXVECTOR3& vDirection, const D3DXCOLOR& color)
{
	D3DLIGHT9 Light;
	::ZeroMemory(&Light, sizeof(Light));

	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Ambient = color * 0.4f;
	Light.Diffuse = color;
	Light.Specular = color * 0.6f;
	Light.Direction = vDirection;
	return Light;
}

L3DENGINE_API D3DLIGHT9 L3D::InitPointLight(const D3DXVECTOR3& vPosition, const D3DXCOLOR& color)
{
	D3DLIGHT9 Light;
	::ZeroMemory(&Light, sizeof(Light));

	Light.Type      = D3DLIGHT_POINT;
	Light.Ambient   = color * 0.4f;
	Light.Diffuse   = color;
	Light.Specular  = color * 0.6f;
	Light.Position  = vPosition;
	Light.Range        = 1000.0f;
	Light.Falloff      = 1.0f;
	Light.Attenuation0 = 1.0f;
	Light.Attenuation1 = 0.0f;
	Light.Attenuation2 = 0.0f;
	return Light;
}

L3DENGINE_API D3DLIGHT9 L3D::InitSpotLight(const D3DXVECTOR3& vPosition, const D3DXVECTOR3& vDirection, const D3DXCOLOR& color)
{
	D3DLIGHT9 Light;
	::ZeroMemory(&Light, sizeof(Light));

	Light.Type      = D3DLIGHT_SPOT;
	Light.Ambient   = color * 0.4f;
	Light.Diffuse   = color;
	Light.Specular  = color * 0.6f;
	Light.Position  = vPosition;
	Light.Direction = vDirection;
	Light.Range        = 1000.0f;
	Light.Falloff      = 1.0f;
	Light.Attenuation0 = 1.0f;
	Light.Attenuation1 = 0.0f;
	Light.Attenuation2 = 0.0f;
	Light.Theta        = 0.5f;
	Light.Phi          = 0.7f;
	return Light;
}

L3DENGINE_API HRESULT CreateL3DEngine(IL3DEngine** ppL3DEngine)
{
	*ppL3DEngine = new L3DEngine;
	return *ppL3DEngine ? S_OK : E_FAIL;
}

L3DENGINE_API HRESULT CreateLObject(IL3DEngine* pL3DEngie, ILObject** ppObject)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		BOOL_ERROR_BREAK(pL3DEngie);

		*ppObject = new L3DObject;
		BOOL_ERROR_BREAK(*ppObject);

		pL3DEngie->AttachObject(*ppObject);

		hResult = S_OK;
	} while (0);

	return hResult;
}
