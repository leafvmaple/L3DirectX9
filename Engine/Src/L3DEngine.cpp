#include <Windows.h>
#include <process.h>
#include <strsafe.h>
#include "LEModel.h"
#include "LEFont.h"
#include "LEInput.h"
#include "LAssert.h"
#include "L3DEngine.h"
#include "IOManager/LFileReader.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	L3DEngine* pEngine = dynamic_cast<L3DEngine*>(L3DEngine::Instance());
	return pEngine->MsgProc(hWnd, msg, wParam, lParam);
}

L3DEngine::L3DEngine()
: m_bActive(FALSE)
, m_p3D9(NULL)
, m_p3DDevice(NULL)
, m_pInput(NULL)
, m_CurSampFilter(m_SampFilter[GRAPHICS_LEVEL_MAX])
{
	memset(&m_Caps9, 0, sizeof(m_Caps9));
	memset(&m_Camera, 0, sizeof(m_Camera));
	memset(&m_SampFilter, 0, sizeof(m_SampFilter));
	memset(&m_WindowParam, 0, sizeof(m_WindowParam));
	memset(&m_PresentParam, 0, sizeof(m_PresentParam));
	
	m_AdapterModes.clear();
	m_ModelList.clear();
}

L3DEngine::~L3DEngine()
{

}

HRESULT L3DEngine::Init(HINSTANCE hInstance, L3DWINDOWPARAM& WindowParam)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	HWND hWnd = NULL;
	UINT uAdapter = D3DADAPTER_DEFAULT;
	D3DDEVTYPE eDeviceType = D3DDEVTYPE_HAL;

	do 
	{
		m_WindowParam = WindowParam;

		m_p3D9 = Direct3DCreate9(D3D_SDK_VERSION);
		BOOL_ERROR_BREAK(m_p3D9);

		hr = GetL3DAdapter(&uAdapter, &eDeviceType);
		HRESULT_ERROR_BREAK(hr);

		hr = GetL3DAdapterMode(uAdapter);
		HRESULT_ERROR_BREAK(hr);

		hr = CreateL3DWindow(&hWnd, hInstance);
		HRESULT_ERROR_BREAK(hr);

		hr = InitPresentParam(hWnd);
		HRESULT_ERROR_BREAK(hr);

		hr = InitSamplerFilter(uAdapter, eDeviceType);
		HRESULT_ERROR_BREAK(hr);

		hr = CreateL3DDevice(uAdapter, eDeviceType, hWnd);
		HRESULT_ERROR_BREAK(hr);

		hr = InitInput(hWnd, hInstance);
		HRESULT_ERROR_BREAK(hr);

		hr = InitCameraParam();
		HRESULT_ERROR_BREAK(hr);

		//Test
		LMESH_DATA LBMesh;
		LFileReader::LoadMesh("Res/Mesh/M2_1035_body.mesh", &LBMesh);

		m_bActive = TRUE;

		hResult = S_OK;
	} while (0);
	
	return hResult;
}

HRESULT L3DEngine::Update(float fDeltaTime)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	LEModel* pObject = NULL;
	LEFont* pFont = NULL;
	std::list<ILModel*>::iterator itModel;
	std::list<ILFont*>::iterator itFont;

	do
	{
		hr = UpdateMessage();
		HRESULT_ERROR_BREAK(hr);

		hr = UpdateInput();
		HRESULT_ERROR_BREAK(hr);

		hr = UpdateCamera(fDeltaTime);
		HRESULT_ERROR_BREAK(hr);

		hr = m_p3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		//hr = m_p3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		HRESULT_ERROR_BREAK(hr);

		m_p3DDevice->BeginScene();

		for (itModel = m_ModelList.begin(); itModel != m_ModelList.end(); itModel++)
		{
			pObject = dynamic_cast<LEModel*>(*itModel);
			BOOL_ERROR_CONTINUE(pObject);

			pObject->UpdateDisplay();
		}

		for (itFont = m_FontList.begin(); itFont != m_FontList.end(); itFont++)
		{
			pFont = dynamic_cast<LEFont*>(*itFont);
			BOOL_ERROR_CONTINUE(pFont);

			pFont->UpdateDisplay();
		}

		m_p3DDevice->EndScene();

		hr = m_p3DDevice->Present(0, 0, 0, 0);
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while(0);

	return hResult;
}

BOOL L3DEngine::IsActive()
{
	return m_bActive;
}

