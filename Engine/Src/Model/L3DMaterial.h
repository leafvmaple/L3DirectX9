#pragma once
#include <d3dx9.h>
#include <string>
#include <vector>
#include "L3DTexture.h"

#define MAX_SUBSET_COUNT 32
#define NORMALIZE_MAT_POWER(a) { if (a > 100.0f || a <= 0.0f) a = 15.0f; }
#define MATERIAL_OPTION_VERSION_2        (1 << 18)

class L3DMaterial
{
public:
	struct MaterialTexture
	{
		struct _TEXTURE
		{
			float fAmount;
			char  szTextureType[MAT_TEXTURETYPE_LENGTH];
			char  szTextureFileName[MAT_TEXTUREFILENAME_LENGTH];
			DWORD dwTextureOptionCount;
		} *pTexture;

		std::vector<L3DTexture::_MtlOption*> TextureOptions;
	};

private:
	struct MaterialSubset
	{
		const static int cNumID = 8;
		const static int cNumColorCast = 8;
		D3DMATERIAL9     Material9;
		DWORD            dwNumFaces;
		//KG3DTexture*     m_lpTextures[cNumID];
		MaterialTexture* pTextureInfo;
		DWORD            dwOption;
		DWORD            dwNumOptions;
		DWORD            dwHashValue;//
		DWORD            dwNumUsedTexture;
		//KG3DTextureHolder* lpNormalMap;

		D3DCOLORVALUE    ColorCast[cNumColorCast];
		float            fSpecPower;
		float            fEmssPower;

		DWORD m_dwMaterialID;
		DWORD m_dwPassID;
	};

	struct _MatCull 
	{
		DWORD CullMode;
		DWORD BakCullMode;
	};

	struct _MatBlend
	{
		DWORD SrcBlend;
		DWORD DestBlend;
		DWORD BakSrcBlend;
		DWORD BakDestBlend;
	};

	struct _MtlAlphaOperation
	{
		DWORD EnableAlphaBlend;
		DWORD SrcBlend;
		DWORD DestBlend;
		DWORD BlendFactor;
		DWORD BlendOperator;

		DWORD EnableSeparateAlphaBlend;
		DWORD BlendOperatorAlpha;
		DWORD SrcBlendAlpha;
		DWORD DestBlendAlpha;

		DWORD EnableAlphaTest;
		DWORD AlphaTestRef;
		DWORD AlphaFunction;

		DWORD BackEnableAlphaBlend;
		DWORD BackSrcBlend;
		DWORD BackDestBlend;
		DWORD BackBlendFactor;
		DWORD BackBlendOperator;

		DWORD BackEnableSeparateAlphaBlend;
		DWORD BackBlendOperatorAlpha;
		DWORD BackSrcBlendAlpha;
		DWORD BackDestBlendAlpha;

		DWORD BackEnableAlphaTest;
		DWORD BackAlphaTestRef;
		DWORD BackAlphaFunction;
	};

	struct _MtlRenderTwiceAlphaOption
	{
		BOOL  bEnable;
		DWORD dwAlphaRefPass0;
		DWORD dwAlphaRefPass1;
		const static DWORD s_dwRef0 = 254;
		const static DWORD s_dwRef1 = 32;
	};

	enum MATERIAL_OPTION_CULL
	{
		MATERIAL_OPTION_CULL_NONE = (1<<10),
		MATERIAL_OPTION_CULL_CW   = (1<<11),
		MATERIAL_OPTION_CULL_CCW  = (1<<12),
	};

	enum MATERIAL_OPTION_FILL
	{
		MATERIAL_OPTION_FILL_POINT     = (1<<4),
		MATERIAL_OPTION_FILL_WIREFRAME = (1<<5),
		MATERIAL_OPTION_FILL_SOLID     = (1<<6),
	};

	enum OPTION_MATERIAL
	{
		OPTION_MATERIAL_LIGHTING = 100,
		OPTION_MATERIAL_CULLING = 101,
		OPTION_MATERIAL_FILLMODE = 102,
		OPTION_MATERIAL_MUTIPASS = 103,
		OPTION_MATERIAL_ALPHATEST = 104,
		OPTION_MATERIAL_ALPHABLEND = 105,
		OPTION_MATERIAL_ZBUFFER = 106,
		OPTION_MATERIAL_ZFUN = 107,
		OPTION_MATERIAL_SHADER = 200,
		OPTION_MATERIAL_PIXELSHADER = 201,
		OPTION_MATERIAL_ALPHAOPERATION = 202,
		OPTION_MATERIAL_MSEFFECT = 203,
		OPTION_MATERIAL_MSEFFECT_NEW = 204,
		OPTION_MATERIAL_ALPHARENDERTWICE = 205,
	};

	enum MATERIAL_OPTION
	{
		MATERIAL_OPTION_EMAP = (1 << 20),
		MATERIAL_OPTION_INCLUDEALLDEFAULTCOLORCAST = (1 << 21),
		MATERIAL_OPTION_SORTASSFX = (1<<22),
		MATERIAL_OPTION_DETAIL = (1<<23),
		MATERIAL_OPTION_TEXTUREBLEND = (1<<24),
		MATERIAL_OPTION_SETCOLOR = (1<<25),
	};

public:
	DWORD m_dwNumMaterials;
	MaterialSubset* m_pMaterialSubset;

public:

	L3DMaterial();
	~L3DMaterial();

	HRESULT LoadLMaterial(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR cszFileName);

	HRESULT UpdateMaterial(DWORD dwSubMaterial);

private:
	HRESULT LoadMaterialBuffer(BYTE*& pbyMaterial, MaterialSubset* pLMaterialData);
	HRESULT LoadMaterialOption(BYTE*& pbyMaterial, MaterialSubset* pLMaterialData);

private:
	LPDIRECT3DDEVICE9 m_p3DDevice;
};