#pragma once
#include "L3DStruct.h"
#include <d3dx9.h>
#include <string>
#include <vector>

#define s_dwMaxSocketName		32
#define s_dwMaxSocket			32
#define s_dwMaxBoneName			32
#define s_dwMaxChildBone		32
#define s_dwInvalidateIndex		0xffffffff

class  KG3DBsp;

typedef struct _KG3DOBJProperty
{
	BOOL    bAutoProduceObstacle;
	BOOL    bObscatleCamera;
	BOOL    bSelectable;
	BOOL	bHeightTest;
	BOOL	bOccluder;

	BOOL    bAffectByTime;
	//
	BOOL bInitallized;
	_KG3DOBJProperty()
		: bAutoProduceObstacle(TRUE)
		, bObscatleCamera(TRUE)
		, bSelectable(TRUE)
		, bHeightTest(TRUE)
		, bOccluder(TRUE)
		, bAffectByTime(0)
		, bInitallized(0)
	{
	}
}KG3DOBJProperty;

#pragma pack(push, 1)
struct OptMeshHeader
{
	const static DWORD s_dwMask = 0x424d4553;
	const static DWORD s_dwVersion = 0;
	DWORD dwMask;
	DWORD dwVersion;
	DWORD dwType;
	DWORD dwSize;
	DWORD dwNumVertex;
	DWORD dwStride;
	DWORD dwNumFace;
	DWORD dwNumSubset;
	DWORD dwAttributeTable;
	D3DXVECTOR3 BoxA;
	D3DXVECTOR3 BoxB;
	DWORD dwNumBone;
	DWORD dwNumSocket;
	DWORD dwNumAttGroup;
	DWORD dwBoneCombineTblSize;
	OptMeshHeader()
	{
		memset(this, 0, sizeof(OptMeshHeader));
		dwMask = s_dwMask;
		dwVersion = s_dwVersion;
	}
};

struct Socket1
{
	TCHAR strSocketName[s_dwMaxSocketName];
	DWORD dwParentBoneIndex;
	D3DXMATRIX matMatrixOffset;
	D3DXMATRIX matCurMatrix;
	Socket1()
	{
		dwParentBoneIndex = s_dwInvalidateIndex;
	}
};

struct BoneInfo1
{
	TCHAR strBoneName[s_dwMaxBoneName];
	DWORD dwNumChild;
	DWORD dwChildIndex[s_dwMaxChildBone];
	DWORD dwParentIndex;

	D3DXMATRIX  CurMatrix;
	D3DXMATRIX  LocalWorldCurMatrix_NotNeeded; //骨骼在本身世界中的位置矩阵
	D3DXMATRIX  ParentReletiveMatrix;

	BOOL        IsFlexibleBone;
	int         nFlexBoneLevel_NotNeeded;

	//BBox        bBox;

	D3DXMATRIX  BBoxScaleMatrix;

	DWORD       NumVerticesRef;
	DWORD*      VerticesIndexRef;
	float*      VerticesWeightRef;
	BOOL bNeedDeleteVerticesRefs;
	BoneInfo1();
	~BoneInfo1();
};

struct MeshDataBlocks
{
	DWORD  PositionBlock;
	DWORD  NormalBlock;
	DWORD  DiffuseBlock;
	DWORD  TextureUVW1Block;
	DWORD  TextureUVW2Block;
	DWORD  TextureUVW3Block;
	DWORD  FacesIndexBlock;
	DWORD  SubsetIndexBlock;
	DWORD  SkinInfoBlock;
	DWORD  LODInfoBlock;
	DWORD  FlexibleBodyBlock; // not need the data now
	DWORD  BBoxBlock;
	DWORD  BlendMeshBlock;
	DWORD  ExtendBlock[17];
};

struct _MeshHead
{
	DWORD  dwNumVertices;
	DWORD  dwNumFaces;
	DWORD  dwNumSubset;
	union
	{
		MeshDataBlocks Blocks;
		DWORD dwBlocks[30];
	};
};

struct _MeshFileHead
{
	TFileHeader KSFileHeader_NotUse;
	DWORD       dwFileMask;
	DWORD       dwBlockLength;
	DWORD       dwVersion;
	DWORD       AnimationBlock_NotUse;
	DWORD       ExtendBlock_NotUse[10];
	DWORD       MeshCount_NotUse;
	const static DWORD s_dwFileMask = 0x4d455348;//"mesh"
	const static DWORD s_dwCurrentVersion = 0;
};
#pragma pack(pop)

struct LMESH_DATA
{
	DWORD           dwID;
	std::string     strFileName;
	DWORD           dwOption;
	DWORD           dwOptionEx;

	DWORD           dwNumVertices;
	DWORD           dwNumFaces;
	DWORD           dwNumSubset;
	DWORD           dwMeshFVF;

	D3DXVECTOR3     *pPos;
	D3DXVECTOR3     *pNormals;
	D3DCOLOR        *pDiffuse;
	D3DXVECTOR3     *pUV1;
	D3DXVECTOR3     *pUV2;
	D3DXVECTOR3     *pUV3;

	DWORD           *pFaceIndices;
	DWORD           *pSubsetIndices;

	DWORD                dwNumBone;
	D3DXMATRIX*          pReverseMatrix;
	BoneInfo1*           pBoneInfo;
	std::vector<int>     vecBaseBoneID;
	DWORD                dwBoneVerteicesBufferSize;
	DWORD*               pdwBoneVerticesIndexBuffer;
	float*               pfBoneVerticesWeightBuffer;

	DWORD                dwNumSocket;
	Socket1*            pSockets;

	KG3DBsp*            pBsp;
	KG3DOBJProperty     Propertys;
	//BBox                BoundingBox;
	D3DXVECTOR2*        pUVSizeMax;

	BOOL                bBin;
};