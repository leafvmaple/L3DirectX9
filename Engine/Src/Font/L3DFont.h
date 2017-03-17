#include "L3DInterface.h"

class L3DFont : public ILFont
{
public:
	L3DFont();
	virtual ~L3DFont();

	virtual HRESULT SetText(LPCWSTR szString);
	virtual HRESULT SetColor(const D3DXCOLOR& color);
	virtual HRESULT SetPosition(int nX, int nY);

public:
	HRESULT Init(IDirect3DDevice9* p3DDevice, int nSize = 9);
	HRESULT UpdateDisplay();

private:
	ID3DXFont* m_pFont;
	WCHAR m_wszFPS[LENGIEN_FONT_STRING_MAX];

	int m_nSize;
	RECT m_Rect;
	D3DXCOLOR m_Color;
};