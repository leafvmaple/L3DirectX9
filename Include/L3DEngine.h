#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include "L3DInterface.h"

class ILObject;

class L3DEngine : public IL3DEngine
{
public:
	L3DEngine();
	virtual ~L3DEngine();

	virtual HRESULT Init(HINSTANCE hInstance, L3DWINDOWPARAM& WindowParam);
	virtual HRESULT Uninit();

	virtual HRESULT AttachObject(ILObject* pAction);
	virtual HRESULT Update(float fDeltaTime);

	virtual BOOL IsActive();

	virtual HRESULT GetDevice(IDirect3DDevice9** pp3DDevice);

private:
	struct _CameraParam
	{
		float fSightDis;
		float fYaw;
		float fPitch;
		float fRoll;
		D3DXVECTOR3 vPositon;
		D3DXVECTOR3 vTarget;
		D3DXVECTOR3 vUp;
	} m_Camera;

	struct SampFilter
	{
		DWORD dwAnisotropy;
		int nMipFilter;
		int nMinFilter;
		int nMagFilter;
	};

	BOOL m_bActive;
	IDirect3D9* m_p3D9;
	IDirect3DDevice9* m_p3DDevice;

	L3DWINDOWPARAM m_WindowParam;
	SampFilter m_SampFilter[GRAPHICS_LEVEL_COUNT];
	SampFilter& m_CurSampFilter;

	D3DCAPS9 m_Caps9;
	D3DPRESENT_PARAMETERS m_PresentParam;
	std::vector<ADAPTERMODE> m_AdapterModes;
	std::list<ILObject*> m_ActionList;

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

	HRESULT UpdateMessage(MSG* pMsg);
	HRESULT UpdateCamera(float fDeltaTime);
};