#include <Windows.h>
#include <strsafe.h>
#include <d3dx9.h>
#include "LAssert.h"
#include "L3DEngine.h"

#pragma comment(lib, "d3d9.lib") //引入依赖库  
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

L3DEngine::L3DEngine()
: m_p3D9(NULL)
, m_p3DDevice(NULL)
, m_pVertexBuffer(NULL)
, m_pIndexBuffer(NULL)
, m_fLastTime(0.f)
{
	memset(&m_WindowParam, 0, sizeof(m_WindowParam));
	m_AdapterModes.clear();
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
		m_fLastTime = (float)timeGetTime();

		m_p3D9 = Direct3DCreate9(D3D_SDK_VERSION);
		BOOL_ERROR_BREAK(m_p3D9);

		hr = GetL3DAdapter(&uAdapter, &eDeviceType);
		HRESULT_ERROR_BREAK(hr);

		hr = GetL3DAdapterMode(uAdapter);
		HRESULT_ERROR_BREAK(hr);

		hr = CreateL3DWindow(&hWnd, hInstance);
		HRESULT_ERROR_BREAK(hr);

		hr = CreateL3DDevice(uAdapter, eDeviceType, hWnd);
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);
	
	return hResult;
}

HRESULT L3DEngine::Setup()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	Vertex* Vertices = NULL;
	D3DXVECTOR3 vPosition(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 vTarget(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMATRIX matCamera;
	D3DXMATRIX matProj;
	WORD* pwIndices = 0;

	do 
	{
		hr = m_p3DDevice->CreateVertexBuffer(
			8 * sizeof(Vertex), D3DUSAGE_WRITEONLY,
			D3DFVF_XYZ , D3DPOOL_MANAGED, &m_pVertexBuffer, 0);
		HRESULT_ERROR_BREAK(hr);

		m_p3DDevice->CreateIndexBuffer(
			36 * sizeof(WORD), D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, 0);
		HRESULT_ERROR_BREAK(hr);

		// vertices of a unit cube
		m_pVertexBuffer->Lock(0, 0, (void**)&Vertices, 0);

		Vertices[0] = Vertex(-1.0f, -1.0f, -1.0f);
		Vertices[1] = Vertex(-1.0f, 1.0f, -1.0f);
		Vertices[2] = Vertex( 1.0f, 1.0f, -1.0f);
		Vertices[3] = Vertex( 1.0f, -1.0f, -1.0f);
		Vertices[4] = Vertex(-1.0f, -1.0f, 1.0f);
		Vertices[5] = Vertex(-1.0f, 1.0f, 1.0f);
		Vertices[6] = Vertex( 1.0f, 1.0f, 1.0f);
		Vertices[7] = Vertex( 1.0f, -1.0f, 1.0f);

		m_pVertexBuffer->Unlock();

		// 定义立方体的三角形
		m_pIndexBuffer->Lock(0, 0, (void**)&pwIndices, 0);

		pwIndices[0] = 0; pwIndices[1] = 1; pwIndices[2] = 2;
		pwIndices[3] = 0; pwIndices[4] = 2; pwIndices[5] = 3;
		// 背面
		pwIndices[6] = 4; pwIndices[7] = 6; pwIndices[8] = 5;
		pwIndices[9] = 4; pwIndices[10] = 7; pwIndices[11] = 6;
		// 左面
		pwIndices[12] = 4; pwIndices[13] = 5; pwIndices[14] = 1;
		pwIndices[15] = 4; pwIndices[16] = 1; pwIndices[17] = 0;
		// 右面
		pwIndices[18] = 3; pwIndices[19] = 2; pwIndices[20] = 6;
		pwIndices[21] = 3; pwIndices[22] = 6; pwIndices[23] = 7;
		// 顶部
		pwIndices[24] = 1; pwIndices[25] = 5; pwIndices[26] = 6;
		pwIndices[27] = 1; pwIndices[28] = 6; pwIndices[29] = 2;
		// 底部
		pwIndices[30] = 4; pwIndices[31] = 0; pwIndices[32] = 3;
		pwIndices[33] = 4; pwIndices[34] = 3; pwIndices[35] = 7;

		m_pIndexBuffer->Unlock();

		// 照相机位置（视图矩阵）
		D3DXMatrixLookAtLH(&matCamera, &vPosition, &vTarget, &vUp);
		m_p3DDevice->SetTransform(D3DTS_VIEW, &matCamera);

		D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI * 0.5f, (float)m_WindowParam.Width / (float)m_WindowParam.Height, 1.0f, 1000.0f);
		m_p3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

		// 渲染状态（填充模式：框架填充）
		m_p3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT L3DEngine::Run()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		hr = EnterMsgLoop(&L3DEngine::Display);
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;

	} while (0);

	return hResult;
}

