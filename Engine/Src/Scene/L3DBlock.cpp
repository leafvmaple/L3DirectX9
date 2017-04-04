#include "L3DBlock.h"
#include "IO/LFileReader.h"
#include "LAssert.h"

L3DBlock::L3DBlock()
{

}

L3DBlock::~L3DBlock()
{

}

HRESULT L3DBlock::LoadBlock(BYTE*& pbySceneBlock)
{
	DWORD dwNum = 0;
	POINT Index;

	do 
	{
		for (int s = 3; s >= 3; s--)
		{
			pbySceneBlock = LFileReader::Convert(pbySceneBlock, dwNum);

			for (DWORD i = 0; i < dwNum; i++)
			{
				pbySceneBlock = LFileReader::Convert(pbySceneBlock, Index);
				CreateBlock(s, Index);
			}
		}

	} while (0);

	return S_OK;
}

HRESULT L3DBlock::CreateBlock(int nLevel, POINT Index)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	LBlockData* pBlockData = NULL;

	do 
	{
		pBlockData = GetBlock(nLevel, Index);
		BOOL_SUCCESS_BREAK(pBlockData);

		pBlockData = new LBlockData;
		BOOL_ERROR_BREAK(pBlockData);

		ZeroMemory(pBlockData, sizeof(LBlockData));

		pBlockData->nIndexX = Index.x;
		pBlockData->nIndexZ = Index.y;
		pBlockData->nLevel  = nLevel;
		pBlockData->bDataInMemory = TRUE;
		pBlockData->nLastUseTime = timeGetTime();

		m_mapBlockData[nLevel][Index] = pBlockData;

		hResult = S_OK;
	} while (0);

	return hResult;
}

LBlockData* L3DBlock::GetBlock(int nLevel, POINT Index)
{
	std::map<POINT, LBlockData*>::iterator iFind;

	iFind = m_mapBlockData[nLevel].find(Index);
	if(iFind == m_mapBlockData[nLevel].end())
		return NULL;
	else
		return iFind->second;
}