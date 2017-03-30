#include "L3DMaterial.h"
#include "IO/LFileReader.h"
#include "LAssert.h"

L3DMaterial::L3DMaterial()
: m_dwNumMaterials(0)
, m_pMaterialSubset(NULL)
, m_p3DDevice(NULL)
{

}

L3DMaterial::~L3DMaterial()
{

}

HRESULT L3DMaterial::LoadLMaterial(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR cszFileName)
{
	HRESULT hResult = E_FAIL;
	BYTE* pbyMaterial = new BYTE[1000000];
	BYTE* pbyBufferHead = NULL;
	size_t uMaterialLen = 0;
	DWORD dwMask = 0;
	DWORD dwBlockLength = 0;

	do 
	{
		LFileReader::Reader(cszFileName, pbyMaterial, 1000000, &uMaterialLen);
		pbyBufferHead = pbyMaterial;

		pbyMaterial = LFileReader::Convert(pbyMaterial, dwMask);
		BOOL_ERROR_BREAK(dwMask >= 0x4D41544C);//MATL对应的ASECC代码

		pbyMaterial = LFileReader::Convert(pbyMaterial, dwBlockLength);
		pbyMaterial = LFileReader::Convert(pbyMaterial, m_dwNumMaterials);
		BOOL_ERROR_BREAK(m_dwNumMaterials <= MAX_SUBSET_COUNT); // just for check file error, normally this value < 10

		m_pMaterialSubset = new MaterialSubset[m_dwNumMaterials];
		BOOL_ERROR_BREAK(m_pMaterialSubset);

		for (DWORD i = 0; i < m_dwNumMaterials; i++)
		{
			LoadMaterialBuffer(pbyMaterial, &m_pMaterialSubset[i]);

			//m_bHasDetail = nHasDetail || m_bHasDetail;
			//m_bSortAsSFX = nIsSortAsSFX || m_bSortAsSFX;
		}
		// maybe still some data in file, old version use it, now not need

		//////////////////////////////////////////////////////////////////////////
		/*if(m_bHasDetail)
		{
			char szName[MAX_PATH];
			wsprintf(szName,"%s.ini",cszFileName);

			LoadMaterialDetails(szName);
		}*/

		m_p3DDevice = p3DDevice;

		hResult = S_OK;
	} while (0);

	return S_OK;
}


