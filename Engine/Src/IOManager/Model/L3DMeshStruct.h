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

#define  ELEMENT_FVF_END  0xffffffff

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
	BYTE*               pbyFileBuffer;
	BOOL                bBin;
};

struct VertexFromatOffsetItem
{
	const static DWORD s_dwMaxVertexElement = 8;
	DWORD dwFVF;
	DWORD dwNumElement;
	DWORD dwSrcOffset[s_dwMaxVertexElement];
	DWORD dwSrcStride[s_dwMaxVertexElement];
	DWORD dwDestOffset[s_dwMaxVertexElement];
	DWORD dwDestStride[s_dwMaxVertexElement];
	DWORD dwElementFVF[s_dwMaxVertexElement];
};

static VertexFromatOffsetItem s_VertexFormat[] = {
	{ D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, 3,   
	0,  1,  3, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 
	12, 12, 12, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0, 12, 24, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	12, 12,  8, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 
	D3DFVF_XYZ, D3DFVF_NORMAL, D3DFVF_TEX1, ELEMENT_FVF_END},//PosNorTex1

	{ D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2, 4,
	0,  1,  3, 4, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 
	12, 12, 12, 12, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 
	0, 12, 24, 32, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 
	12, 12,  8,  8, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	D3DFVF_XYZ, D3DFVF_NORMAL, D3DFVF_TEX1, D3DFVF_TEX2, ELEMENT_FVF_END,},//PosNorTex2

	{ D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3, 5,
	0,  1,  3, 4, 5, 0xffffffff, 0xffffffff, 0xffffffff, 
	12, 12, 12, 12, 12, 0xffffffff, 0xffffffff, 0xffffffff, 
	0, 12, 24, 32, 40, 0xffffffff, 0xffffffff, 0xffffffff, 
	12, 12,  8,  8,  8, 0xffffffff, 0xffffffff, 0xffffffff, 
	D3DFVF_XYZ, D3DFVF_NORMAL, D3DFVF_TEX1, D3DFVF_TEX2, D3DFVF_TEX3, ELEMENT_FVF_END},//PosNorTex3 有问题

	{ D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, 3,
	0,  2,  3, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 
	12,  4, 12, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0, 12, 16, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	12,  4,  8, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	D3DFVF_XYZ, D3DFVF_DIFFUSE, D3DFVF_TEX1, ELEMENT_FVF_END, },//PosDiffuseTex1

	{ D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2, 4,
	0,  2,  3,  4, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 
	12,  4, 12, 12, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0, 12, 16, 24, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	12,  4,  8,  8, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 
	D3DFVF_XYZ, D3DFVF_DIFFUSE, D3DFVF_TEX1, D3DFVF_TEX2, ELEMENT_FVF_END},//PosDiffuseTex2

	{ D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX3, 5, 
	0,  2,  3,  4,  5, 0xffffffff, 0xffffffff, 0xffffffff, 
	12,  4, 12, 12, 12, 0xffffffff, 0xffffffff, 0xffffffff, 
	0, 12, 16, 24, 32, 0xffffffff, 0xffffffff, 0xffffffff,
	12,  4,  8,  8,  8, 0xffffffff, 0xffffffff, 0xffffffff, 
	D3DFVF_XYZ, D3DFVF_DIFFUSE, D3DFVF_TEX1, D3DFVF_TEX2, D3DFVF_TEX3, ELEMENT_FVF_END},//PosDiffuseTex2

	{ D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1, 4,
	0,  1,  2,  3, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 
	0, 12,  4, 12, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 
	0, 12, 16, 20, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 
	0, 12,  4,  8, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 
	D3DFVF_XYZ, D3DFVF_DIFFUSE, D3DFVF_NORMAL, D3DFVF_TEX1, ELEMENT_FVF_END,},//PosNormalDiffuseTex1 ?????bug

	{ D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX2, 5,
	0,  1,  2,  3,  4, 0xffffffff, 0xffffffff, 0xffffffff, 
	0, 12,  4, 12, 12, 0xffffffff, 0xffffffff, 0xffffffff,
	0, 12, 16, 24, 32, 0xffffffff, 0xffffffff, 0xffffffff,
	0, 12,  4,  8,  8, 0xffffffff, 0xffffffff, 0xffffffff, 
	D3DFVF_XYZ, D3DFVF_DIFFUSE, D3DFVF_NORMAL, D3DFVF_TEX1, D3DFVF_TEX2, ELEMENT_FVF_END},//PosNormalDiffuseTex2

	{ D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX3, 6,
	0,  1,  2,  3,  4,  5, 0xffffffff, 0xffffffff, 
	0, 12,  4,  8,  8,  8, 0xffffffff, 0xffffffff, 
	0, 12, 16, 28, 40, 52, 0xffffffff, 0xffffffff, 
	0, 12,  4, 12, 12, 12, 0xffffffff, 0xffffffff, 
	D3DFVF_XYZ, D3DFVF_DIFFUSE, D3DFVF_NORMAL, D3DFVF_TEX1, D3DFVF_TEX2, D3DFVF_TEX3, ELEMENT_FVF_END}//PosNormalDiffuseTex3
};