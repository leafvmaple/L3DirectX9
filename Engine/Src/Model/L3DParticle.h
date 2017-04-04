#pragma once
#include <d3dx9.h>
#include <string>
#include <vector>

#define MAX_LODLEVEL 5

const DWORD PARSYS_ID = MAKEFOURCC('P', 'A', 'R', '\0');
const DWORD SFX_TEXTURE_FILE_NAME_LENGTH = 128;

enum PARSYS_FILE_ELEMENT_ID
{
	PARSYS_FEID_GENERAL_INFORMATION,
	//PARSYS_FEID_TEXTURE,
	PARSYS_FEID_MATERIAL,
	PARSYS_FEID_LAUNCHER,
	PARSYS_FEID_TRACK,
	PARSYS_FEID_MESH,

	PARSYS_FEID_PARICLE_KIND_EXT,
	PARSYS_FEID_SIZE
};

struct PARSYS_FILE_DATA
{
	LPCWSTR strFileName;
	int    nVersion;
	DWORD  dwOption;
	FLOAT  fDurationTime;
	FLOAT  fLodDistance[MAX_LODLEVEL];
	int    nNumLodLevel;       // 一共多少层lod
	BOOL   bCloseFloorFlag;    // 贴地表
	BOOL   bFixedSize;         // 锁定缩放
	BOOL   bLockYAxis;         // 锁定y轴
	BYTE   byShockWaveFlag;
	DWORD  dwMaxParticles;     // 最大粒子数
	//AABBOX AABBoxLocal;        // 包围盒
	//RENDER_LAUNCHER_ORDER RenderOrder;      // 渲染顺序

	//std::vector<IKG3DMaterial2Instance*> vecMaterial;
	//vector<TRACK_STATIC_DATA*>      vecParticleTrackData;
	//vector<PARTICLE_LAUNCHER_DATA*> vecLauncherData;
};

class L3DParticle
{
private:
#pragma pack(push, 1)
	struct SFX_FILE_HEADER
	{
		DWORD   dwID;               // MAKEFOURCC = ('S', 'F', 'X', '\0');
		DWORD   dwVersion;          // HIWORD(Version) = Major version,
		// LOWORD(Version) = Minor version
		DWORD   dwFileLength;
		DWORD   dwElementNum;
	};

	struct PARSYS_ELEMENT
	{
		DWORD   dwElementID;
		DWORD   dwElementOffset;
		DWORD   dwElementLength;
	};

	struct PARSYS_GENERAL_INFORMATION_BLOCK
	{
		FLOAT fLodDistance[MAX_LODLEVEL];
		int   nNumLodLevel; 
		BYTE  byShockWaveFlag;
		BYTE  byCloseFloorFlag;
		BYTE  byFixedSize;
		BYTE  byLockYAlex;
		BYTE  byRenderOrder;
		DWORD dwMaxParticle;
		D3DXVECTOR3 vAABBoxMax;
		D3DXVECTOR3 vAABBoxMin;
		DWORD dwReserved[20];
	};

	struct PARSYS_MESH_BLOCK
	{
		BYTE  byMeshName[SFX_TEXTURE_FILE_NAME_LENGTH];
		DWORD dwReserved[5];
	};

	struct PARSYS_LAUNCHER_BLOCK
	{
		char  szName[MAX_PATH];    
		DWORD dwID;
		BYTE  byLauncherType;
		BYTE  byMotionType; 
		BYTE  bySpawnMode; 	
		BYTE  byHorizontal;
		BYTE  byVertical;
		BYTE  byFaceType;
		BYTE  byParticleType;
		BYTE  bySortMode;
		FLOAT fLod[MAX_LODLEVEL];
		int   nMaterialIndex;
		DWORD dwNumModules;
		BOOL  bPreCompute;
		int   dwNumPlay;
		FLOAT fDurationTime;
		FLOAT fDelayTime;
		BYTE  byUVFlowType;
		BYTE  byReserved[3];
		DWORD dwVersion;
		DWORD dwReserved[18];
	};
#pragma pack(pop)

public:
	L3DParticle();
	~L3DParticle();

	typedef HRESULT (L3DParticle::*ParticleLoadFunc)(BYTE*, DWORD, PARSYS_FILE_DATA*);
	//static ParticleLoadFunc ms_ParSysLoadFunction[PARSYS_FEID_SIZE];

	HRESULT LoadParticle(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR cszFileName);

public:
	//std::vector<IKG3DMaterial2Instance*> m_Material;

	HRESULT ReadGeneralInformationBlock(BYTE *pbyBuffer, DWORD dwLength, PARSYS_FILE_DATA *pFileData);
	HRESULT ReadParticleMaterialBlock(BYTE *pbyBuffer, DWORD dwLength, PARSYS_FILE_DATA *pFileData);
	HRESULT ReadParticleLauncherBlock(BYTE *pbyBuffer, DWORD dwLength, PARSYS_FILE_DATA *pFileData);
	HRESULT ReadParticleTrackBlock(BYTE *pbyBuffer, DWORD dwLength, PARSYS_FILE_DATA *pFileData);
};

static L3DParticle::ParticleLoadFunc ms_ParSysLoadFunction[PARSYS_FEID_SIZE] = 
{
	&L3DParticle::ReadGeneralInformationBlock,
	&L3DParticle::ReadParticleMaterialBlock,
	&L3DParticle::ReadParticleLauncherBlock,
	&L3DParticle::ReadParticleTrackBlock,
	NULL
};