#include "LCharacter.h"
#include "LAssert.h"

LCharacter::LCharacter()
: m_fAngleX(0.f)
, m_fAngleY(0.f)
{
	ZeroMemory(m_wszMeshPath, sizeof(m_wszMeshPath));
}

LCharacter::LCharacter(TCHAR* pwszMeshPath)
: m_fAngleX(0.f)
, m_fAngleY(0.f)
{
	wcscpy_s(m_wszMeshPath, pwszMeshPath);
}

LCharacter::~LCharacter()
{

}

HRESULT LCharacter::Create(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do
	{
		BOOL_ERROR_BREAK(p3DDevice);
		BOOL_ERROR_BREAK(p3DEngine);

		//hr = ILModel::Create(p3DEngine, &pMesh, LOBJECT_MESH_DX, m_wszMeshPath, &m_pObject);
		//HRESULT_ERROR_BREAK(hr);

		hr = ILModel::Create(p3DEngine, LOBJECT_MESH_LX, m_wszMeshPath, &m_pObject);
		HRESULT_ERROR_BREAK(hr);

		//hr = m_pObject->SetMaterial(L3D::RED_MTL);
		//HRESULT_ERROR_BREAK(hr);

		//hr = m_pObject->SetAlpha(0.5);
		//HRESULT_ERROR_BREAK(hr);

		//hr = m_pObject->SetTexture("res/texture.png");
		//HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT LCharacter::Display(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice, float fDeltaTime)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	D3DXQUATERNION qRotation;

	do 
	{
		D3DXQuaternionRotationYawPitchRoll(&qRotation, m_fAngleY, 0, 0);
		m_pObject->SetRotation(qRotation);

		m_fAngleY += fDeltaTime;

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT LCharacter::SetScale(float fScale)
{
	return m_pObject->SetScale(fScale);
}

HRESULT LCharacter::SetTranslation(const D3DXVECTOR3& vTranslation)
{
	return m_pObject->SetTranslation(vTranslation);
}
