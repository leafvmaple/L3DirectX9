#include <atlconv.h>
#include "L3DTexture.h"
#include "LAssert.h"
#include "IOManager/LFileReader.h"
#include "L3DInterface.h"

L3DTexture::L3DTexture()
: m_dwTextureCount(0)
, m_p3DDevice(NULL)
{
	m_arrSubTextureCount.clear();
	m_arrTexture.clear();
}

L3DTexture::~L3DTexture()
{

}


HRESULT L3DTexture::LoadLTexture(LPDIRECT3DDEVICE9 p3DDevice, LPCWSTR cszFileName)
{
	m_p3DDevice = p3DDevice;
	return S_OK;
}

HRESULT L3DTexture::LoadLTextureByParam(LPDIRECT3DDEVICE9 p3DDevice, const LTextureParam* pTextureParam, size_t nCount)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do 
	{
		m_arrTexture.resize(nCount);
		m_arrSubTextureCount.resize(nCount);
		m_dwTextureCount = nCount;
		for (DWORD i = 0; i < nCount; i++)
		{
			m_arrTexture[i].resize(pTextureParam[i].pSubTextureCount);
			m_arrSubTextureCount[i] = pTextureParam[i].pSubTextureCount;
			for (DWORD j = 0;j < pTextureParam[i].pSubTextureCount; j ++)
			{
				hr = D3DXCreateTextureFromFile(p3DDevice, pTextureParam[i].pSubTexture[j].wszTextureFileName, &m_arrTexture[i][j]);
				HRESULT_ERROR_BREAK(hr);
			}
		}
		m_p3DDevice = p3DDevice;

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT L3DTexture::UpdateTexture(DWORD dwSubTexture)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;

	do
	{
		for (DWORD i = 0; i < m_arrSubTextureCount[dwSubTexture]; i++)
		{
			hr = m_p3DDevice->SetTexture(i, m_arrTexture[dwSubTexture][i]);
			HRESULT_ERROR_BREAK(hr);

			hResult = S_OK;
		}

	} while (0);

	return hResult;
}
