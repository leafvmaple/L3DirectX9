#include "L3DMaterial.h"
#include "IO/LFileReader.h"
#include "LAssert.h"
#include "L3DInterface.h"

extern LPDIRECT3DDEVICE9 g_p3DDevice;

L3DMaterial::L3DMaterial()
: m_dwNumMaterials(0)
, m_pMaterialSubset(NULL)
{

}

L3DMaterial::~L3DMaterial()
{

}

HRESULT L3DSubsetMaterial::LoadLSubsetMaterial(LPCWSTR pcszDirectory, BYTE*& pbyMaterial)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	DWORD dwMaterialOptionCount = 0;

	do 
	{
		pbyMaterial = LFileReader::Convert(pbyMaterial, Material9.Ambient);
		pbyMaterial = LFileReader::Convert(pbyMaterial, Material9.Diffuse);
		pbyMaterial = LFileReader::Convert(pbyMaterial, Material9.Specular);
		pbyMaterial = LFileReader::Convert(pbyMaterial, Material9.Emissive);
		pbyMaterial = LFileReader::Convert(pbyMaterial, Material9.Power);
		NORMALIZE_MAT_POWER(Material9.Power);
		pbyMaterial = LFileReader::Convert(pbyMaterial, dwOption);
		pbyMaterial = LFileReader::Convert(pbyMaterial, dwMaterialOptionCount);

		for (DWORD j = 0; j < dwMaterialOptionCount; j++)
		{
			hr = LoadOption(pbyMaterial);
			HRESULT_ERROR_BREAK(hr);
		}

		pLTexture = new L3DTexture;
		BOOL_ERROR_BREAK(pLTexture);

		hr = pLTexture->LoadLTexture(pcszDirectory, pbyMaterial);
		HRESULT_ERROR_BREAK(hr);

		if (dwOption & MATERIAL_OPTION_VERSION_2)
		{
			if (dwOption & MATERIAL_OPTION_INCLUDEALLDEFAULTCOLORCAST)
			{
				pbyMaterial = LFileReader::Copy<D3DCOLORVALUE, L3DSubsetMaterial::cNumColorCast>(pbyMaterial, ColorCast);
			}
			else
			{
				pbyMaterial = LFileReader::Convert(pbyMaterial, ColorCast[0]);
				dwOption |= MATERIAL_OPTION_INCLUDEALLDEFAULTCOLORCAST;
			}

			pbyMaterial = LFileReader::Convert(pbyMaterial, fSpecPower);
			pbyMaterial = LFileReader::Convert(pbyMaterial, fEmssPower);
		}

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT L3DSubsetMaterial::LoadOption(BYTE*& pbyMaterial)
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
			dwOption &= ~MATERIAL_OPTION_CULL_CCW;
			dwOption &= ~MATERIAL_OPTION_CULL_CW;
			dwOption &= ~MATERIAL_OPTION_CULL_NONE;
			switch(pCullOption->CullMode)
			{
			case D3DCULL_CCW:
				{
					dwOption |= MATERIAL_OPTION_CULL_CCW;
					break;
				}
			case D3DCULL_CW:
				{
					dwOption |= MATERIAL_OPTION_CULL_CW;
					break;
				}
			case D3DCULL_NONE:
				{
					dwOption |= MATERIAL_OPTION_CULL_NONE;
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

	dwOption &= ~MATERIAL_OPTION_FILL_WIREFRAME;

	return S_OK;
}

HRESULT L3DSubsetMaterial::UpdateSubsetMaterial()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do
	{
		hr = g_p3DDevice->SetMaterial(&Material9);
		HRESULT_ERROR_BREAK(hr);

		hr = pLTexture->UpdateTexture();
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT L3DMaterial::LoadLMaterial(LPCWSTR cszFileName)
{
	HRESULT hResult = E_FAIL;
	BYTE* pbyMaterial = NULL;
	BYTE* pbyBufferHead = NULL;
	size_t uMaterialLen = 0;
	DWORD dwMask = 0;
	DWORD dwBlockLength = 0;
	TCHAR wcszDir[MAX_PATH];

	do 
	{
		LFileReader::Reader(cszFileName, &pbyMaterial, &uMaterialLen);
		pbyBufferHead = pbyMaterial;

		pbyMaterial = LFileReader::Convert(pbyMaterial, dwMask);
		BOOL_ERROR_BREAK(dwMask >= 0x4D41544C);//MATL对应的ASECC代码

		pbyMaterial = LFileReader::Convert(pbyMaterial, dwBlockLength);
		pbyMaterial = LFileReader::Convert(pbyMaterial, m_dwNumMaterials);
		BOOL_ERROR_BREAK(m_dwNumMaterials <= MAX_SUBSET_COUNT); // just for check file error, normally this value < 10

		m_pMaterialSubset = new L3DSubsetMaterial[m_dwNumMaterials];
		BOOL_ERROR_BREAK(m_pMaterialSubset);

		_wsplitpath_s(cszFileName, NULL, 0, wcszDir, MAX_PATH, NULL, 0, NULL, 0);

		for (DWORD i = 0; i < m_dwNumMaterials; i++)
		{
			m_pMaterialSubset[i].LoadLSubsetMaterial(wcszDir, pbyMaterial);

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
		hr = m_pMaterialSubset[dwSubMaterial].UpdateSubsetMaterial();
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);

	return hResult;
}
