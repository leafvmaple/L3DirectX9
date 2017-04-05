#include "L3DEntity.h"
#include "LAssert.h"
#include "IO/LFileReader.h"
#include <cstdio>

L3DEntity::L3DEntity()
{

}

L3DEntity::~L3DEntity()
{

}

HRESULT L3DEntity::LoadEntity(LPCWSTR pcszFileName)
{
	TCHAR szDrive[MAX_PATH];
	TCHAR szExt[MAX_PATH];
	TCHAR szDir[MAX_PATH];
	TCHAR szOnlyFileName[MAX_PATH];
	TCHAR szMapIniFileName[MAX_PATH];
	TCHAR szFileName[MAX_PATH];
	TCHAR wszFileDir[MAX_PATH];

	do 
	{
		_wsplitpath_s(pcszFileName, szDrive, MAX_PATH, szDir, MAX_PATH, szOnlyFileName, MAX_PATH, szExt, MAX_PATH);
		swprintf_s(wszFileDir, TEXT("%s%s%sFile/"), szDrive, szDir, szOnlyFileName);

	} while (0);

	return S_OK;
}