HRESULT L3DEngine::GetDevice(IDirect3DDevice9** pp3DDevice)
{
	*pp3DDevice = m_p3DDevice;
	return *pp3DDevice ? S_OK : E_FAIL;
}

HRESULT L3DEngine::Uninit()
{
	ILModel* pAction;
	std::list<ILModel*>::iterator it;

	for (it = m_ModelList.begin(); it != m_ModelList.end();)
	{
		pAction = *it;
		BOOL_ERROR_CONTINUE(pAction);

		it = m_ModelList.erase(it);
		SAFE_DELETE(pAction);
	}

	if (m_pInput)
	{
		m_pInput->Uninit();
		SAFE_DELETE(m_pInput);
	}

	return S_OK;
}

HRESULT L3DEngine::AttachObject(ILModel* pAction)
{
	HRESULT hResult = E_FAIL;

	do 
	{
		BOOL_ERROR_BREAK(pAction);
		m_ModelList.push_back(pAction);

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT L3DEngine::AttachFont(ILFont* pFont)
{
	HRESULT hResult = E_FAIL;

	do 
	{
		BOOL_ERROR_BREAK(pFont);
		m_FontList.push_back(pFont);

		hResult = S_OK;
	} while (0);

	return hResult;
}

LRESULT L3DEngine::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)  
	{
	case WM_DESTROY:
		PostQuitMessage(0);  
		break;
	case WM_MOUSEWHEEL:
		m_Camera.fSightDis -= ((float)GET_WHEEL_DELTA_WPARAM(wParam) * 0.005f);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		break;
	}  

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HRESULT L3DEngine::InitPresentParam(HWND hWnd)
{
	m_PresentParam.BackBufferWidth            = 800;
	m_PresentParam.BackBufferHeight           = 600;
	m_PresentParam.BackBufferFormat           = D3DFMT_A8R8G8B8; //���ظ�ʽ
	m_PresentParam.BackBufferCount            = 1;
	m_PresentParam.MultiSampleType            = D3DMULTISAMPLE_NONE;
	m_PresentParam.MultiSampleQuality         = 0;
	m_PresentParam.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
	m_PresentParam.hDeviceWindow              = hWnd;
	m_PresentParam.Windowed                   = m_WindowParam.bWindow;
	m_PresentParam.EnableAutoDepthStencil     = TRUE;
	m_PresentParam.AutoDepthStencilFormat     = D3DFMT_D24S8;
	m_PresentParam.Flags                      = 0;
	m_PresentParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_PresentParam.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	return S_OK;
}

HRESULT L3DEngine::GetL3DAdapter(PUINT puAdapter, D3DDEVTYPE* pDeviceType)
{
	HRESULT hr = E_FAIL;
	UINT uAdapterCount = 0;
	D3DADAPTER_IDENTIFIER9 Identifier;

	uAdapterCount = m_p3D9->GetAdapterCount();
	for (UINT i = 0; i < uAdapterCount; i++)
	{
		hr = m_p3D9->GetAdapterIdentifier(i, 0, &Identifier);
		HRESULT_ERROR_CONTINUE(hr);

		if (strstr(Identifier.Description, "NVPerfHUD") != 0 || !_strcmpi(Identifier.Description, "NVIDIA PerfHUD"))
		{
			*puAdapter = i;
			*pDeviceType = D3DDEVTYPE_REF;
			return S_OK;
		}
	}
	*puAdapter = D3DADAPTER_DEFAULT;
	*pDeviceType = D3DDEVTYPE_HAL;
	return S_OK;
}

HRESULT L3DEngine::GetL3DAdapterMode(UINT uAdapter)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	UINT uModeCount = 0;
	UINT uModeIndex = 0;
	D3DDISPLAYMODE DisplayMode;

	m_AdapterModes.clear();
	do
	{
		hr = m_p3D9->GetAdapterDisplayMode(uAdapter, &DisplayMode);
		HRESULT_ERROR_BREAK(hr);

		uModeCount = m_p3D9->GetAdapterModeCount(uAdapter, DisplayMode.Format);
		BOOL_ERROR_BREAK(uModeCount);

		for (UINT uMode = 0; uMode < uModeCount; ++uMode)
		{
			hr = m_p3D9->EnumAdapterModes(uAdapter, DisplayMode.Format, uMode, &DisplayMode);
			HRESULT_ERROR_CONTINUE(hr);

			size_t uAdapterIndex = 0;
			for (; uAdapterIndex < m_AdapterModes.size(); uAdapterIndex++)
			{
				ADAPTERMODE& AModel = m_AdapterModes[uAdapterIndex];
				if (AModel.uHeight == DisplayMode.Height && AModel.uWidth == DisplayMode.Width)
				{
					for (UINT uRefreshRateIndex = 0; uRefreshRateIndex != _countof(AModel.uRefreshRates); ++uRefreshRateIndex)
					{
						if (!AModel.uRefreshRates[uRefreshRateIndex])
						{
							AModel.uRefreshRates[uRefreshRateIndex] = DisplayMode.RefreshRate;
							break;
						}
					}
					break;
				}
			}
			if (uAdapterIndex == m_AdapterModes.size())
			{
				ADAPTERMODE AModel = {DisplayMode.Width, DisplayMode.Height, DisplayMode.RefreshRate};
				m_AdapterModes.push_back(AModel);
			}
		}

		hResult = S_OK;
	}
	while(0);

	return hResult;
}

