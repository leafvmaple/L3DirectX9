#include <cstdio>
#include <atlconv.h>
#include "L3DEntity.h"
#include "LAssert.h"
#include "IO/LFileReader.h"
#include "L3DSceneDef.h"

L3DEntity::L3DEntity()
: m_p3DDevice(NULL)
{

}

L3DEntity::~L3DEntity()
{

}

HRESULT L3DEntity::LoadEntity(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR pcszFileName)
{
	HRESULT hr = E_FAIL;
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

		_wsplitpath_s(pcszFileName, szDrive, MAX_PATH, szDir, MAX_PATH, szOnlyFileName, MAX_PATH, szExt, MAX_PATH);
		swprintf_s(wszFileDir, TEXT("%s%s%sFile/"), szDrive, szDir, szOnlyFileName);

		hr = LoadEntityInfo(wszFileDir);
		HRESULT_ERROR_BREAK(hr);

		m_p3DDevice = p3DDevice;

	} while (0);

	return S_OK;
}

HRESULT L3DEntity::LoadEntityInfo(LPCWSTR cszDirectory)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	TCHAR wszEntityInfoName[MAX_PATH];
	BYTE* pbyEntityInfo = NULL;
	BYTE* pbyBufferHead = NULL;
	size_t uTerrainLen = 0;

	DWORD dwVersion = 0;
	DWORD dwNum = 0;
	InfoFileData* pInfoData = NULL;
	LSceneDataClip* pEntityClip = NULL;

	do
	{
		swprintf_s(wszEntityInfoName, TEXT("%s\\map.EntityInfo"),cszDirectory);

		LFileReader::Reader(wszEntityInfoName, &pbyEntityInfo, &uTerrainLen);
		BOOL_ERROR_BREAK(pbyEntityInfo);
		pbyBufferHead = pbyEntityInfo;

		pbyEntityInfo = LFileReader::Convert(pbyEntityInfo, dwVersion);
		assert(dwVersion == MAP_FILE_VERSION_PACKREGION);

		pbyEntityInfo = LFileReader::Convert(pbyEntityInfo, dwNum);
		BOOL_SUCCESS_BREAK(!dwNum);

		pbyEntityInfo = LFileReader::Convert(pbyEntityInfo, pInfoData, dwNum);
		for (DWORD i = 0; i < dwNum; i++)
		{
			pEntityClip = new LSceneDataClip;
			BOOL_ERROR_BREAK(pEntityClip);

			hr = pEntityClip->LoadDataFromFile(pbyBufferHead + pInfoData[i].dwFilePos);
			HRESULT_ERROR_BREAK(hr);

			m_EntityInformations.insert(std::pair<UINT, LSceneDataClip*>(pInfoData[i].uHandle, pEntityClip));
		}

		hResult = S_OK;
	} while (0);

	if (!SUCCEEDED(hr) && pEntityClip)
		SAFE_DELETE(pEntityClip);

	return S_OK;
}

