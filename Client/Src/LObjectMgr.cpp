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

HRESULT LObjectMgr::Init(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice)
{
	m_fLastTime = (float)timeGetTime();
	m_p3DEngine = p3DEngine;
	m_p3DDevice = p3DDevice;
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

HRESULT LObjectMgr::Active()
{
	HRESULT hr = E_FAIL;
	float fCurTime = 0;
	float fDeltaTime = 0;
	LObject* pObject = NULL;
	std::list<LObject*>::iterator it;
	MSG Msg;

	::ZeroMemory(&Msg, sizeof(MSG));

	while(Msg.message != WM_QUIT)
	{
		if(::PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&Msg);
			::DispatchMessage(&Msg);
		}
		else
		{
			fCurTime = (float)timeGetTime();
			fDeltaTime = (fCurTime - m_fLastTime) * 0.001f;

			for (it = m_ObjectList.begin(); it != m_ObjectList.end(); it++)
			{
				pObject = *it;
				BOOL_ERROR_CONTINUE(pObject);
				pObject->Display(m_p3DEngine, m_p3DDevice, fDeltaTime);
			}

			hr = m_p3DEngine->Active(fCurTime);
			HRESULT_ERROR_BREAK(hr);

			m_fLastTime = fCurTime;
		}
	}
	return Msg.wParam;
}

