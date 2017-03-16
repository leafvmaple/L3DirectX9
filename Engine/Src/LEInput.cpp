#include "LEInput.h"
#include "LAssert.h"

LEInput::LEInput()
: m_pDirectInput(NULL)
, m_KeyboardDevice(NULL)
, m_MouseDevice(NULL)
{
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
	ZeroMemory(&m_keyBuffer, sizeof(m_keyBuffer));
}

LEInput::~LEInput()
{

}

HRESULT LEInput::Init(HWND hWnd, HINSTANCE hInstance, DWORD dwKeyboardCoopFlags, DWORD dwMouseCoopFlags)
{
	HRESULT hr;
	HRESULT hResult = E_FAIL;

	do 
	{
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
		HRESULT_ERROR_BREAK(hr);

		hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_KeyboardDevice, NULL);
		HRESULT_ERROR_BREAK(hr);

		hr = m_KeyboardDevice->SetCooperativeLevel(hWnd, dwKeyboardCoopFlags);
		HRESULT_ERROR_BREAK(hr);

		hr = m_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
		HRESULT_ERROR_BREAK(hr);

		hr = m_KeyboardDevice->Acquire();
		HRESULT_ERROR_BREAK(hr);

		hr = m_KeyboardDevice->Poll();
		HRESULT_ERROR_BREAK(hr);

		hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_MouseDevice, NULL);
		HRESULT_ERROR_BREAK(hr);

		hr = m_MouseDevice->SetCooperativeLevel(hWnd ,dwMouseCoopFlags);
		HRESULT_ERROR_BREAK(hr);

		hr = m_MouseDevice->SetDataFormat(&c_dfDIMouse);
		HRESULT_ERROR_BREAK(hr);

		hr = m_MouseDevice->Acquire();
		HRESULT_ERROR_BREAK(hr);

		hr = m_KeyboardDevice->Poll();
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT LEInput::Uninit()
{
	if(m_KeyboardDevice)
		m_KeyboardDevice->Unacquire();
	if(m_MouseDevice)
		m_MouseDevice->Unacquire();

	SAFE_RELEASE(m_KeyboardDevice);
	SAFE_RELEASE(m_MouseDevice);
	SAFE_RELEASE(m_pDirectInput);

	return S_OK;
}

HRESULT LEInput::Update()
{
	HRESULT hr = E_FAIL;

	hr = m_KeyboardDevice->GetDeviceState(sizeof(m_keyBuffer), (void**)&m_keyBuffer); 
	if(hr)
	{
		m_KeyboardDevice->Acquire();  
		m_KeyboardDevice->GetDeviceState(sizeof(m_keyBuffer),(LPVOID)m_keyBuffer );
	}

	hr = m_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&m_MouseState); 
	if(hr)
	{
		m_MouseDevice->Acquire();
		m_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&m_MouseState);
	}

	return S_OK;
}

bool LEInput::IsKeyDown(int nKey)
{
	if(m_keyBuffer[nKey] & 0x80)
		return true;
	else
		return false;
}

bool LEInput::IsMouseButtonDown(int nButton)
{
	return (m_MouseState.rgbButtons[nButton] & 0x80) != 0;
}

float LEInput::MouseDX()
{
	return (float)m_MouseState.lX;
}

float LEInput::MouseDY()
{
	return (float)m_MouseState.lY;
}

float LEInput::MouseDZ()
{
	return (float)m_MouseState.lZ;
}