#include <Windows.h>
#include "LAssert.h"
#include "LExports.h"
#include "L3DInterface.h"
#include "LObjectMgr.h"
#include "Object/LCube.h"
#include "Object/LTeapot.h"
#include "Object/LCharacter.h"

class L3DENGINE_CLASS IL3DEngine;

class LClient
{
public:
	LClient();
	~LClient();

	HRESULT Init(HINSTANCE hInstance);
	HRESULT Update();
	void Uninit();

	BOOL IsActive();

	HRESULT ShowFPS(float fDeltaTime);
protected:
private:
	float m_fLastTime;
	float m_fTimeElapsed;
	int m_nFrame;
	ILFont* m_pFont;
	LObjectMgr* m_pObjectMgr;
};