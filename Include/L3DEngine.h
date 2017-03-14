#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include "L3DInterface.h"

class ILModel;

class L3DEngine : public IL3DEngine
{
public:
	L3DEngine();
	virtual ~L3DEngine();

	virtual HRESULT Init(HINSTANCE hInstance, L3DWINDOWPARAM& WindowParam);
	virtual HRESULT Uninit();
	
	virtual HRESULT Update(float fDeltaTime);

	virtual HRESULT SetResourceDir(LPCWSTR lpResourceDir);

	virtual BOOL IsActive();
	virtual HRESULT GetDevice(IDirect3DDevice9** pp3DDevice);

	static struct CameraParam
	{
		float fSightDis;
		float fYaw;
		float fPitch;
		float fRoll;
		D3DXVECTOR3 vPositon;
		D3DXVECTOR3 vTarget;
		D3DXVECTOR3 vUp;
	} m_Camera;

public:
	HRESULT AttachObject(ILModel* pAction);
	HRESULT AttachFont(ILFont* pFont);

private:
	struct SampFilter
	{
		DWORD dwAnisotropy;
		int nMipFilter;
		int nMinFilter;
		int nMagFilter;
	};
	
	WCHAR m_szResourceDir[FILENAME_MAX];
	IDirect3D9* m_p3D9;
	IDirect3DDevice9* m_p3DDevice;

	L3DWINDOWPARAM m_WindowParam;
	SampFilter m_SampFilter[GRAPHICS_LEVEL_COUNT];
	SampFilter& m_CurSampFilter;

	BOOL m_bActive;
	D3DCAPS9 m_Caps9;
	D3DPRESENT_PARAMETERS m_PresentParam;
	std::vector<ADAPTERMODE> m_AdapterModes;
	std::list<ILModel*> m_ModelList;
	std::list<ILFont*> m_FontList;

private:
	static LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HRESULT InitPresentParam(HWND hWnd);
	HRESULT InitSamplerFilter(UINT uAdapter, D3DDEVTYPE eDeviceType);
	HRESULT InitCameraParam();

	HRESULT GetL3DAdapter(PUINT puAdapter, D3DDEVTYPE* pDeviceType);
	HRESULT GetL3DAdapterMode(UINT uAdapter);
	HRESULT CreateL3DWindow(HWND* pWnd, HINSTANCE hInstance);
	HRESULT CreateL3DDevice(UINT uAdapter, D3DDEVTYPE eDeviceType, HWND hWnd);

	HRESULT UpdateMessage();
	HRESULT UpdateCamera(float fDeltaTime);
};