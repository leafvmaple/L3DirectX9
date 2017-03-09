#include "LEFont.h"
#include "LAssert.h"

LEFont::LEFont()
: m_pFont(NULL)
, m_nSize(0)
{
	m_Color = L3D::BLACK;
	ZeroMemory(&m_Rect, sizeof(m_Rect));
	ZeroMemory(m_wszFPS, sizeof(m_wszFPS));
}

LEFont::~LEFont()
{

}

HRESULT LEFont::Init(IDirect3DDevice9* p3DDevice, int nSize/* = 9*/)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	HDC hDC = NULL;

	do
	{
		hDC = GetDC(NULL);
		m_nSize = (MulDiv(nSize, GetDeviceCaps(hDC, LOGPIXELSY), 72));

		hr = D3DXCreateFont(p3DDevice, m_nSize, 0, FW_NORMAL, 0, FALSE, 
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
			DEFAULT_PITCH | FF_DONTCARE, TEXT("����"), 
			&m_pFont);
		HRESULT_ERROR_BREAK(hr);

		ReleaseDC(NULL, hDC);

		m_Rect.bottom = m_Rect.top + m_nSize;

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT LEFont::SetText(LPCWSTR szString)
{
	m_Rect.right = m_Rect.left + wcslen(szString) * m_nSize;
	wcscpy_s(m_wszFPS, szString);
	return S_OK;
}

HRESULT LEFont::SetColor(const D3DXCOLOR& color)
{
	m_Color = color;
	return S_OK;
}

HRESULT LEFont::SetPosition(int nX, int nY)
{
	int nWidth = 0;
	int nHeight = 0;

	nWidth = m_Rect.right - m_Rect.left;
	nHeight = m_Rect.bottom - m_Rect.top;

	SetRect(&m_Rect, nX, nY, nX + nWidth, nY + nHeight);
	return S_OK;
}

HRESULT LEFont::UpdateDisplay()
{
	m_pFont->DrawText(NULL, m_wszFPS, -1, &m_Rect, DT_TOP | DT_LEFT, m_Color);
	return S_OK;
}
