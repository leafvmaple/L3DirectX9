#include "L3DScene.h"
#include "L3DTerrain.h"
#include "LAssert.h"

L3DScene::L3DScene()
: m_pLTerrain(NULL)
{

}

L3DScene::~L3DScene()
{

}

HRESULT L3DScene::LoadScene(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR pcszFileName)
{
	HRESULT hr = E_FAIL;

	do 
	{
		m_pLTerrain = new L3DTerrain;
		BOOL_ERROR_BREAK(m_pLTerrain);

		hr = m_pLTerrain->LoadTerrain(p3DDevice, pcszFileName);
		HRESULT_ERROR_BREAK(hr);

	} while (0);

	return S_OK;
}
