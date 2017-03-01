#include <Windows.h>
#include <strsafe.h>
#include "LAssert.h"
#include "L3DEngine.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

DWORD Vertex::VERTEX_FVF = D3DFVF_XYZ;
DWORD LightVertex::LIGHT_VERTEX_FVF = Vertex::VERTEX_FVF | D3DFVF_NORMAL;
DWORD ColorVertex::COLOR_VERTEX_FVF = LightVertex::LIGHT_VERTEX_FVF | D3DFVF_DIFFUSE;
DWORD TexVertex::TEX_VERTEX_FVF = ColorVertex::COLOR_VERTEX_FVF | D3DFVF_TEX1;

void L3D::InitVertexNormal(LightVertex* pVertexs)
{
	D3DXVECTOR3 vNormal;
	D3DXVECTOR3 u = pVertexs[0] - pVertexs[1];
	D3DXVECTOR3 v = pVertexs[1] - pVertexs[2];
	D3DXVec3Cross(&vNormal, &u, &v);
	D3DXVec3Normalize(&vNormal, &vNormal);
	pVertexs[0].SetNormal(vNormal);
	pVertexs[1].SetNormal(vNormal);
	pVertexs[2].SetNormal(vNormal);
}

D3DLIGHT9 L3D::InitDirectionalLight(const D3DXVECTOR3& vDirection, const D3DXCOLOR& color)
{
	D3DLIGHT9 Light;
	::ZeroMemory(&Light, sizeof(Light));
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Ambient = color * 0.4f;
	Light.Diffuse = color;
	Light.Specular = color * 0.6f;
	Light.Direction = vDirection;
	return Light;
}

