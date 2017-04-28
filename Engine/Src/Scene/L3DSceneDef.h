#pragma once
#include <d3dx9.h>

struct InfoFileData
{
	UINT  uHandle;
	DWORD dwFilePos;
};

struct LSceneDataClip
{
	DWORD dwType;
	DWORD dwLength;
	BYTE* pbyBuffer;
	int   nRefrence;

	HRESULT LoadDataFromFile(BYTE* pbyTerrain)
	{
		pbyTerrain = LFileReader::Convert(pbyTerrain, dwType);
		pbyTerrain = LFileReader::Convert(pbyTerrain, dwLength);

		return S_OK;
	}
};