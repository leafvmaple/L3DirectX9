#pragma once
#include <d3dx9.h>
#include <map>

#define CHILD_NODE_COUNT 4
#define MAX_RENDER 6

class L3DTexture;
struct LSceneDataClip;

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

class LTerrainNode
{
	ULONG m_ulRefCount;
	LTerrainNode* m_pParentNode;
	LTerrainNode* m_ChlidrenNode[CHILD_NODE_COUNT];
	int m_nIndexX;
	int m_nIndexZ;
	//EM_TERRAIN_NODE_TYPE m_eLevel;
	//KGMtlData *m_pMtlData;
	unsigned long long m_ullMaterialKey;

	int m_nIndexDiffuseMap;
	BOOL m_bHasChildren;

	//AABBOX m_BBox;
	D3DXVECTOR3 m_vCenter;

	float m_fHeightMin;
	float m_fHeightMax;


	float m_fViewDistance;

	float m_fLODDiffHeight[5];
	int   m_nLodLevel;
	DWORD m_dwNeighbourLevel;
	BOOL m_bLodProcessed;

	bool m_bInRenderList[MAX_RENDER];
};

class LTerrainConverMap
{
public:
	HRESULT LoadConverMapBuffer(BYTE* pbyConverMap, DWORD dwLen);
	LTerrainConverMap();
	~LTerrainConverMap();
	HRESULT UpdateTerrainConverMap();

private:
	CHAR m_szTextureFileName[MAX_PATH];
	CHAR m_szHeightColorFileName[MAX_PATH];

	D3DXVECTOR4 m_vRect;

	L3DTexture* m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pConverVertex;

	float m_fLowest;
	float m_fHeightTotal;
};

class L3DTerrain
{
public:
	L3DTerrain();
	~L3DTerrain();

	HRESULT LoadTerrain(LPCWSTR cszFileName);

	HRESULT UpdateTerrain();

private:
	BOOL m_bEnableTerrainConverMap;
	std::map<UINT, LSceneDataClip*>  m_TerrainInformations;

	LTerrainConverMap* m_pTerrainCoverMap;

	HRESULT LoadTerrainInfo(LPCWSTR cszDirectory);
	HRESULT LoadTerrainClipBuffer(LSceneDataClip* pLSceneDataClip, BYTE* pbyTerrain);
};