#include <cstdio>
#include <atlconv.h>
#include "L3DTerrain.h"
#include "LAssert.h"
#include "IO/LFileReader.h"
#include "Model/L3DTexture.h"
#include "L3DEngine.h"

L3DTerrain::L3DTerrain()
: m_p3DDevice(NULL)
, m_bEnableTerrainConverMap(0)
, m_pTerrainCoverMap(NULL)
{

}

L3DTerrain::~L3DTerrain()
{

}


HRESULT LTerrainConverMap::LoadConverMapBuffer(BYTE* pbyConverMap, DWORD dwLen)
{
	do
	{
		pbyConverMap = LFileReader::Copy(pbyConverMap, m_szTextureFileName, MAX_PATH);
		pbyConverMap = LFileReader::Copy(pbyConverMap, m_szHeightColorFileName, MAX_PATH);
		pbyConverMap = LFileReader::Convert(pbyConverMap, m_vRect);
		pbyConverMap = LFileReader::Convert(pbyConverMap, m_fLowest);
		pbyConverMap = LFileReader::Convert(pbyConverMap, m_fHeightTotal);

		m_vRect.z = abs(m_vRect.z);
		m_vRect.w = abs(m_vRect.w);

		m_pTexture = new L3DTexture;
		m_pTexture->LoadLTexture(L3DEngine::Instance()->GetDevice(), m_szTextureFileName);

		/*
		if(!m_lpTextureLight)
		{
			g_cTextureTable.LoadResourceFromFile("data\\source\\other\\特效\\贴图\\GX_光效\\GQ_光球.tga",0,0,(IKG3DResourceBase**)&m_lpTextureLight);
			//g_cTextureTable.LoadResourceFromFile("data\\source\\other\\特效\\贴图\\GX_光效\\XX_X星星.tga",0,0,(IKG3DResourceBase**)&m_lpTextureLight);
		}
		if(!m_lpTextureCloud)
		{
			g_cTextureTable.LoadResourceFromFile("data\\public\\defaultscene\\cloud.tga",0,0,(IKG3DResourceBase**)&m_lpTextureCloud);
			//g_cTextureTable.LoadResourceFromFile("data\\source\\other\\特效\\贴图\\GX_光效\\XX_X星星.tga",0,0,(IKG3DResourceBase**)&m_lpTextureLight);
		}

		D3DVIEWPORT9 viewPort;
		hr = g_pd3dDevice->GetViewport(&viewPort);
		KGLOG_COM_PROCESS_ERROR(hr);
		CreatePointLightTextures(viewPort.Width,viewPort.Height);

		ComputeHeightMapSize();
		*/

	} while (0);

	//pFile->Reset();
	return S_OK;
}

HRESULT L3DTerrain::LoadTerrain(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR cszFileName)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	TCHAR szDrive[MAX_PATH];
	TCHAR szExt[MAX_PATH];
	TCHAR szDir[MAX_PATH];
	TCHAR szOnlyFileName[MAX_PATH];
	TCHAR szMapIniFileName[MAX_PATH];
	TCHAR szFileName[MAX_PATH];
	TCHAR wszFileDir[MAX_PATH];

	do
	{
		ZeroMemory(szFileName, sizeof(szFileName));

		_wsplitpath_s(cszFileName, szDrive, MAX_PATH, szDir, MAX_PATH, szOnlyFileName, MAX_PATH, szExt, MAX_PATH);
		swprintf_s(wszFileDir, TEXT("%s%s%sFile/"), szDrive, szDir, szOnlyFileName);

		hr = LoadTerrainInfo(wszFileDir);
		HRESULT_ERROR_BREAK(hr);

		swprintf_s(szMapIniFileName, TEXT("%s%s%s_Setting.ini"), szDrive, szDir, szOnlyFileName);

		m_bEnableTerrainConverMap = GetPrivateProfileInt(TEXT("ConverMap"), TEXT("bTerrainAlphaConverEnable"), 0, szMapIniFileName);
		GetPrivateProfileString(TEXT("ConverMap"), TEXT("TerrainAlphaConverMapFile"), TEXT("AA"), szFileName, MAX_PATH, szMapIniFileName);

		m_p3DDevice = p3DDevice;

	} while (0);

	return S_OK;
}

