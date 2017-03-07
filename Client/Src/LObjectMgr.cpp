#include "LObjectMgr.h"
#include "Object/LObject.h"
#include "LAssert.h"

LObjectMgr::LObjectMgr()
: m_fLastTime(0.f)
, m_p3DEngine(NULL)
, m_p3DDevice(NULL)
{
	m_ObjectList.clear();
}

LObjectMgr::~LObjectMgr()
{
	LObject* pObject = NULL;
	std::list<LObject*>::iterator it;

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
		// Init Engine
		hr = CreateL3DEngine(&m_p3DEngine);
		HRESULT_ERROR_BREAK(hr);

		hr = m_p3DEngine->Init(hInstance, WindowParam);
		HRESULT_ERROR_BREAK(hr);

		hr = m_p3DEngine->GetDevice(&m_p3DDevice);
		HRESULT_ERROR_BREAK(hr);

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

		m_fLastTime = (float)timeGetTime();

	} while (0);

	return S_OK;
}

HRESULT LObjectMgr::Uninit()
{
	if (m_p3DEngine)
		m_p3DEngine->Uninit();
	return S_OK;
}

HRESULT LObjectMgr::Setup()
{
	LObject* pObject = NULL;
	std::list<LObject*>::iterator it;

	for (it = m_ObjectList.begin(); it != m_ObjectList.end(); it++)
	{
		pObject = *it;
		BOOL_ERROR_CONTINUE(pObject);
		pObject->Setup(m_p3DEngine, m_p3DDevice);
	}

	return S_OK;
}

HRESULT LObjectMgr::Update()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	float fCurTime = 0;
	float fDeltaTime = 0;
	LObject* pObject = NULL;
	std::list<LObject*>::iterator it;

	do
	{
		fCurTime = (float)timeGetTime();
		fDeltaTime = (fCurTime - m_fLastTime) * 0.001f;

		for (it = m_ObjectList.begin(); it != m_ObjectList.end(); it++)
		{
			pObject = *it;
			BOOL_ERROR_CONTINUE(pObject);
			pObject->Display(m_p3DEngine, m_p3DDevice, fDeltaTime);
		}

		hr = m_p3DEngine->Update(fDeltaTime);
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while(0);

	m_fLastTime = fCurTime;

	return hResult;
}

BOOL LObjectMgr::IsActive()
{
	return m_p3DEngine->IsActive();
}
