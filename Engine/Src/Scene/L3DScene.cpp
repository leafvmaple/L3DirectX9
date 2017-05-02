#include "L3DScene.h"
#include "L3DTerrain.h"
#include "L3DEntity.h"
#include "L3DBlock.h"
#include "LAssert.h"
#include "IO/LFileReader.h"

L3DScene::L3DScene()
: m_pLTerrain(NULL)
, m_pLEntity(NULL)
, m_pLBlock(NULL)
{

}

L3DScene::~L3DScene()
{

}

HRESULT L3DScene::Init(LPCWSTR pcszFileName)
{
	HRESULT hr = E_FAIL;
	BYTE* pbyMapFile = NULL;
	size_t uLen = 0;
	MapFileHeader* pFileHeader = NULL;

	do
	{
		LFileReader::Reader(pcszFileName, &pbyMapFile, &uLen);
		BOOL_ERROR_BREAK(pbyMapFile);

		pbyMapFile = LFileReader::Convert(pbyMapFile, pFileHeader);
		BOOL_ERROR_BREAK(pFileHeader->dwMask == 0x10203040);

		m_pLEntity = new L3DEntity;
		BOOL_ERROR_BREAK(m_pLEntity);

		hr = m_pLEntity->LoadEntity(pcszFileName);
		HRESULT_ERROR_BREAK(hr);

		m_pLTerrain = new L3DTerrain;
		BOOL_ERROR_BREAK(m_pLTerrain);

		hr = m_pLTerrain->LoadTerrain(pcszFileName);
		HRESULT_ERROR_BREAK(hr);

		m_pLBlock = new L3DBlock;
		BOOL_ERROR_BREAK(m_pLBlock);

		hr = m_pLBlock->LoadBlock(pbyMapFile);
		HRESULT_ERROR_BREAK(hr);

	} while (0);

	return S_OK;
}

HRESULT L3DScene::UpdateDisplay()
{
	if (m_pLTerrain)
		m_pLTerrain->UpdateTerrain();

	return S_OK;
}