HRESULT L3DTerrain::LoadTerrainInfo(LPCWSTR cszDirectory)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	TCHAR wszTerrainInfoName[MAX_PATH];
	BYTE* pbyTerrainInfo = NULL;
	BYTE* pbyBufferHead = NULL;
	size_t uTerrainLen = 0;

	DWORD dwVersion = 0;
	DWORD dwNum = 0;
	InfoFileData* pInfoData = NULL;
	LTerrainClip* pSceneDataClip = NULL;

	do
	{
		swprintf_s(wszTerrainInfoName, TEXT("%s\\map.TerrainInfo"),cszDirectory);

		LFileReader::Reader(wszTerrainInfoName, &pbyTerrainInfo, &uTerrainLen);
		BOOL_ERROR_BREAK(pbyTerrainInfo);
		pbyBufferHead = pbyTerrainInfo;

		pbyTerrainInfo = LFileReader::Convert(pbyTerrainInfo, dwVersion);
		assert(dwVersion == MAP_FILE_VERSION_PACKREGION);

		pbyTerrainInfo = LFileReader::Convert(pbyTerrainInfo, dwNum);
		BOOL_SUCCESS_BREAK(!dwNum);

		pbyTerrainInfo = LFileReader::Convert(pbyTerrainInfo, pInfoData, dwNum);
		for (DWORD i = 0; i < dwNum; i++)
		{
			pSceneDataClip = new LTerrainClip;
			BOOL_ERROR_BREAK(pSceneDataClip);

			hr = LoadClipBuffer(pSceneDataClip, pbyBufferHead + pInfoData[i].dwFilePos);
			HRESULT_ERROR_BREAK(hr);

			m_TerrainInformations.insert(std::pair<UINT, LTerrainClip*>(pInfoData[i].uHandle, pSceneDataClip));
		}

		hResult = S_OK;
	} while (0);

	if (!SUCCEEDED(hr) && pSceneDataClip)
		SAFE_DELETE(pSceneDataClip);

	return S_OK;
}