L3DEngine::L3DEngine()
: m_p3D9(NULL)
, m_p3DDevice(NULL)
, m_pVertexBuffer(NULL)
, m_pIndexBuffer(NULL)
, m_fLastTime(0.f)
{
	memset(&m_Caps9, 0, sizeof(m_Caps9));
	memset(&m_SampFilter, 0, sizeof(m_SampFilter));
	memset(&m_WindowParam, 0, sizeof(m_WindowParam));
	memset(&m_PresentParam, 0, sizeof(m_PresentParam));

	m_AdapterModes.clear();

	m_fAngleX = D3DX_PI / 4.0f;
	m_fAngleY = 0;
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

		hr = InitPresentParam(hWnd);
		HRESULT_ERROR_BREAK(hr);

		hr = InitTextureSamplerFilter(uAdapter, eDeviceType);
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
	ColorVertex* pVertices = NULL;
	WORD* pwIndices = NULL;
	IDirect3DTexture9* pTexture = NULL;
	D3DMATERIAL9 Material = {L3D::WHITE, L3D::WHITE, L3D::WHITE, L3D::BLACK, 5.f};
	D3DLIGHT9 DirectionalLight;
	D3DXVECTOR3 vPosition(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 vTarget(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMATRIX matCamera;
	D3DXMATRIX matProj;

	do 
	{
		hr = m_p3DDevice->CreateVertexBuffer(
			8 * sizeof(ColorVertex), D3DUSAGE_WRITEONLY,
			ColorVertex::COLOR_VERTEX_FVF , D3DPOOL_MANAGED, &m_pVertexBuffer, 0);
		HRESULT_ERROR_BREAK(hr);

		m_p3DDevice->CreateIndexBuffer(
			36 * sizeof(WORD), D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, 0);
		HRESULT_ERROR_BREAK(hr);

		m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

		pVertices[0] = ColorVertex(-1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255, 0, 0));
		pVertices[1] = ColorVertex(-1.0f,  1.0f, -1.0f, D3DCOLOR_XRGB(0, 255, 0));
		pVertices[2] = ColorVertex( 1.0f,  1.0f, -1.0f, D3DCOLOR_XRGB(0, 0, 255));
		pVertices[3] = ColorVertex( 1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255, 0, 0));
		pVertices[4] = ColorVertex(-1.0f, -1.0f,  1.0f, D3DCOLOR_XRGB(0, 255, 0));
		pVertices[5] = ColorVertex(-1.0f,  1.0f,  1.0f, D3DCOLOR_XRGB(0, 0, 255));
		pVertices[6] = ColorVertex( 1.0f,  1.0f,  1.0f, D3DCOLOR_XRGB(255, 0, 0));
		pVertices[7] = ColorVertex( 1.0f, -1.0f,  1.0f, D3DCOLOR_XRGB(0, 255, 0));

		m_pVertexBuffer->Unlock();

		//L3D::InitVertexNormal(&pVertices[0]);
		//L3D::InitVertexNormal(&pVertices[3]);

		m_p3DDevice->SetMaterial(&Material);

		DirectionalLight = L3D::InitDirectionalLight(D3DXVECTOR3(1.f, 0.f, 0.f), L3D::WHITE);
		m_p3DDevice->SetLight(0, &DirectionalLight);
		m_p3DDevice->LightEnable(0, TRUE);

		// �����������������
		m_pIndexBuffer->Lock(0, 0, (void**)&pwIndices, 0);

		pwIndices[0] = 0; pwIndices[1] = 1; pwIndices[2] = 2;
		pwIndices[3] = 0; pwIndices[4] = 2; pwIndices[5] = 3;
		// ����
		pwIndices[6] = 4; pwIndices[7] = 6; pwIndices[8] = 5;
		pwIndices[9] = 4; pwIndices[10] = 7; pwIndices[11] = 6;
		// ����
		pwIndices[12] = 4; pwIndices[13] = 5; pwIndices[14] = 1;
		pwIndices[15] = 4; pwIndices[16] = 1; pwIndices[17] = 0;
		// ����
		pwIndices[18] = 3; pwIndices[19] = 2; pwIndices[20] = 6;
		pwIndices[21] = 3; pwIndices[22] = 6; pwIndices[23] = 7;
		// ����
		pwIndices[24] = 1; pwIndices[25] = 5; pwIndices[26] = 6;
		pwIndices[27] = 1; pwIndices[28] = 6; pwIndices[29] = 2;
		// �ײ�
		pwIndices[30] = 4; pwIndices[31] = 0; pwIndices[32] = 3;
		pwIndices[33] = 4; pwIndices[34] = 3; pwIndices[35] = 7;

		m_pIndexBuffer->Unlock();

		D3DXCreateTextureFromFile(m_p3DDevice, TEXT("res/metal_max.png"), &pTexture);
		BOOL_ERROR_BREAK(pTexture);

		//hr = m_p3DDevice->SetTexture(0, pTexture);
		//HRESULT_ERROR_BREAK(hr);

		// ��ͼ����
		D3DXMatrixLookAtLH(&matCamera, &vPosition, &vTarget, &vUp);
		m_p3DDevice->SetTransform(D3DTS_VIEW, &matCamera);

		// ͶӰ����
		D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI * 0.5f, (float)m_WindowParam.Width / (float)m_WindowParam.Height, 1.0f, 1000.0f);
		m_p3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

		// ��Ⱦ״̬
		//m_p3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_p3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_p3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		m_p3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

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

HRESULT L3DEngine::InitTextureSamplerFilter(UINT uAdapter, D3DDEVTYPE eDeviceType)
{
	m_p3D9->GetDeviceCaps(uAdapter, eDeviceType, &m_Caps9);

	m_SampFilter.dwMinAnisotropy = 1;
	m_SampFilter.dwMaxAnisotropy = m_Caps9.MaxAnisotropy;

	m_SampFilter.nMinMipFilter = D3DTEXF_POINT;
	m_SampFilter.nMinMinFilter = D3DTEXF_POINT;
	m_SampFilter.nMinMagFilter = D3DTEXF_POINT;

	m_SampFilter.nMaxMipFilter = D3DTEXF_POINT;
	m_SampFilter.nMaxMinFilter = D3DTEXF_POINT;
	m_SampFilter.nMaxMagFilter = D3DTEXF_POINT;

	if (m_SampFilter.dwMaxAnisotropy <= 1)
	{
		if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR)
			m_SampFilter.nMaxMipFilter = D3DTEXF_LINEAR;

		if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR)
			m_SampFilter.nMaxMinFilter = D3DTEXF_LINEAR;

		if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR)
			m_SampFilter.nMaxMagFilter = D3DTEXF_LINEAR;
	}
	else
	{
		if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR)
			m_SampFilter.nMaxMipFilter = D3DTEXF_LINEAR;

		if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC)
			m_SampFilter.nMaxMinFilter = D3DTEXF_ANISOTROPIC;
		else if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR)
			m_SampFilter.nMaxMinFilter = D3DTEXF_LINEAR;

		if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC)
			m_SampFilter.nMaxMagFilter = D3DTEXF_ANISOTROPIC;
		else if (m_Caps9.TextureFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR)
			m_SampFilter.nMaxMagFilter = D3DTEXF_LINEAR;
	}
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
	//int nVertexProcessing = 0;

	do
	{
		//nVertexProcessing = (m_Caps9.DevCaps && D3DDEVCAPS_HWTRANSFORMANDLIGHT) ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		hr = m_p3D9->CreateDevice(uAdapter, eDeviceType, hWnd, D3DCREATE_MIXED_VERTEXPROCESSING, &m_PresentParam, &m_p3DDevice);
		HRESULT_SUCCESS_BREAK(hr);

		hr = m_p3D9->CreateDevice(uAdapter, eDeviceType, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_PresentParam, &m_p3DDevice);
		HRESULT_SUCCESS_BREAK(hr);

		hr = m_p3D9->CreateDevice(uAdapter, eDeviceType, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_PresentParam, &m_p3DDevice);
		HRESULT_SUCCESS_BREAK(hr);

		hResult = S_OK;
	}
	while(0);

	return hResult;
}

HRESULT L3DEngine::Display(float fDeltaTime)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	D3DXMATRIX matX;
	D3DXMATRIX matY;
	D3DXMATRIX matTransform;

	do 
	{
		BOOL_ERROR_BREAK(m_p3DDevice);

		D3DXMatrixRotationX(&matX, m_fAngleX);
		D3DXMatrixRotationY(&matY, m_fAngleY);

		m_fAngleY += fDeltaTime;
		if(m_fAngleY >= 6.28f)
			m_fAngleY = 0.0f;

		matTransform = matX * matY;
		m_p3DDevice->SetTransform(D3DTS_WORLD, &matTransform);

		hr = m_p3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		HRESULT_ERROR_BREAK(hr);

		m_p3DDevice->BeginScene();
		m_p3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(ColorVertex));
		m_p3DDevice->SetIndices(m_pIndexBuffer);
		m_p3DDevice->SetFVF(ColorVertex::COLOR_VERTEX_FVF);
		//m_p3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);

		// ���ƴ��������ڴ��е�12������
		m_p3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
		m_p3DDevice->EndScene();

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
