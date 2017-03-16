#include "LAssert.h"
#include "L3DInterface.h"
#include "L3DEngine.h"
#include "LEModel.h"
#include "LEFont.h"
#include "LEInput.h"

IL3DEngine* IL3DEngine::m_pInstance = NULL;

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

IL3DEngine* IL3DEngine::Instance()
{
	if (!m_pInstance)
		m_pInstance = new L3DEngine;
	return m_pInstance;
}

HRESULT ILModel::Create(IL3DEngine* pL3DEngie, TexVertex* pModelVerteices, UINT nVerteicesCount, WORD* pwModelIndices, UINT nIndicesCount, ILModel** ppModel)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	L3DEngine* pEngine = NULL;
	LEModel* pEModel = NULL;
	IDirect3DDevice9* p3DDevice = NULL;

	do 
	{
		BOOL_ERROR_BREAK(pL3DEngie);

		pEModel = new LEModel;
		BOOL_ERROR_BREAK(pEModel);

		pEngine = dynamic_cast<L3DEngine*>(pL3DEngie);
		BOOL_ERROR_BREAK(pEngine);

		hr = pEngine->GetDevice(&p3DDevice);
		HRESULT_ERROR_BREAK(hr);

		hr = pEModel->Init(p3DDevice, pModelVerteices, nVerteicesCount, pwModelIndices, nIndicesCount);
		HRESULT_ERROR_BREAK(hr);

		pEngine->AttachObject(pEModel);
		*ppModel = pEModel;

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT ILModel::Create(IL3DEngine* pL3DEngie, ID3DXBaseMesh** ppMesh, LOBJECT_MESH_TYPE eModelType, LPCWSTR pcszFileName, ILModel** ppModel)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	L3DEngine* pEngine = NULL;
	LEModel* pEModel = NULL;
	IDirect3DDevice9* p3DDevice = NULL;

	do 
	{
		BOOL_ERROR_BREAK(pL3DEngie);

		pEModel = new LEModel;
		BOOL_ERROR_BREAK(pEModel);

		pEngine = dynamic_cast<L3DEngine*>(pL3DEngie);
		BOOL_ERROR_BREAK(pEngine);

		hr = pEngine->GetDevice(&p3DDevice);
		HRESULT_ERROR_BREAK(hr);

		hr = pEModel->Init(p3DDevice, ppMesh, eModelType, pcszFileName);
		HRESULT_ERROR_BREAK(hr);

		pEngine->AttachObject(pEModel);
		*ppModel = pEModel;

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT ILFont::Create(IL3DEngine* pL3DEngie, ILFont** ppFont, int nSize/* = 9 */)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	L3DEngine* pEngine = NULL;
	LEFont* pEFont = NULL;
	IDirect3DDevice9* p3DDevice = NULL;

	do 
	{
		BOOL_ERROR_BREAK(pL3DEngie);

		pEFont = new LEFont;
		BOOL_ERROR_BREAK(pEFont);

		pEngine = dynamic_cast<L3DEngine*>(pL3DEngie);
		BOOL_ERROR_BREAK(pEngine);

		hr = pEngine->GetDevice(&p3DDevice);
		HRESULT_ERROR_BREAK(hr);

		hr = pEFont->Init(p3DDevice, nSize);
		HRESULT_ERROR_BREAK(hr);

		pEngine->AttachFont(pEFont);
		*ppFont = pEFont;

		hResult = S_OK;
	} while (0);

	return hResult;
}