HRESULT L3DEngine::InitSamplerFilter(UINT uAdapter, D3DDEVTYPE eDeviceType)
{
	m_p3D9->GetDeviceCaps(uAdapter, eDeviceType, &m_Caps9);

	m_SampFilter[GRAPHICS_LEVEL_MIN].dwAnisotropy = 1;
	m_SampFilter[GRAPHICS_LEVEL_MAX].dwAnisotropy = m_Caps9.MaxAnisotropy;

	m_SampFilter[GRAPHICS_LEVEL_MIN].nMipFilter = D3DTEXF_POINT;
	m_SampFilter[GRAPHICS_LEVEL_MIN].nMinFilter = D3DTEXF_POINT;
	m_SampFilter[GRAPHICS_LEVEL_MIN].nMagFilter = D3DTEXF_POINT;

	m_SampFilter[GRAPHICS_LEVEL_MAX].nMipFilter = D3DTEXF_POINT;
	m_SampFilter[GRAPHICS_LEVEL_MAX].nMinFilter = D3DTEXF_POINT;
	m_SampFilter[GRAPHICS_LEVEL_MAX].nMagFilter = D3DTEXF_POINT;

	if (m_Caps9.MaxAnisotropy <= 1)
	{
		if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR)
			m_SampFilter[GRAPHICS_LEVEL_MAX].nMipFilter = D3DTEXF_LINEAR;

		if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR)
			m_SampFilter[GRAPHICS_LEVEL_MAX].nMinFilter = D3DTEXF_LINEAR;

		if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR)
			m_SampFilter[GRAPHICS_LEVEL_MAX].nMagFilter = D3DTEXF_LINEAR;
	}
	else
	{
		if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR)
			m_SampFilter[GRAPHICS_LEVEL_MAX].nMipFilter = D3DTEXF_LINEAR;

		if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC)
			m_SampFilter[GRAPHICS_LEVEL_MAX].nMinFilter = D3DTEXF_ANISOTROPIC;
		else if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR)
			m_SampFilter[GRAPHICS_LEVEL_MAX].nMinFilter = D3DTEXF_LINEAR;

		if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC)
			m_SampFilter[GRAPHICS_LEVEL_MAX].nMagFilter = D3DTEXF_ANISOTROPIC;
		else if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR)
			m_SampFilter[GRAPHICS_LEVEL_MAX].nMagFilter = D3DTEXF_LINEAR;
	}
	return S_OK;
}

