#include "LObjectMgr.h"
#include "Object/LModel.h"
#include "Scene/LScene.h"
#include "LAssert.h"

LObjectMgr::LObjectMgr()
: m_p3DDevice(NULL)
, m_pFont(NULL)
{
	m_ObjectList.clear();
}

LObjectMgr::~LObjectMgr()
{
	LModel* pObject = NULL;
	std::list<LModel*>::iterator it;

	for (it = m_ObjectList.begin(); it != m_ObjectList.end(); it++)
	{
		pObject = *it;
		SAFE_DELETE(pObject);
	}
}

HRESULT LObjectMgr::Init(HINSTANCE hInstance, L3DWINDOWPARAM& WindowParam)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	D3DLIGHT9 DirectionalLight;

	do 
	{
		hr = IL3DEngine::Instance()->Init(hInstance, WindowParam);
		HRESULT_ERROR_BREAK(hr);

		m_p3DDevice = IL3DEngine::Instance()->GetDevice();
		BOOL_ERROR_BREAK(m_p3DDevice);

		DirectionalLight = L3D::InitDirectionalLight(D3DXVECTOR3(1.0f, -0.0f, 0.25f), L3D::WHITE);
		m_p3DDevice->SetLight(0, &DirectionalLight);
		m_p3DDevice->LightEnable(0, TRUE);

		//m_p3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_p3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_p3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		m_p3DDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);

		m_p3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		m_p3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

		m_p3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_p3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	} while (0);

	return S_OK;
}

HRESULT LObjectMgr::Uninit()
{
	IL3DEngine::Instance()->Uninit();
	return S_OK;
}

LScene* LObjectMgr::CreateScene(TCHAR* pwcsMeshPath)
{
	LScene* pLScene;

	do
	{
		pLScene = new LScene;
		BOOL_ERROR_BREAK(pLScene);

		m_SceneList.push_back(pLScene);
		pLScene->Create(IL3DEngine::Instance(), m_p3DDevice, pwcsMeshPath);

	} while (0);

	return pLScene;
}

HRESULT LObjectMgr::Update(float fDeltaTime)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	LModel* pObject = NULL;
	std::list<LModel*>::iterator it;

	do
	{
		for (it = m_ObjectList.begin(); it != m_ObjectList.end(); it++)
		{
			pObject = *it;
			BOOL_ERROR_CONTINUE(pObject);
			pObject->Display(IL3DEngine::Instance(), m_p3DDevice, fDeltaTime);
		}

		hr = IL3DEngine::Instance()->Update(fDeltaTime);
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while(0);

	return hResult;
}

BOOL LObjectMgr::IsActive()
{
	return IL3DEngine::Instance()->IsActive();
}