#include <cstdio>
#include "L3DTerrain.h"
#include "LAssert.h"
#include "IO/LFileReader.h"

L3DTerrain::L3DTerrain()
: m_p3DDevice(NULL)
, m_bEnableTerrainConverMap(0)
{

}

L3DTerrain::~L3DTerrain()
{

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
	LSceneDataClip* pSceneDataClip = NULL;

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
			pSceneDataClip = new LSceneDataClip;
			BOOL_ERROR_BREAK(pSceneDataClip);

			hr = LoadClipFromMemory(pSceneDataClip, pbyBufferHead + pInfoData[i].dwFilePos);
			HRESULT_ERROR_BREAK(hr);

			m_TerrainInformations.insert(std::pair<UINT, LSceneDataClip*>(pInfoData[i].uHandle, pSceneDataClip));
		}

		hResult = S_OK;
	} while (0);

	if (!SUCCEEDED(hr) && pSceneDataClip)
		SAFE_DELETE(pSceneDataClip);

	return S_OK;
}

HRESULT L3DTerrain::LoadClipFromMemory(LSceneDataClip* pLSceneDataClip, BYTE* pbyTerrain)
{
	do 
	{
		pbyTerrain = LFileReader::Convert(pbyTerrain, pLSceneDataClip->dwType);
		pbyTerrain = LFileReader::Convert(pbyTerrain, pLSceneDataClip->dwLength);

		pLSceneDataClip->pbyBuffer = new BYTE[pLSceneDataClip->dwLength];
		BOOL_ERROR_BREAK(pLSceneDataClip->pbyBuffer);

		LFileReader::Convert(pbyTerrain, pLSceneDataClip->pbyBuffer, pLSceneDataClip->dwLength);

		pLSceneDataClip->nRefrence = 0;

	} while (0);

	return S_OK;
}
