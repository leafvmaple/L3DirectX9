#include "L3DTexture.h"
#include "LAssert.h"
#include "IO/LFileReader.h"
#include "L3DInterface.h"

extern LPDIRECT3DDEVICE9 g_p3DDevice;

L3DTexture::L3DTexture()
{
	m_vecTextures.clear();
}

L3DTexture::~L3DTexture()
{

}


HRESULT L3DTexture::LoadLTexture(LPCWSTR cszFileName)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		_TextureBase* pTextureBase = new _TextureBase;
		ZeroMemory(pTextureBase, sizeof(_TextureBase));

		D3DXCreateTextureFromFile(g_p3DDevice, cszFileName, &pTextureBase->pTexture);
		BOOL_ERROR_BREAK(pTextureBase->pTexture)

		m_vecTextures.push_back(pTextureBase);

		hResult = S_OK;
	} while (0);
	
	return hResult;
}

HRESULT L3DTexture::LoadLTexture(LPCWSTR pcszDirectory, BYTE*& pbyTexture)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	_TEXTURE* pTextureInfo = NULL;
	_MtlOption* pOption = NULL;
	TCHAR wcszTextureName[MAX_PATH];
	DWORD dwNumUsedTexture = 0;

	do
	{
		pbyTexture = LFileReader::Convert(pbyTexture, dwNumUsedTexture);

		for (DWORD dwTextIndex = 0; dwTextIndex < dwNumUsedTexture; dwTextIndex++)
		{
			_TextureBase* pTextureBase = new _TextureBase;

			pbyTexture = LFileReader::Convert(pbyTexture, pTextureInfo);

			USES_CONVERSION;

			swprintf_s(wcszTextureName, TEXT("%s%s"), pcszDirectory, A2CW(pTextureInfo->szTextureFileName));

			hr = D3DXCreateTextureFromFile(g_p3DDevice, wcszTextureName, &pTextureBase->pTexture);
			HRESULT_ERROR_BREAK(hr);

			pTextureBase->m_vecTextureOptions.clear();

			pOption = new _MtlOption;
			BOOL_ERROR_BREAK(pOption);

			for (DWORD j = 0; j < pTextureInfo->dwTextureOptionCount; j++)
			{
				pbyTexture = LFileReader::Convert(pbyTexture, pOption->Type);

				switch(pOption->Type)
				{
				case OPTION_TEXTURE_OPERATION:
					{
						_MtlTextureOp* pDataReadIn = new _MtlTextureOp;
						pbyTexture = LFileReader::Copy(pbyTexture, pDataReadIn);

						pOption->pData = pDataReadIn;
						break;
					}
				case OPTION_TEXTURE_OPERATIONEX:
					{
						_TextureOpEx* pDataReadIn = new _TextureOpEx;
						pbyTexture = LFileReader::Copy(pbyTexture, pDataReadIn);

						pOption->pData = pDataReadIn;
						break;
					}
				case OPTION_TEXTURE_MAPMETHOD:
					{
						_TextureMap* pDataReadIn = new _TextureMap;
						pbyTexture = LFileReader::Copy(pbyTexture, pDataReadIn);

						pOption->pData = pDataReadIn;
						break;
					}
				case OPTION_TEXTURE_TRANSFROM:
					{
						_TextureTf* pDataReadIn = new _TextureTf;
						pbyTexture = LFileReader::Copy(pbyTexture, pDataReadIn);

						pOption->pData = pDataReadIn;
						break;
					}
				}
				pTextureBase->m_vecTextureOptions.push_back(pOption);
			}
			m_vecTextures.push_back(pTextureBase);
		}

		hResult = S_OK;

	} while (0);

	return hResult;
}

HRESULT L3DTexture::UpdateTexture()
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do
	{
		for (DWORD i = 0; i < m_vecTextures.size(); i++)
		{
			hr = g_p3DDevice->SetTexture(i, m_vecTextures[i]->pTexture);
			HRESULT_ERROR_BREAK(hr);
		}

		hResult = S_OK;

	} while (0);

	return hResult;
}
