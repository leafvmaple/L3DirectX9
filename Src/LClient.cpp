
#include <Windows.h>
#include "Engine/LAssert.h"
#include "Engine/L3DEngine.h"
#include "Cube.h"
#include "Teapot.h"

L3DEngine* g_pL3DEngine = NULL;

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HRESULT hr = E_FAIL;
	L3DEngine::L3DWINDOWPARAM WindowParam;
	//Cube* pTexture = NULL;
	Teapot* pTeapot = NULL;

	g_pL3DEngine = new L3DEngine;
	BOOL_ERROR_RETURN(g_pL3DEngine);

	WindowParam.x = 100;
	WindowParam.y = 100;
	WindowParam.Width = 800;
	WindowParam.Height = 600;
	WindowParam.bWindow = TRUE;
	WindowParam.lpszClassName = TEXT("LDirectX");
	WindowParam.lpszWindowName = TEXT("L3D DirectX9 Engine");

	hr = g_pL3DEngine->Init(hInstance, WindowParam);
	HRESULT_ERROR_RETURN(hr);

	//pTexture = new Cube;
	//BOOL_ERROR_RETURN(pTexture);

	pTeapot = new Teapot;
	BOOL_ERROR_RETURN(pTeapot);

	//hr = g_pL3DEngine->AddAction(pTexture);
	hr = g_pL3DEngine->AddAction(pTeapot);
	HRESULT_ERROR_RETURN(hr);

	hr = g_pL3DEngine->Setup();
	HRESULT_ERROR_RETURN(hr);

	hr = g_pL3DEngine->Active();
	HRESULT_ERROR_RETURN(hr);

	hr = g_pL3DEngine->Uninit();
	HRESULT_ERROR_RETURN(hr);

	return 1;
}