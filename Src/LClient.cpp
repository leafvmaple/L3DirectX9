
#include <Windows.h>
#include "LAssert.h"
#include "L3DEngine.h"

L3DEngine* g_pL3DEngine = NULL;

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HRESULT hr = E_FAIL;
	L3DEngine::L3DWINDOWPARAM WindowParam;

	g_pL3DEngine = new L3DEngine;
	BOOL_ERROR_RETURN(g_pL3DEngine);

	WindowParam.x = 100;
	WindowParam.y = 100;
	WindowParam.Width = 300;
	WindowParam.Height = 300;
	WindowParam.bWindow = TRUE;
	WindowParam.lpszClassName = TEXT("LDirectX");
	WindowParam.lpszWindowName = TEXT("L3D DirectX9 Engine");

	hr = g_pL3DEngine->Init(hInstance, WindowParam);
	HRESULT_ERROR_RETURN(hr);

	hr = g_pL3DEngine->Setup();
	HRESULT_ERROR_RETURN(hr);

	hr = g_pL3DEngine->Run();
	HRESULT_ERROR_RETURN(hr);

	hr = g_pL3DEngine->Uninit();
	HRESULT_ERROR_RETURN(hr);

	return 1;
}