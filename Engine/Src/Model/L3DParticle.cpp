#include "L3DParticle.h"
#include "LAssert.h"
#include "IO/LFileReader.h"

L3DParticle::L3DParticle()
{

}

L3DParticle::~L3DParticle()
{

}

HRESULT L3DParticle::LoadParticle(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR cszFileName)
{
	BYTE *pbyParticle = NULL;
	SFX_FILE_HEADER* pHeader = NULL;
	PARSYS_ELEMENT* pParSysArr = NULL;
	PARSYS_FILE_DATA* pFileData = NULL;
	size_t uBufferLen = 0;
	size_t uFileLen = 0;

	do
	{
		LFileReader::Reader(cszFileName, &pbyParticle, &uBufferLen);
		BOOL_ERROR_BREAK(pbyParticle);

		pbyParticle = LFileReader::Convert(pbyParticle, pHeader);
		BOOL_ERROR_BREAK(pHeader->dwID == PARSYS_ID);

		uFileLen = pHeader->dwFileLength - sizeof(SFX_FILE_HEADER);

		pFileData = new PARSYS_FILE_DATA;
		BOOL_ERROR_BREAK(pFileData);

		ZeroMemory(pFileData, sizeof(PARSYS_FILE_DATA));
		pFileData->strFileName = cszFileName;
		pFileData->nVersion = (int)LOWORD(pHeader->dwVersion);
		//pFileData->dwOption = dwOption;

		pbyParticle = LFileReader::Convert(pbyParticle, pParSysArr);

		for (DWORD i = 0; i < pHeader->dwElementNum; i++)
		{
			(this->*ms_ParSysLoadFunction[pParSysArr[i].dwElementID])(pbyParticle + pParSysArr[i].dwElementOffset - uFileLen, pParSysArr[i].dwElementLength, pFileData);
		}

	} while (0);

	return S_OK;
}

HRESULT L3DParticle::ReadGeneralInformationBlock(BYTE *pbyBuffer, DWORD dwLength, PARSYS_FILE_DATA *pFileData)
{
	HRESULT hResult = E_FAIL;

	do 
	{
		PARSYS_GENERAL_INFORMATION_BLOCK *pBlock = (PARSYS_GENERAL_INFORMATION_BLOCK*)&pbyBuffer;

		pFileData->nNumLodLevel    = pBlock->nNumLodLevel;
		pFileData->bCloseFloorFlag = (BOOL)pBlock->byCloseFloorFlag;
		pFileData->bFixedSize      = (BOOL)pBlock->byFixedSize;
		pFileData->bLockYAxis      = (BOOL)pBlock->byLockYAlex;
		//pFileData->RenderOrder     = (RENDER_LAUNCHER_ORDER)pBlock->byRenderOrder;
		pFileData->dwMaxParticles  = pBlock->dwMaxParticle;
		pFileData->byShockWaveFlag  = pBlock->byShockWaveFlag;

		for (int i = 0; i < MAX_LODLEVEL; i++)
		{
			pFileData->fLodDistance[i] = pBlock->fLodDistance[i];
		}
		//pFileData->AABBoxLocal.AddPosition(pBlock->vAABBoxMax);
		//pFileData->AABBoxLocal.AddPosition(pBlock->vAABBoxMin);

	} while (0);

	return S_OK;
}

HRESULT L3DParticle::ReadParticleMaterialBlock(BYTE *pbyBuffer, DWORD dwLength, PARSYS_FILE_DATA *pFileData)
{
	/*
	HRESULT hrResult = E_FAIL;
	HRESULT hRetCode = E_FAIL;
	IKG3DMaterial2Instance* pMtlIns = NULL;
	DWORD dwTextureOption = 0;

	do 
	{
		//GetTextureScaleOption(pFileData->dwOption, dwTextureOption);

		g_cEngineManager.m_lpMaterialSystem->RequestMaterialInstance(&pMtlIns);
		KG_PROCESS_ERROR(pMtlIns);

		hRetCode = pMtlIns->LoadFromBuffer(&pbyBuffer, dwLength, dwTextureOption);
		KG_COM_PROCESS_ERROR(hRetCode);

		pFileData->vecMaterial.push_back(pMtlIns);

	} while (0);
	*/

	return S_OK;
}

