#include "LCube.h"
#include "LAssert.h"
#include "L3DInterface.h"

LCube::LCube()
{
	m_fAngleX = D3DX_PI / 4.0f;
	m_fAngleY = 0;
}

LCube::~LCube()
{

}

HRESULT LCube::Setup(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	TexVertex* pVertices = NULL;
	IDirect3DVertexBuffer9* pVertexBuffer = NULL;
	IDirect3DIndexBuffer9* pIndexBuffer = NULL;
	WORD* pwIndices = NULL;

	do
	{
		BOOL_ERROR_BREAK(p3DDevice);
		BOOL_ERROR_BREAK(p3DEngine);

		pVertices = new TexVertex[8];
		pVertices[0] = TexVertex(-1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255, 0, 0),  0.0f,  1.0f);
		pVertices[1] = TexVertex(-1.0f,  1.0f, -1.0f, D3DCOLOR_XRGB(0, 255, 0),  0.0f,  0.0f);
		pVertices[2] = TexVertex( 1.0f,  1.0f, -1.0f, D3DCOLOR_XRGB(0, 0, 255),  1.0f,  0.0f);
		pVertices[3] = TexVertex( 1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255, 0, 0),  0.0f,  0.0f);
		pVertices[4] = TexVertex(-1.0f, -1.0f,  1.0f, D3DCOLOR_XRGB(0, 255, 0),  1.0f,  1.0f);
		pVertices[5] = TexVertex(-1.0f,  1.0f,  1.0f, D3DCOLOR_XRGB(0, 0, 255),  0.0f,  1.0f);
		pVertices[6] = TexVertex( 1.0f,  1.0f,  1.0f, D3DCOLOR_XRGB(255, 0, 0),  1.0f,  1.0f);
		pVertices[7] = TexVertex( 1.0f, -1.0f,  1.0f, D3DCOLOR_XRGB(0, 255, 0),  1.0f,  0.0f);

		pwIndices = new WORD[36];
		pwIndices[0] = 0; pwIndices[1] = 1; pwIndices[2] = 2;
		pwIndices[3] = 0; pwIndices[4] = 2; pwIndices[5] = 3;
		// 背面
		pwIndices[6] = 7; pwIndices[7] = 5; pwIndices[8] = 4;
		pwIndices[9] = 7; pwIndices[10] = 6; pwIndices[11] = 5;
		// 左面
		pwIndices[12] = 4; pwIndices[13] = 5; pwIndices[14] = 1;
		pwIndices[15] = 4; pwIndices[16] = 1; pwIndices[17] = 0;
		// 右面
		pwIndices[18] = 3; pwIndices[19] = 2; pwIndices[20] = 6;
		pwIndices[21] = 3; pwIndices[22] = 6; pwIndices[23] = 7;
		// 顶部
		pwIndices[24] = 1; pwIndices[25] = 5; pwIndices[26] = 6;
		pwIndices[27] = 1; pwIndices[28] = 6; pwIndices[29] = 2;
		// 底部
		pwIndices[30] = 4; pwIndices[31] = 0; pwIndices[32] = 3;
		pwIndices[33] = 4; pwIndices[34] = 3; pwIndices[35] = 7;

		hr = ILModel::Create(p3DEngine, pVertices, 8 * sizeof(TexVertex), pwIndices, 36 * sizeof(WORD), &m_pObject);
		HRESULT_ERROR_BREAK(hr);

		hr = m_pObject->SetScale(1.5);
		HRESULT_ERROR_BREAK(hr);

		hr = m_pObject->SetMaterial(L3D::GREEN_MTL);
		HRESULT_ERROR_BREAK(hr);

		hr = m_pObject->SetTexture("res/texture.png");
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT LCube::Display(IL3DEngine* p3DEngine, IDirect3DDevice9* p3DDevice, float fDeltaTime)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	D3DXQUATERNION qRotation;

	do 
	{
		BOOL_ERROR_BREAK(p3DDevice);

		D3DXQuaternionRotationYawPitchRoll(&qRotation, m_fAngleY, m_fAngleX, 0);
		m_pObject->SetRotation(qRotation);

		m_fAngleY += fDeltaTime;

		hResult = S_OK;

	} while (0);

	return hResult;
}