#include <Windows.h>
#include "LAssert.h"
#include "LExports.h"
#include "L3DInterface.h"
#include "LObjectMgr.h"
#include "Object/LCube.h"
#include "Object/LTeapot.h"

class L3DENGINE_CLASS IL3DEngine;

class LClient
{
public:
	LClient();
	~LClient();

	HRESULT Init(HINSTANCE hInstance);
	HRESULT Update();
	HRESULT Uninit();

	BOOL IsActive();
protected:
private:
	LObjectMgr* m_pObjectMgr;
};