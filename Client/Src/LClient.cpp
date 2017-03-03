
#include <Windows.h>
#include "LAssert.h"
#include "LExports.h"
#include "L3DInterface.h"
#include "LObjectMgr.h"
#include "Object/LCube.h"
#include "Object/LTeapot.h"

class L3DENGINE_CLASS IL3DEngine;
IL3DEngine* g_pL3DEngine = NULL;

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HRESULT hr = E_FAIL;
	L3DWINDOWPARAM WindowParam;
	LObjectMgr* pObjectMgr = NULL;
	IDirect3DDevice9* p3DDevice = NULL;

	hr = CreateL3DEngine(&g_pL3DEngine);
	HRESULT_ERROR_RETURN(hr);

	WindowParam.x = 100;
	WindowParam.y = 100;
	WindowParam.Width = 800;
	WindowParam.Height = 600;
	WindowParam.bWindow = TRUE;
	WindowParam.lpszClassName = TEXT("LDirectX");
	WindowParam.lpszWindowName = TEXT("L3D DirectX9 Engine");

	hr = g_pL3DEngine->Init(hInstance, WindowParam);
	HRESULT_ERROR_RETURN(hr);

	hr = g_pL3DEngine->GetDevice(&p3DDevice);
	HRESULT_ERROR_RETURN(hr);

	pObjectMgr = new LObjectMgr;
	BOOL_ERROR_RETURN(pObjectMgr);

	hr = pObjectMgr->Init(g_pL3DEngine, p3DDevice);
	HRESULT_ERROR_RETURN(hr);

	hr = pObjectMgr->CreateObject<LCube>();
	HRESULT_ERROR_RETURN(hr);

	hr = pObjectMgr->CreateObject<LTeapot>();
	HRESULT_ERROR_RETURN(hr);

	hr = pObjectMgr->Setup();
	HRESULT_ERROR_RETURN(hr);

	while(true)
	{
		hr = pObjectMgr->Active();
		HRESULT_ERROR_RETURN(hr);
	}

	hr = pObjectMgr->Uninit();
	HRESULT_ERROR_RETURN(hr);

	return 1;
}