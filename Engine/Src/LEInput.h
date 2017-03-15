#pragma once
#include <dinput.h>
#include "L3DInterface.h"

class LEInput
{
public:
	LEInput();
	~LEInput();

	HRESULT Init(HWND hWnd, HINSTANCE hInstance, DWORD dwKeyboardCoopFlags, DWORD dwMouseCoopFlags);
	HRESULT Uninit();

	HRESULT Update();

	bool IsKeyDown(int nKey);
	bool IsMouseButtonDown(int nButton);
	float MouseDX();
	float MouseDY();
	float MouseDZ();

public:
	IDirectInput8* m_pDirectInput;
	IDirectInputDevice8* m_KeyboardDevice;
	IDirectInputDevice8* m_MouseDevice;
	DIMOUSESTATE m_MouseState;
	char m_keyBuffer[256];
};