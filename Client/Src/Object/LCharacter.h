#pragma once
#include "LModel.h"

class LCharacter : public LModel
{
public:
	LCharacter();
	LCharacter(WCHAR* pwszMeshPath);

	virtual ~LCharacter();
	virtual HRESULT Display(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice, float fDeltaTime);

public:
	HRESULT Create(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice);
	HRESULT SetScale(float fScale);
	HRESULT SetTranslation(const D3DXVECTOR3& vTranslation);

private:
	WCHAR m_wszMeshPath[LENGIEN_FILENAME_MAX];

	float m_fAngleX;
	float m_fAngleY;
};