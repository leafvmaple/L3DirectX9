#include "LScene.h"
#include "LAssert.h"

LScene::LScene()
: m_LpScene(NULL)
, m_p3DDevice(NULL)
{

}

HRESULT LScene::Create(IL3DEngine* p3DEngine, LPDIRECT3DDEVICE9 p3DDevice, TCHAR* pwszMeshPath)
{
	HRESULT hr = E_FAIL;

	do
	{
		hr = ILScene::Create(p3DEngine, pwszMeshPath, &m_LpScene);
		HRESULT_ERROR_BREAK(hr);

		m_p3DDevice = p3DDevice;

	} while (0);

	return S_OK;
}
