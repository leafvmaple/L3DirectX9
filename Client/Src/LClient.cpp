
#include <Windows.h>
#include "LAssert.h"
#include "LExports.h"
#include "L3DEngine.h"
#include "ILObject.h"
#include "Object/LCube.h"
#include "Object/LTeapot.h"

class L3DENGINE_CLASS L3DEngine;
L3DEngine* g_pL3DEngine = NULL;

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HRESULT hr = E_FAIL;
	L3DWINDOWPARAM WindowParam;
	LCube* pCube = NULL;
	LTeapot* pTeapot = NULL;

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

	pCube = new LCube;
	BOOL_ERROR_RETURN(pCube);

	pTeapot = new LTeapot;
	BOOL_ERROR_RETURN(pTeapot);

	hr = pTeapot->SetTranslation(D3DXVECTOR3(-1, -1, -1));
	HRESULT_ERROR_RETURN(hr);

	hr = g_pL3DEngine->AddAction(pCube);
	HRESULT_ERROR_RETURN(hr);

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