HRESULT L3DParticle::ReadParticleLauncherBlock(BYTE *pbyBuffer, DWORD dwLength, PARSYS_FILE_DATA *pFileData)
{
	/*HRESULT hResult = E_FAIL;
	DWORD i;
	PARTICLE_LAUNCHER_DATA* pLauncherData = NULL;
	PARSYS_LAUNCHER_BLOCK* pBlock = NULL;
	DWORD dwPos = 0;

	pBlock = (PARSYS_LAUNCHER_BLOCK *)&pbyBuffer;
	dwPos = sizeof(PARSYS_LAUNCHER_BLOCK);

	ASSERT(pFileData);

	pLauncherData = new PARTICLE_LAUNCHER_DATA;
	KGLOG_PROCESS_ERROR(pLauncherData);

	strcpy_s(pLauncherData->m_szName, sizeof(pLauncherData->m_szName), pBlock->szName);
	pLauncherData->m_byLauncherType = pBlock->byLauncherType;
	pLauncherData->m_dwID           = pBlock->dwID;
	pLauncherData->m_byMotionType   = pBlock->byMotionType;
	pLauncherData->m_bySpawnMode    = pBlock->bySpawnMode;
	pLauncherData->m_byHorizontal   = pBlock->byHorizontal;
	pLauncherData->m_byVertical     = pBlock->byVertical;
	pLauncherData->m_ParticleFaceType = (PARSYS_PARTICLE_FACETYPE)pBlock->byFaceType;
	pLauncherData->m_byParticleType = pBlock->byParticleType;
	pLauncherData->m_bPreCompute   = pBlock->bPreCompute;
	pLauncherData->m_dwNumPlay     = pBlock->dwNumPlay;
	pLauncherData->m_fDurationTime = pBlock->fDurationTime;
	pLauncherData->m_fDelayTime    = pBlock->fDelayTime;
	pLauncherData->m_SortMode      = (ParticleSortMode)pBlock->bySortMode;
	pLauncherData->m_nMaterialIndex = pBlock->nMaterialIndex;
	pLauncherData->m_byUVFlowType  = pBlock->byUVFlowType;
	pLauncherData->m_dwVersion     = pBlock->dwVersion;

	if(pLauncherData->m_dwVersion < PARSYS_LAUCHER_VERSION_1)
	{
		if(pLauncherData->m_byLauncherType == MESH_LAUNCHER ||
			pLauncherData->m_byLauncherType == MESHQUOTE_LAUNCHER)
		{
			pLauncherData->m_byUVFlowType = PARTICLE_UV_FLOW_RESET;
		}
		else
		{
			pLauncherData->m_byUVFlowType = PARTICLE_UV_FLOW_CONTINUE;
		}
	}

	for (int i = 0; i < MAX_LODLEVEL; i++)
	{
		pLauncherData->m_fLod[i] = pBlock->fLod[i];
	}

	for (i = 0; i < pBlock->dwNumModules; ++i)
	{
		DWORD dwType = *(DWORD*)(&pbyBuffer[dwPos]);
		dwPos += sizeof(DWORD);
		DWORD dwReadLength = 0;
		KG3DParticleModule* pModules = g_CreateParticleModule(dwType);
		BYTE* pByteData = (BYTE*)&pbyBuffer[dwPos];
		dwReadLength = pModules->ReadData(pByteData);
		dwPos += dwReadLength;

		pLauncherData->m_Modules.push_back(pModules);
	}

	if (pLauncherData->m_bPreCompute)
	{
		BYTE* pbyData = (BYTE*)&pbyBuffer[dwPos];
		pLauncherData->m_pPreComputeInfo = new KG3DPreComputeInfo;
		KGLOG_PROCESS_ERROR(pLauncherData->m_pPreComputeInfo);

		pLauncherData->m_pPreComputeInfo->Init();
		pLauncherData->m_pPreComputeInfo->Clear();

		dwPos += pLauncherData->m_pPreComputeInfo->ReadData(pbyData);
	}

	if (dwPos < dwLength)
	{
		pLauncherData->m_dwExLength = dwLength - dwPos;
		pLauncherData->m_pByte = new BYTE[pLauncherData->m_dwExLength];
		KG_PROCESS_ERROR(pLauncherData->m_pByte);

		memcpy(pLauncherData->m_pByte, &(pbyBuffer[dwPos]), pLauncherData->m_dwExLength);
	} 

	pFileData->vecLauncherData.push_back(pLauncherData);
	pLauncherData = NULL;*/

	return S_OK;
}

HRESULT L3DParticle::ReadParticleTrackBlock(BYTE *pbyBuffer, DWORD dwLength, PARSYS_FILE_DATA *pFileData)
{
	/*HRESULT hResult = E_FAIL;
	HRESULT hrRetCode = E_FAIL;
	SFX_TRACK_BLOCK*   pBlock = (SFX_TRACK_BLOCK*)&pbyBuffer;
	KG3DSFXTrackData*  pTrackData = NULL;
	TRACK_STATIC_DATA* pTrackStaticData = NULL;

	KG_PROCESS_ERROR(pFileData);

	pTrackStaticData = new TRACK_STATIC_DATA;
	KGLOG_PROCESS_ERROR(pTrackStaticData);

	hrRetCode = g_cTrackTable.LoadResourceFromFile(
		(const char*)pBlock->byTrackFileName, 
		0, 
		0, 
		(IKG3DResourceBase **)&pTrackData
		);
	if (FAILED(hrRetCode))
	{
		KGLogPrintf(KGLOG_ERR, "[SFX] Load track file failed : %s", (const char*)pBlock->byTrackFileName);
		goto Exit0;
	}

	pTrackStaticData->vTranslation = pBlock->vTranslation;
	pTrackStaticData->vScalingCenter = pBlock->vScalingCenter;
	pTrackStaticData->qScalingRotation = pBlock->qScalingRotation;
	pTrackStaticData->vScaling = pBlock->vScaling;
	pTrackStaticData->vRotationCenter = pBlock->vRotationCenter;
	pTrackStaticData->qRotation = pBlock->qRotation;
	pTrackStaticData->dwInterval = pBlock->dwInterval;

	pTrackStaticData->pTrackData = pTrackData;
	pTrackData = NULL;

	pFileData->vecParticleTrackData.push_back(pTrackStaticData);
	pTrackStaticData = NULL;*/

	return S_OK;
}
