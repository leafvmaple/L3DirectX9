#include <cstdio>
#include <atlconv.h>
#include "L3DTerrain.h"
#include "LAssert.h"
#include "IO/LFileReader.h"
#include "Model/L3DTexture.h"
#include "L3DEngine.h"
#include "L3DSceneDef.h"

extern LPDIRECT3DDEVICE9 g_p3DDevice;

#define FVFWATER (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

HRESULT LTerrainConverMap::LoadConverMapBuffer(BYTE* pbyConverMap, DWORD dwLen)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do
	{
		pbyConverMap = LFileReader::Copy(pbyConverMap, m_szTextureFileName, MAX_PATH);
		pbyConverMap = LFileReader::Copy(pbyConverMap, m_szHeightColorFileName, MAX_PATH);
		pbyConverMap = LFileReader::Convert(pbyConverMap, m_vRect);
		pbyConverMap = LFileReader::Convert(pbyConverMap, m_fLowest);
		pbyConverMap = LFileReader::Convert(pbyConverMap, m_fHeightTotal);

		m_vRect.z = abs(m_vRect.z);
		m_vRect.w = abs(m_vRect.w);

		USES_CONVERSION;

		m_pTexture = new L3DTexture;
		BOOL_ERROR_BREAK(m_pTexture);

		hr = m_pTexture->LoadLTexture(A2CW(m_szTextureFileName));
		HRESULT_ERROR_BREAK(hr);

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

		hr = g_p3DDevice->CreateVertexBuffer(4 * sizeof(VERTEXWATER), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, FVFWATER, D3DPOOL_DEFAULT, &m_pConverVertex, NULL );
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);

	//pFile->Reset();
	return hResult;
}

HRESULT LTerrainConverMap::UpdateTerrainConverMap()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	VERTEXWATER* pVer = NULL;
	LPDIRECT3DDEVICE9 p3DDevice = NULL;

	do 
	{
		hr = m_pTexture->UpdateTexture();
		HRESULT_ERROR_BREAK(hr);

		D3DXVECTOR3 A = D3DXVECTOR3(m_vRect.x, 0, m_vRect.y);
		D3DXVECTOR3 B = A + D3DXVECTOR3(m_vRect.z, 0, 0);
		D3DXVECTOR3 D = A + D3DXVECTOR3(0, 0, -m_vRect.w);
		D3DXVECTOR3 C = B + D - A;
		D3DXVECTOR4 Value = D3DXVECTOR4(1, 1, 1, 1);

		hr = m_pConverVertex->Lock(0, 4 * sizeof(VERTEXWATER), (void**)&pVer, D3DLOCK_DISCARD);
		HRESULT_ERROR_BREAK(hr);

		D3DXVECTOR2 uv0 = D3DXVECTOR2(static_cast<float>(0), static_cast<float>(0));
		D3DXVECTOR2 uv1 = D3DXVECTOR2(static_cast<float>(1), static_cast<float>(0));
		D3DXVECTOR2 uv2 = D3DXVECTOR2(static_cast<float>(0), static_cast<float>(1));
		D3DXVECTOR2 uv3 = D3DXVECTOR2(static_cast<float>(1), static_cast<float>(1));
		D3DCOLOR Diffuse = D3DCOLOR_RGBA((int)(Value.y*255),(int)(Value.z*255),(int)(Value.w*255),255);
		D3DXVECTOR3 Normal(0, 1, 0);

		pVer[0].p = A;
		pVer[0].c = Diffuse;
		pVer[0].n = Normal;
		pVer[0].tu = uv0.x;
		pVer[0].tv = uv0.y;

		pVer[1].p = B;
		pVer[1].c = Diffuse;
		pVer[1].n = Normal;
		pVer[1].tu = uv1.x;
		pVer[1].tv = uv1.y;

		pVer[2].p = D;
		pVer[2].c = Diffuse;
		pVer[2].n = Normal;
		pVer[2].tu = uv2.x;
		pVer[2].tv = uv2.y;

		pVer[3].p = C;
		pVer[3].c = Diffuse;
		pVer[3].n = Normal;
		pVer[3].tu = uv3.x;
		pVer[3].tv = uv3.y;

		/*State.SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
		State.SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
		State.SetSamplerState(0,D3DSAMP_MAGFILTER,(D3DTEXTUREFILTERTYPE)g_cEngineOption.nSampMagFilter);
		State.SetSamplerState(0,D3DSAMP_MINFILTER,(D3DTEXTUREFILTERTYPE)g_cEngineOption.nSampMinFilter);
		State.SetSamplerState(0,D3DSAMP_MIPFILTER,(D3DTEXTUREFILTERTYPE)g_cEngineOption.nSampMipFilter);
		State.SetSamplerState(0,D3DSAMP_MAXANISOTROPY ,g_cEngineOption.dwMaxAnisotropy);*/

		g_p3DDevice->SetStreamSource(0, m_pConverVertex, 0, sizeof(VERTEXWATER));
		g_p3DDevice->SetFVF(FVFWATER);
		g_p3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		hResult = S_OK;
	} while (0);

	return hResult;
}

L3DTerrain::L3DTerrain()
: m_bEnableTerrainConverMap(0)
, m_pTerrainCoverMap(NULL)
{

}

L3DTerrain::~L3DTerrain()
{

}


HRESULT L3DTerrain::LoadTerrain(LPCWSTR cszFileName)
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

	} while (0);

	return hResult;
}


HRESULT L3DTerrain::UpdateTerrain()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		if (m_pTerrainCoverMap)
		{
			hr = m_pTerrainCoverMap->UpdateTerrainConverMap();
			HRESULT_ERROR_BREAK(hr);
		}

		hResult = S_OK;
	} while (0);

	return hResult;
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
	LSceneDataClip* pTerrainClip = NULL;

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
			pTerrainClip = new LSceneDataClip;
			BOOL_ERROR_BREAK(pTerrainClip);

			pbyTerrainInfo = pTerrainClip->LoadDataFromFile(pbyBufferHead + pInfoData[i].dwFilePos);

			hr = LoadTerrainClipBuffer(pTerrainClip, pbyTerrainInfo);
			HRESULT_ERROR_BREAK(hr);

			m_TerrainInformations.insert(std::pair<UINT, LSceneDataClip*>(pInfoData[i].uHandle, pTerrainClip));
		}

		hResult = S_OK;
	} while (0);

	if (!SUCCEEDED(hr) && pTerrainClip)
		SAFE_DELETE(pTerrainClip);

	return hResult;
}

HRESULT L3DTerrain::LoadTerrainClipBuffer(LSceneDataClip* pLSceneDataClip, BYTE* pbyTerrain)
{
	HRESULT hr = E_FAIL;

	do
	{
		//pLSceneDataClip->pbyBuffer = new BYTE[pLSceneDataClip->dwLength];
		//BOOL_ERROR_BREAK(pLSceneDataClip->pbyBuffer);

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