HRESULT L3DEngine::Uninit()
{
	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();

	if (m_pIndexBuffer)
		m_pIndexBuffer->Release();

	return S_OK;
}

LRESULT WINAPI L3DEngine::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)  
	{  
	case WM_DESTROY:  
		//Cleanup();  
		PostQuitMessage(0);  
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		break;
	}  

	return DefWindowProc(hWnd, msg, wParam, lParam);
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

HRESULT L3DEngine::CreateL3DWindow(HWND* pWnd, HINSTANCE hInstance)
{
	BOOL bRetCode = FALSE;
	HRESULT hResult = E_FAIL;
	WNDCLASSEX wndClassEx;

	do
	{
		wndClassEx.cbSize        = sizeof(WNDCLASSEX);
		wndClassEx.style         = CS_HREDRAW | CS_VREDRAW;
		wndClassEx.lpfnWndProc   = (WNDPROC)L3DEngine::MsgProc;        //The WNDPROC type defines a pointer to this callback function
		wndClassEx.cbClsExtra    = 0;	//The system initializes the bytes to zero                      
		wndClassEx.cbWndExtra    = 0;	//The system initializes the bytes to zero
		wndClassEx.hInstance     = hInstance;
		//wndClassEx.hInstance     = GetModuleHandle(NULL);
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
	D3DCAPS9 Caps9;
	D3DPRESENT_PARAMETERS PresentParam;
	int nVertexProcessing = 0;

	do
	{
		m_p3D9->GetDeviceCaps(uAdapter, eDeviceType, &Caps9);
		nVertexProcessing = (Caps9.DevCaps && D3DDEVCAPS_HWTRANSFORMANDLIGHT) ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		PresentParam.BackBufferWidth = 800;
		PresentParam.BackBufferHeight = 600;
		PresentParam.BackBufferFormat = D3DFMT_A8R8G8B8; //像素格式
		PresentParam.BackBufferCount = 1;
		PresentParam.MultiSampleType= D3DMULTISAMPLE_NONE;
		PresentParam.MultiSampleQuality = 0;
		PresentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
		PresentParam.hDeviceWindow = hWnd;
		PresentParam.Windowed = m_WindowParam.bWindow;
		PresentParam.EnableAutoDepthStencil = TRUE;
		PresentParam.AutoDepthStencilFormat = D3DFMT_D24S8; // depth format
		PresentParam.Flags = 0;
		PresentParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		PresentParam.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		hr = m_p3D9->CreateDevice(uAdapter, eDeviceType, hWnd, nVertexProcessing, &PresentParam, &m_p3DDevice);
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	}
	while(0);

	return hResult;
}

HRESULT L3DEngine::Display(float fDeltaTime)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	D3DXMATRIX Rx;
	D3DXMATRIX Ry;
	static float y = 0.0f;

	do 
	{
		BOOL_ERROR_BREAK(m_p3DDevice);

		// 旋转立方体
			
		//x轴旋转45弧度
		D3DXMatrixRotationX(&Rx, 3.14f / 4.0f);
		// 每一帧中增加y轴的弧度
		
		D3DXMatrixRotationY(&Ry, y);
		y += fDeltaTime;
		//当y轴旋转2周时，重新回到0弧度
		if(y >= 6.28f)
			y = 0.0f;
		// 结合x轴与y轴的旋转矩阵
		D3DXMATRIX p = Rx * Ry;
		m_p3DDevice->SetTransform(D3DTS_WORLD, &p);

		hr = m_p3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		HRESULT_ERROR_BREAK(hr);

		m_p3DDevice->BeginScene();// 开始绘制场景
		m_p3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Vertex));// 设置资源流
		m_p3DDevice->SetIndices(m_pIndexBuffer); // 设置索引缓存
		m_p3DDevice->SetFVF(D3DFVF_XYZ); // 设置顶点格式
		// 利用索引缓存绘制
		m_p3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,0,8,0,12);
		m_p3DDevice->EndScene();// 结束绘制场景

		hr = m_p3DDevice->Present(0, 0, 0, 0);
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;

	} while (0);

	return hResult;
}

HRESULT L3DEngine::EnterMsgLoop(HRESULT (L3DEngine::* pfnDisplay)(float fDeltaTime))
{
	float fCurTime = 0;
	float fDeltaTime = 0;
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
			(this->*pfnDisplay)(fDeltaTime);
		}
		m_fLastTime = fCurTime;
	}
	return Msg.wParam;
}