HRESULT L3DTerrain::LoadClipBuffer(LTerrainClip* pLSceneDataClip, BYTE* pbyTerrain)
{
	HRESULT hr = E_FAIL;

	do
	{
		pbyTerrain = LFileReader::Convert(pbyTerrain, pLSceneDataClip->dwType);
		pbyTerrain = LFileReader::Convert(pbyTerrain, pLSceneDataClip->dwLength);

		pLSceneDataClip->pbyBuffer = new BYTE[pLSceneDataClip->dwLength];
		BOOL_ERROR_BREAK(pLSceneDataClip->pbyBuffer);

		LFileReader::Convert(pbyTerrain, pLSceneDataClip->pbyBuffer, pLSceneDataClip->dwLength);

		pLSceneDataClip->nRefrence = 0;

		switch(pLSceneDataClip->dwType)
		{
		case Terrain_Info_Convermap:
			{
				DWORD dwType = 0;
				DWORD dwLen = 0;

				if (!m_pTerrainCoverMap)
					m_pTerrainCoverMap = new LTerrainConverMap;

				hr = m_pTerrainCoverMap->LoadConverMapBuffer(pLSceneDataClip->pbyBuffer, pLSceneDataClip->dwLength);
				break;
			}
			/*
		case Terrain_Info_DetailMtlMgr:
			{
				DWORD dwType = 0;
				DWORD dwLen = 0;
				hr = m_lpDataInterface->GetTerrainInformation(uInfoHandle,&dwType,&dwLen,pFile->GetBuffer());
				KGLOG_COM_PROCESS_ERROR(hr);
				m_lpDetailMtls->SetHandle(uInfoHandle);
				hr = m_lpDetailMtls->LoadFromInformationBuffer(dwLen,pFile);
				KGLOG_COM_PROCESS_ERROR(hr);
			}
			break;
		case Terrain_Info_GrassSet:
			{
				DWORD dwType = 0;
				DWORD dwLen = 0;
				hr = m_lpDataInterface->GetTerrainInformation(uInfoHandle,&dwType,&dwLen,pFile->GetBuffer());
				KGLOG_COM_PROCESS_ERROR(hr);
				m_uSetHandle[MAP_GRASS] = uInfoHandle;
				KG3DGrassSet* pGrassSet = m_lpSetPointer[MAP_GRASS];
				if(!pGrassSet)
				{
					hr = E_FAIL;
					pGrassSet = new KG3DGrassSet;
					KGLOG_PROCESS_ERROR(pGrassSet);
					pGrassSet->AddRef();
					pGrassSet->Init();
					m_lpSetPointer[MAP_GRASS] = pGrassSet;
				}

				hr = pGrassSet->LoadFromInformationBuffer(dwLen,pFile);
				KGLOG_COM_PROCESS_ERROR(hr);
			}
			break;
		case Terrain_Info_StoneSet:
			{
				DWORD dwType = 0;
				DWORD dwLen = 0;
				hr = m_lpDataInterface->GetTerrainInformation(uInfoHandle,&dwType,&dwLen,pFile->GetBuffer());
				KGLOG_COM_PROCESS_ERROR(hr);
				m_uSetHandle[MAP_STONE] = uInfoHandle;
				KG3DGrassSet* pStoneSet = m_lpSetPointer[MAP_STONE];
				if(!pStoneSet)
				{
					hr = E_FAIL;
					pStoneSet = new KG3DGrassSet;
					KGLOG_PROCESS_ERROR(pStoneSet);
					pStoneSet->AddRef();
					pStoneSet->Init();
					m_lpSetPointer[MAP_STONE] = pStoneSet;
				}

				hr = pStoneSet->LoadFromInformationBuffer(dwLen,pFile);
				KGLOG_COM_PROCESS_ERROR(hr);
			}
			break;
		case Terrain_Info_FrutexSet:
			{
				DWORD dwType = 0;
				DWORD dwLen = 0;
				hr = m_lpDataInterface->GetTerrainInformation(uInfoHandle,&dwType,&dwLen,pFile->GetBuffer());
				KGLOG_COM_PROCESS_ERROR(hr);
				m_uSetHandle[MAP_FRUTEX] = uInfoHandle;
				KG3DGrassSet* pFrutexSet = m_lpSetPointer[MAP_FRUTEX];
				if(!pFrutexSet)
				{
					hr = E_FAIL;
					pFrutexSet = new KG3DGrassSet;
					KGLOG_PROCESS_ERROR(pFrutexSet);
					pFrutexSet->AddRef();
					pFrutexSet->Init();
					m_lpSetPointer[MAP_FRUTEX] = pFrutexSet;
				}

				hr = pFrutexSet->LoadFromInformationBuffer(dwLen,pFile);
				KGLOG_COM_PROCESS_ERROR(hr);
			}
			break;
		case Terrain_Info_RoadMgr:
			{
				// 			DWORD dwType = 0;
				// 			DWORD dwLen = 0;
				// 			hr = m_lpDataInterface->GetTerrainInformation(uInfoHandle,&dwType,&dwLen,pFile->GetBuffer());
				// 			KGLOG_COM_PROCESS_ERROR(hr);
				// 			m_lpRoadMgr->m_uHandle = uInfoHandle;
				// 			hr = m_lpRoadMgr->LoadFromInformationBuffer(dwLen,pFile);
				// 			KGLOG_COM_PROCESS_ERROR(hr);
			}
			break;
		case Terrain_Info_RiverMgr:
			{
				// 			DWORD dwType = 0;
				// 			DWORD dwLen = 0;
				// 			hr = m_lpDataInterface->GetTerrainInformation(uInfoHandle,&dwType,&dwLen,pFile->GetBuffer());
				// 			KGLOG_COM_PROCESS_ERROR(hr);
				// 			m_lpRiverMgr->m_uHandle = uInfoHandle;
				// 			hr = m_lpRiverMgr->LoadFromInformationBuffer(dwLen,pFile);
				// 			KGLOG_COM_PROCESS_ERROR(hr);
			}
			break;
			*/
		}

	} while (0);

	return S_OK;
}

