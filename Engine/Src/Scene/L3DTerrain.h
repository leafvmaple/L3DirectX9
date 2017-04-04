#pragma once
#include <d3dx9.h>
#include <map>

#define MAP_FILE_VERSION_PACKREGION 2

enum L3D_TERRAIN_BLOCK_TYPE
{
	Terrain_Block_Invalid = 0,

	Terrain_Block_Convermap,
	Terrain_Block_GroundHeight,
	Terrain_Block_Water,
	Terrain_Block_DetailMtl,
	Terrain_Block_Grass,
	Terrain_Block_Stone,
	Terrain_Block_Frutex,
	Terrain_Block_Road,
	Terrain_Block_River,
	Terrain_Block_GroundHeight_Compress,
	Terrain_Block_Water_Compress,
	Terrain_Block_Grass_Compress,
	Terrain_Block_Stone_Compress,
	Terrain_Block_Frutex_Compress,
	Terrain_Block_HeightMaxMin,
	Terrain_Block_Water_Version2,
	Terrain_Block_Total
};

class L3DTerrain
{
public:
	struct LSceneDataClip
	{
		DWORD dwType;
		DWORD dwLength;
		BYTE* pbyBuffer;
		int   nRefrence;
	};

private:
	struct InfoFileData
	{
		UINT uHandle;
		DWORD dwFilePos;
	};

public:
	L3DTerrain();
	~L3DTerrain();

	HRESULT LoadTerrain(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR cszFileName);

private:
	LPDIRECT3DDEVICE9 m_p3DDevice;
	BOOL m_bEnableTerrainConverMap;
	std::map<UINT, LSceneDataClip*>  m_TerrainInformations;

	HRESULT LoadTerrainInfo(LPCWSTR cszDirectory);
	HRESULT LoadClipFromMemory(LSceneDataClip* pLSceneDataClip, BYTE* pbyTerrain);
};