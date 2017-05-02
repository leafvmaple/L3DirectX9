#pragma once
#include <d3dx9.h>
#include <string>
#include <vector>

#define MAT_TEXTURETYPE_LENGTH 20
#define MAT_TEXTUREFILENAME_LENGTH 256

enum OPTION_TEXTURE
{
	OPTION_TEXTURE_OPERATION = 1000,
	OPTION_TEXTURE_MAPMETHOD = 1001,
	OPTION_TEXTURE_TRANSFROM = 1002,
	OPTION_TEXTURE_ANIMATION = 1003,
	OPTION_TEXTURE_OPERATIONEX = 1004,
};

class L3DTexture
{
private:
	struct _TEXTURE
	{
		float fAmount;
		char  szTextureType[MAT_TEXTURETYPE_LENGTH];
		char  szTextureFileName[MAT_TEXTUREFILENAME_LENGTH];
		DWORD dwTextureOptionCount;
	};

	struct LTextureParam
	{
		struct SubTextureParam
		{
			TCHAR wszTextureType[MAT_TEXTURETYPE_LENGTH];
			TCHAR wszTextureFileName[MAT_TEXTUREFILENAME_LENGTH];
		} *pSubTexture;

		DWORD pSubTextureCount;
	};

	struct _MtlOption
	{
		DWORD Type;
		PVOID pData;
	};

	struct _MtlTextureOp
	{
		DWORD ColorOp;
		DWORD ColorArg1;
		DWORD ColorArg2;
		DWORD AlphaOp;
		DWORD AlphaArg1;
		DWORD AlphaArg2;
		DWORD TFactor;

		DWORD BakColorOp;
		DWORD BakColorArg1;
		DWORD BakColorArg2;
		DWORD BakAlphaOp;
		DWORD BakAlphaArg1;
		DWORD BakAlphaArg2;
		DWORD BakTFactor;
	};

	struct _TextureOpEx
	{
		DWORD ColorOp;
		DWORD ColorArg1;
		DWORD ColorArg2;
		DWORD ColorArg0;
		DWORD Result;
		DWORD AlphaOp;
		DWORD AlphaArg1;
		DWORD AlphaArg2;
		DWORD AlphaArg0;
		DWORD AlphaResult;
		DWORD TFactor;
		DWORD Constant;

		DWORD BakColorOp;
		DWORD BakColorArg1;
		DWORD BakColorArg2;
		DWORD BakColorArg0;
		DWORD BakResult;
		DWORD BakAlphaOp;
		DWORD BakAlphaArg1;
		DWORD BakAlphaArg2;
		DWORD BakAlphaArg0;
		DWORD BakTFactor;
		DWORD BakConstant;
	};

	struct _TextureMap
	{
		DWORD TextureMap;
		DWORD BakTextureMap;
	};

	struct _TextureTf
	{
		D3DXVECTOR3 Scaling;
		D3DXVECTOR3 Rotation;
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Speed_Scaling;
		D3DXVECTOR3 Speed_Rotation;
		D3DXVECTOR3 Speed_Position;
	};

	struct _TextureBase
	{
		LPDIRECT3DTEXTURE9 pTexture;
		std::vector<_MtlOption*> m_vecTextureOptions;
	};

public:
	L3DTexture();
	~L3DTexture();

	HRESULT LoadLTexture(LPCWSTR cszFileName);
	HRESULT LoadLTexture(LPCWSTR pcszDirectory, BYTE*& pbyTexture);

	HRESULT UpdateTexture();

private:
	std::vector<_TextureBase*> m_vecTextures;
};