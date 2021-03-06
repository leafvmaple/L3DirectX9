#pragma once
#include "L3DInterface.h"

class L3DEntity;
class L3DTerrain;
class L3DBlock;

class L3DScene : public ILScene
{
private:
	struct MapFileHeader
	{
		DWORD dwMask;
		DWORD dwVersion;
		UINT  uCurrentEntityID;
		UINT  uCurrentEntityInformationID;
		UINT  uCurrentTerrainInformationID;
		DWORD dwNumEntity;
		DWORD dwNumEntityInformation;
		DWORD dwNumTerrainInformation;
		DWORD dwNumSceneBlock[4];
		DWORD dwEntityTransfoVersion;
		DWORD dwMapOption;
		DWORD dwSceneSize;
		DWORD dwExtend[125];			 
	};

public:
	L3DScene();
	virtual ~L3DScene();

	virtual HRESULT Init(LPCWSTR pcszFileName);
	HRESULT UpdateDisplay();

private:
	L3DTerrain* m_pLTerrain;
	L3DEntity* m_pLEntity;
	L3DBlock* m_pLBlock;
};