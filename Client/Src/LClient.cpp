#include "LClient.h"

LClient::LClient()
: m_pObjectMgr(NULL)
{

}

LClient::~LClient()
{

}

HRESULT LClient::Init(HINSTANCE hInstance)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
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

		hr = m_pObjectMgr->CreateObject<LCube>();
		HRESULT_ERROR_BREAK(hr);

		hr = m_pObjectMgr->CreateObject<LTeapot>();
		HRESULT_ERROR_BREAK(hr);

		hr = m_pObjectMgr->Setup();
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;

	} while (0);

	return hResult;
}

HRESULT LClient::Active()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	float fCurTime = 0;
	float fDeltaTime = 0;

	do
	{
		hr = m_pObjectMgr->Active();
		HRESULT_ERROR_BREAK(hr);

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

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HRESULT hr = E_FAIL;
	LClient Client;

	hr = Client.Init(hInstance);
	HRESULT_ERROR_RETURN(hr);

	while (true)
	{
		hr = Client.Active();
		HRESULT_ERROR_BREAK(hr);
	}

	Client.Uninit();

	return 1;
}