HRESULT L3DEngine::InitInput(HWND hWnd, HINSTANCE hInstance)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		m_pInput = new LEInput;
		BOOL_ERROR_BREAK(m_pInput);

		hr = m_pInput->Init(hWnd, hInstance, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT L3DEngine::InitCameraParam()
{
	m_Camera.fSightDis = 5.f;
	m_Camera.vTarget  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Camera.vUp      = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	return S_OK;
}

HRESULT L3DEngine::CreateL3DWindow(HWND* pWnd, HINSTANCE hInstance)
{
	BOOL bRetCode = FALSE;
	HRESULT hResult = E_FAIL;
	WNDCLASSEX wndClassEx;

	do
	{
		wndClassEx.cbSize        = sizeof(WNDCLASSEX);
		wndClassEx.style         = CS_HREDRAW | CS_VREDRAW;
		wndClassEx.lpfnWndProc   = (WNDPROC)WndProc;
		wndClassEx.cbClsExtra    = 0;
		wndClassEx.cbWndExtra    = 0;
		wndClassEx.hInstance     = hInstance;
		wndClassEx.hIcon         = ::LoadIcon(NULL, IDI_WINLOGO);
		wndClassEx.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
		wndClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndClassEx.lpszMenuName  = NULL;
		wndClassEx.lpszClassName = m_WindowParam.lpszClassName;
		wndClassEx.hIconSm       = NULL;

		bRetCode = RegisterClassEx(&wndClassEx);
		BOOL_ERROR_BREAK(bRetCode);

		*pWnd = CreateWindow(m_WindowParam.lpszClassName, m_WindowParam.lpszWindowName, WS_OVERLAPPEDWINDOW,
			m_WindowParam.x, m_WindowParam.y, m_WindowParam.Width, m_WindowParam.Height,  NULL, NULL, hInstance, NULL);
		BOOL_ERROR_BREAK(*pWnd);

		ShowWindow(*pWnd, SW_SHOWDEFAULT); 

		bRetCode = UpdateWindow(*pWnd);
		BOOL_ERROR_BREAK(bRetCode);

		hResult = S_OK;
	}
	while(0);

	return hResult;
}

HRESULT L3DEngine::CreateL3DDevice(UINT uAdapter, D3DDEVTYPE eDeviceType, HWND hWnd)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	int nVertexProcessing = 0;

	do
	{
		m_CurSampFilter = m_SampFilter[GRAPHICS_LEVEL];

		nVertexProcessing = (m_Caps9.DevCaps && D3DDEVCAPS_HWTRANSFORMANDLIGHT) ? D3DCREATE_MIXED_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		hr = m_p3D9->CreateDevice(uAdapter, eDeviceType, hWnd, nVertexProcessing, &m_PresentParam, &m_p3DDevice);
		HRESULT_ERROR_BREAK(hr);

		m_p3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, m_CurSampFilter.nMipFilter);
		m_p3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, m_CurSampFilter.nMinFilter);
		m_p3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, m_CurSampFilter.nMagFilter);

		m_p3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, m_CurSampFilter.dwAnisotropy);

		m_p3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		m_p3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

		hResult = S_OK;
	}
	while(0);

	return hResult;
}

HRESULT L3DEngine::UpdateMessage()
{
	MSG Msg;

	::ZeroMemory(&Msg, sizeof(MSG));

	while(::PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&Msg);
		::DispatchMessage(&Msg);
	}

	if (Msg.message == WM_QUIT)
		m_bActive = FALSE;

	return S_OK;
}

HRESULT L3DEngine::UpdateInput()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		BOOL_ERROR_BREAK(m_pInput);

		hr = m_pInput->Update();
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT L3DEngine::UpdateCamera(float fDeltaTime)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	D3DXMATRIX matCamera;
	D3DXMATRIX matProj;
	D3DXVECTOR3 vDelta;

	do 
	{
		BOOL_ERROR_BREAK(m_pInput);

		if (m_pInput->IsMouseButtonDown(0)) 
		{
			m_Camera.fYaw += (m_pInput->MouseDX())*  0.01f;
			m_Camera.fPitch += (m_pInput->MouseDY()) * 0.01f;
		}

		if(m_pInput->IsKeyDown(DIK_LEFT))
			m_Camera.fYaw -= 0.001f;

		if(m_pInput->IsKeyDown(DIK_RIGHT))
			m_Camera.fYaw += 0.001f;

		if(m_pInput->IsKeyDown(DIK_UP))
			m_Camera.fPitch += 0.001f;

		if(m_pInput->IsKeyDown(DIK_DOWN))
			m_Camera.fPitch -= 0.001f;

		m_Camera.fSightDis = max(m_Camera.fSightDis, 3.f);

		D3DXMatrixRotationYawPitchRoll(&matCamera, m_Camera.fYaw, m_Camera.fPitch, m_Camera.fRoll);
		D3DXVec3TransformNormal(&vDelta, &D3DXVECTOR3(0.0f, 0.0f, -m_Camera.fSightDis), &matCamera);
		m_Camera.vPositon = m_Camera.vTarget + vDelta;

		// ��ͼ����
		D3DXMatrixLookAtLH(&matCamera, &m_Camera.vPositon, &m_Camera.vTarget, &m_Camera.vUp);
		m_p3DDevice->SetTransform(D3DTS_VIEW, &matCamera);

		// ͶӰ����
		D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI * 0.5f, (float)m_WindowParam.Width / (float)m_WindowParam.Height, 1.0f, 1000.0f);
		m_p3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

		hResult = S_OK;
	} while (0);

	return S_OK;
}