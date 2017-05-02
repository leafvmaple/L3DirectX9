#include "L3DFont.h"
#include "LAssert.h"

extern LPDIRECT3DDEVICE9 g_p3DDevice;

L3DFont::L3DFont()
: m_pFont(NULL)
, m_nSize(0)
{
	m_Color = L3D::BLACK;
	ZeroMemory(&m_Rect, sizeof(m_Rect));
	ZeroMemory(m_wszFPS, sizeof(m_wszFPS));
}

L3DFont::~L3DFont()
{

}

HRESULT L3DFont::Init(int nSize/* = 9*/)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	HDC hDC = NULL;

	do
	{
		hDC = GetDC(NULL);
		m_nSize = (MulDiv(nSize, GetDeviceCaps(hDC, LOGPIXELSY), 72));

		hr = D3DXCreateFont(g_p3DDevice, m_nSize, 0, FW_NORMAL, 0, FALSE, 
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
			DEFAULT_PITCH | FF_DONTCARE, TEXT("ËÎÌו"), 
			&m_pFont);
		HRESULT_ERROR_BREAK(hr);

		ReleaseDC(NULL, hDC);

		m_Rect.bottom = m_Rect.top + m_nSize;

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT L3DFont::SetText(LPCWSTR szString)
{
	m_Rect.right = m_Rect.left + wcslen(szString) * m_nSize;
	wcscpy_s(m_wszFPS, szString);
	return S_OK;
}

HRESULT L3DFont::SetColor(const D3DXCOLOR& color)
{
	m_Color = color;
	return S_OK;
}

HRESULT L3DFont::SetPosition(int nX, int nY)
{
	int nWidth = 0;
	int nHeight = 0;

	nWidth = m_Rect.right - m_Rect.left;
	nHeight = m_Rect.bottom - m_Rect.top;

	SetRect(&m_Rect, nX, nY, nX + nWidth, nY + nHeight);
	return S_OK;
}

HRESULT L3DFont::UpdateDisplay()
{
	m_pFont->DrawText(NULL, m_wszFPS, -1, &m_Rect, DT_TOP | DT_LEFT, m_Color);
	return S_OK;
}
