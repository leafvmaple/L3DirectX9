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

	BYTE* LoadDataFromFile(BYTE* pbyTerrain)
	{
		pbyTerrain = LFileReader::Convert(pbyTerrain, dwType);
		pbyTerrain = LFileReader::Convert(pbyTerrain, dwLength);

		return pbyTerrain;
	}
};

struct VERTEXWATER
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DCOLOR    c;
	float tu,tv;
};
