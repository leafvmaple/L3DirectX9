#pragma once
#include <d3dx9.h>
#include <map>

enum
{
	BLOCK_TYPE_REGION   = 0,
	BLOCK_TYPE_SECTION  = 1,
	BLOCK_TYPE_ZONE     = 2,
	BLOCK_TYPE_AREA     = 3,
};

struct LBlockData;
struct LBlockDataChildData
{
	LBlockData* m_zChildBlock[64];
	DWORD       m_zFilePosition[64];
};

struct LBlockData
{
	int nLevel;
	int nIndexX;
	int nIndexZ;

	int nLastUseTime;
	DWORD dwDataSize;

	BOOL bDataInMemory;

	BOOL bTerrainBlock;
	BOOL bModified;
	BOOL bDisableDynamicUnload;
	BOOL bSaveChildData;
	LBlockData* pParentBlockData;
	LBlockDataChildData* lpChildBlockData;
};

class L3DBlock
{
public:
	L3DBlock();
	~L3DBlock();

	HRESULT LoadBlock(BYTE*& pbySceneBlock);

private:
	struct POINT_OPERATOR
	{
		bool operator()(const POINT& _Left, const POINT& _Right) {
			return (_Left.x != _Right.x) ? _Left.x < _Right.x : _Left.y < _Right.y;
		}
	};
	

	std::map<POINT, LBlockData*, POINT_OPERATOR> m_mapBlockData[4];

	HRESULT CreateBlock(int nLevel, POINT Index);
	LBlockData* GetBlock(int nLevel, POINT Index);
};