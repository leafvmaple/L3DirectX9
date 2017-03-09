#include "LClient.h"
#include "L3DInterface.h"

LClient::LClient()
: m_fLastTime(0.f)
, m_fTimeElapsed(0.f)
, m_nFrame(0)
, m_pFont(NULL)
, m_pObjectMgr(NULL)
{

}

LClient::~LClient()
{

}

HRESULT LClient::Init(HINSTANCE hInstance)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	IL3DEngine* pEngine = NULL;
	L3DWINDOWPARAM WindowParam;

	do 
	{
		WindowParam.x = 100;
		WindowParam.y = 100;
		WindowParam.Width = 800;
		WindowParam.Height = 600;
		WindowParam.bWindow = TRUE;
		WindowParam.lpszClassName = TEXT("LDirectX");
		WindowParam.lpszWindowName = TEXT("L3D DirectX9 Engine");

		m_pObjectMgr = new LObjectMgr;
		HRESULT_ERROR_BREAK(m_pObjectMgr);

		hr = m_pObjectMgr->Init(hInstance, WindowParam);
		HRESULT_ERROR_BREAK(hr);

		hr = m_pObjectMgr->CreateModel<LCube>();
		HRESULT_ERROR_BREAK(hr);

		hr = m_pObjectMgr->CreateModel<LTeapot>();
		HRESULT_ERROR_BREAK(hr);

		hr = m_pObjectMgr->Setup();
		HRESULT_ERROR_BREAK(hr);

		pEngine = m_pObjectMgr->GetEngine();
		BOOL_ERROR_BREAK(pEngine);

		hr = ILFont::Create(pEngine, &m_pFont, 12);
		HRESULT_ERROR_BREAK(hr);

		hr = m_pFont->SetText(TEXT("FPS:"));
		HRESULT_ERROR_BREAK(hr);

		hr = m_pFont->SetColor(L3D::GREEN);
		HRESULT_ERROR_BREAK(hr);

		hr = m_pFont->SetPosition(0, 0);
		HRESULT_ERROR_BREAK(hr);

		m_fLastTime = (float)timeGetTime();

		hResult = S_OK;

	} while (0);

	return hResult;
}

HRESULT LClient::Update()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	float fCurTime = 0;
	float fDeltaTime = 0;

	do
	{
		fCurTime = (float)timeGetTime();
		fDeltaTime = (fCurTime - m_fLastTime) * 0.001f;

		hr = m_pObjectMgr->Update(fDeltaTime);
		HRESULT_ERROR_BREAK(hr);

		hr = ShowFPS(fDeltaTime);
		HRESULT_ERROR_BREAK(hr);

		m_fLastTime = fCurTime;

		hResult = S_OK;
	} while(0);

	return hResult;
}

HRESULT LClient::Uninit()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		hr = m_pObjectMgr->Uninit();
		HRESULT_ERROR_RETURN(hr);

	} while (0);

	return S_OK;
}

BOOL LClient::IsActive()
{
	return m_pObjectMgr->IsActive();
}

HRESULT LClient::ShowFPS(float fDeltaTime)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	WCHAR wszFPS[FONT_STRING_MAX];

	do 
	{
		m_nFrame++;
		m_fTimeElapsed += fDeltaTime;

		if (m_fTimeElapsed >= 1.0f)
		{
			swprintf(wszFPS, FONT_STRING_MAX, TEXT("FPS:%.2f"), m_nFrame / m_fTimeElapsed);

			hr = m_pFont->SetText(wszFPS);
			HRESULT_ERROR_BREAK(hr);

			m_fTimeElapsed = 0.f;
			m_nFrame = 0;
		}
		hResult = S_OK;
	} while (0);
	
	return hResult;
}

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HRESULT hr = E_FAIL;
	LClient Client;

	hr = Client.Init(hInstance);
	HRESULT_ERROR_RETURN(hr);

	while (Client.IsActive())
	{
		hr = Client.Update();
		HRESULT_ERROR_BREAK(hr);
	}

	Client.Uninit();

	return 1;
}