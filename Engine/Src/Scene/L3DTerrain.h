#pragma once
#include <d3dx9.h>
#include <map>

#define MAP_FILE_VERSION_PACKREGION 2

class L3DTexture;

enum L3D_TERRAIN_INFO_TYPE
{
	Terrain_Info_Invalid = 0,

	Terrain_Info_Convermap,
	Terrain_Info_DetailMtlMgr,
	Terrain_Info_GrassSet,
	Terrain_Info_StoneSet,
	Terrain_Info_FrutexSet,
	Terrain_Info_RoadMgr,
	Terrain_Info_RiverMgr,
	Terrain_Info_Total
};

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

class LTerrainConverMap
{
public:
	HRESULT LoadConverMapBuffer(BYTE* pbyConverMap, DWORD dwLen);

private:
	TCHAR m_szTextureFileName[MAX_PATH];
	TCHAR m_szHeightColorFileName[MAX_PATH];

	D3DXVECTOR4 m_vRect;

	float m_fLowest;
	float m_fHeightTotal;

	L3DTexture* m_pTexture;
};

class L3DTerrain
{
public:
	struct LTerrainClip
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
	std::map<UINT, LTerrainClip*>  m_TerrainInformations;

	LTerrainConverMap* m_pTerrainCoverMap;

	HRESULT LoadTerrainInfo(LPCWSTR cszDirectory);
	HRESULT LoadClipBuffer(LTerrainClip* pLSceneDataClip, BYTE* pbyTerrain);

};