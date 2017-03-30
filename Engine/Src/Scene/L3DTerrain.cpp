#include "L3DTerrain.h"
#include <cstdio>


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

	do
	{
		ZeroMemory(szFileName, sizeof(szFileName));

		_wsplitpath_s(cszFileName, szDrive, MAX_PATH, szDir, MAX_PATH, szOnlyFileName, MAX_PATH, szExt, MAX_PATH);
		swprintf_s(szMapIniFileName, TEXT("%s%s%s_Setting.ini"), szDrive, szDir, szOnlyFileName);

		m_bEnableTerrainConverMap = GetPrivateProfileInt(TEXT("ConverMap"), TEXT("bTerrainAlphaConverEnable"), 0, szMapIniFileName);
		GetPrivateProfileString(TEXT("ConverMap"), TEXT("TerrainAlphaConverMapFile"), TEXT("AA"), szFileName, MAX_PATH, szMapIniFileName);

		m_p3DDevice = p3DDevice;

	} while (0);

	return S_OK;
}