HRESULT L3DMaterial::UpdateMaterial(DWORD dwSubMaterial)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		hr = m_p3DDevice->SetMaterial(&m_pMaterialSubset[dwSubMaterial].Material9);
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT L3DMaterial::LoadMaterialBuffer(BYTE*& pbyMaterial, MaterialSubset* pLMaterialData)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	DWORD dwMaterialOptionCount = 0;
	L3DTexture::_MtlOption* pNewOption = NULL;

	do 
	{
		BOOL_ERROR_BREAK(pLMaterialData);

		pbyMaterial = LFileReader::Convert(pbyMaterial, pLMaterialData->Material9.Ambient);
		pbyMaterial = LFileReader::Convert(pbyMaterial, pLMaterialData->Material9.Diffuse);
		pbyMaterial = LFileReader::Convert(pbyMaterial, pLMaterialData->Material9.Specular);
		pbyMaterial = LFileReader::Convert(pbyMaterial, pLMaterialData->Material9.Emissive);
		pbyMaterial = LFileReader::Convert(pbyMaterial, pLMaterialData->Material9.Power);
		NORMALIZE_MAT_POWER(pLMaterialData->Material9.Power);
		pbyMaterial = LFileReader::Convert(pbyMaterial, pLMaterialData->dwOption);
		pbyMaterial = LFileReader::Convert(pbyMaterial, dwMaterialOptionCount);

		for (DWORD j = 0; j < dwMaterialOptionCount; j++)
		{
			hr = LoadMaterialOption(pbyMaterial, pLMaterialData);
			HRESULT_ERROR_BREAK(hr);
		}

		pbyMaterial = LFileReader::Convert(pbyMaterial, pLMaterialData->dwNumUsedTexture);
		pLMaterialData->pTextureInfo = new MaterialTexture[pLMaterialData->dwNumUsedTexture];

		for (DWORD dwTextIndex = 0; dwTextIndex < pLMaterialData->dwNumUsedTexture; dwTextIndex++)
		{
			pbyMaterial = LFileReader::Convert(pbyMaterial, pLMaterialData->pTextureInfo[dwTextIndex].pTexture);

			BOOL_ERROR_BREAK(pbyMaterial);

			pNewOption = new L3DTexture::_MtlOption;
			BOOL_ERROR_BREAK(pNewOption);

			for (DWORD j = 0; j < pLMaterialData->pTextureInfo[dwTextIndex].pTexture->dwTextureOptionCount; j++)
			{
				pbyMaterial = LFileReader::Convert(pbyMaterial, pNewOption->Type);

				switch(pNewOption->Type)
				{
				case OPTION_TEXTURE_OPERATION:
					{
						L3DTexture::_MtlTextureOp* pDataReadIn = new L3DTexture::_MtlTextureOp;
						pbyMaterial = LFileReader::Copy(pbyMaterial, pDataReadIn);

						pNewOption->pData = pDataReadIn;
						break;
					}
				case OPTION_TEXTURE_OPERATIONEX:
					{
						L3DTexture::_TextureOpEx* pDataReadIn = new L3DTexture::_TextureOpEx;
						pbyMaterial = LFileReader::Copy(pbyMaterial, pDataReadIn);

						pNewOption->pData = pDataReadIn;
						break;
					}
				case OPTION_TEXTURE_MAPMETHOD:
					{
						L3DTexture::_TextureMap* pDataReadIn = new L3DTexture::_TextureMap;
						pbyMaterial = LFileReader::Copy(pbyMaterial, pDataReadIn);

						pNewOption->pData = pDataReadIn;
						break;
					}
				case OPTION_TEXTURE_TRANSFROM:
					{
						L3DTexture::_TextureTf* pDataReadIn = new L3DTexture::_TextureTf;
						pbyMaterial = LFileReader::Copy(pbyMaterial, pDataReadIn);

						pNewOption->pData = pDataReadIn;
						break;
					}
				}
				pLMaterialData->pTextureInfo[dwTextIndex].TextureOptions.push_back(pNewOption);
			}
		}

		if (pLMaterialData->dwOption & MATERIAL_OPTION_VERSION_2)
		{
			if (pLMaterialData->dwOption & MATERIAL_OPTION_INCLUDEALLDEFAULTCOLORCAST)
			{
				pbyMaterial = LFileReader::Copy<D3DCOLORVALUE, MaterialSubset::cNumColorCast>(pbyMaterial, pLMaterialData->ColorCast);
			}
			else
			{
				pbyMaterial = LFileReader::Convert(pbyMaterial, pLMaterialData->ColorCast[0]);
				pLMaterialData->dwOption |= MATERIAL_OPTION_INCLUDEALLDEFAULTCOLORCAST;
			}

			pbyMaterial = LFileReader::Convert(pbyMaterial, pLMaterialData->fSpecPower);
			pbyMaterial = LFileReader::Convert(pbyMaterial, pLMaterialData->fEmssPower);
		}

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT L3DMaterial::LoadMaterialOption(BYTE*& pbyMaterial, MaterialSubset* pLMaterialData)
{
	DWORD dwType = 0;
	DWORD dwData = 0;

	pbyMaterial = LFileReader::Convert(pbyMaterial, dwType);
	switch(dwType)
	{
	case OPTION_MATERIAL_ZFUN:
		{
			pbyMaterial = LFileReader::Convert(pbyMaterial, dwData);
			break;
		}
	case OPTION_MATERIAL_CULLING:
		{
			_MatCull *pCullOption = NULL;

			pbyMaterial = LFileReader::Convert(pbyMaterial, pCullOption);

			//pOption->pData = pMatCull;
			//move data into material option
			pLMaterialData->dwOption &= ~MATERIAL_OPTION_CULL_CCW;
			pLMaterialData->dwOption &= ~MATERIAL_OPTION_CULL_CW;
			pLMaterialData->dwOption &= ~MATERIAL_OPTION_CULL_NONE;
			switch(pCullOption->CullMode)
			{
			case D3DCULL_CCW:
				{
					pLMaterialData->dwOption |= MATERIAL_OPTION_CULL_CCW;
					break;
				}
			case D3DCULL_CW:
				{
					pLMaterialData->dwOption |= MATERIAL_OPTION_CULL_CW;
					break;
				}
			case D3DCULL_NONE:
				{
					pLMaterialData->dwOption |= MATERIAL_OPTION_CULL_NONE;
					break;
				}
			}
			break;
		}
	case OPTION_MATERIAL_ALPHABLEND:
		{
			_MatBlend *pBlendOption = NULL;
			//_MtlAlphaOperation* pMtlAlphaOp = GetAlphaOperation();

			pbyMaterial = LFileReader::Convert(pbyMaterial, pBlendOption);

			//pMtlAlphaOp->SrcBlend  = pBlendOption->SrcBlend;
			//pMtlAlphaOp->DestBlend = pBlendOption->DestBlend;

			//pMtlAlphaOp->EnableAlphaBlend = TRUE;
			break;
		}
	case OPTION_MATERIAL_ALPHAOPERATION:
		{
			_MtlAlphaOperation* pAlphaOption = NULL; //GetAlphaOperation();

			pbyMaterial = LFileReader::Convert(pbyMaterial, pAlphaOption);
			break;
		}
	case OPTION_MATERIAL_ALPHARENDERTWICE:
		{
			_MtlRenderTwiceAlphaOption *pAlphaOption = NULL; //GetAlphaRenderTwice();

			pbyMaterial = LFileReader::Convert(pbyMaterial, pAlphaOption);
			break;
		}
	case OPTION_MATERIAL_MSEFFECT:
		{
			struct _SAVED_EFFECT
			{
				char szFileName[MAX_PATH];
				int  nTechnique;
				int  nEffParaIndex;
			};

			pbyMaterial += sizeof(_SAVED_EFFECT);
			break;
		}
	}

	pLMaterialData->dwOption &= ~MATERIAL_OPTION_FILL_WIREFRAME;

	return S_OK;
}
