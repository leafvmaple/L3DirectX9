#include "Cube.h"
#include "Engine/LAssert.h"

Cube::Cube()
{
	m_pVertexBuffer = NULL;
	m_pIndexBuffer  = NULL;

	m_fAngleX = D3DX_PI / 4.0f;
	m_fAngleY = 0;
}

Cube::~Cube()
{
	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();

	if (m_pIndexBuffer)
		m_pIndexBuffer->Release();
}

HRESULT Cube::Setup(IDirect3DDevice9* p3DDevice)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	TexVertex* pVertices = NULL;
	WORD* pwIndices = NULL;
	IDirect3DTexture9* pTexture = NULL;
	D3DMATERIAL9 Material = {L3D::WHITE, L3D::WHITE, L3D::WHITE, L3D::BLACK, 5.f};
	D3DLIGHT9 DirectionalLight;

	do 
	{
		hr = p3DDevice->CreateVertexBuffer(
			8 * sizeof(TexVertex), D3DUSAGE_WRITEONLY,
			TexVertex::TEX_VERTEX_FVF , D3DPOOL_MANAGED, &m_pVertexBuffer, 0);
		HRESULT_ERROR_BREAK(hr);

		p3DDevice->CreateIndexBuffer(
			36 * sizeof(WORD), D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, 0);
		HRESULT_ERROR_BREAK(hr);

		m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

		pVertices[0] = TexVertex(-1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255, 0, 0),  0.0f,  1.0f);
		pVertices[1] = TexVertex(-1.0f,  1.0f, -1.0f, D3DCOLOR_XRGB(0, 255, 0),  0.0f,  0.0f);
		pVertices[2] = TexVertex( 1.0f,  1.0f, -1.0f, D3DCOLOR_XRGB(0, 0, 255),  1.0f,  0.0f);
		pVertices[3] = TexVertex( 1.0f, -1.0f, -1.0f, D3DCOLOR_XRGB(255, 0, 0),  0.0f,  0.0f);
		pVertices[4] = TexVertex(-1.0f, -1.0f,  1.0f, D3DCOLOR_XRGB(0, 255, 0),  1.0f,  1.0f);
		pVertices[5] = TexVertex(-1.0f,  1.0f,  1.0f, D3DCOLOR_XRGB(0, 0, 255),  0.0f,  1.0f);
		pVertices[6] = TexVertex( 1.0f,  1.0f,  1.0f, D3DCOLOR_XRGB(255, 0, 0),  1.0f,  1.0f);
		pVertices[7] = TexVertex( 1.0f, -1.0f,  1.0f, D3DCOLOR_XRGB(0, 255, 0),  1.0f,  0.0f);

		m_pVertexBuffer->Unlock();

		//L3D::InitVertexNormal(&pVertices[0]);
		//L3D::InitVertexNormal(&pVertices[3]);

		p3DDevice->SetMaterial(&Material);

		DirectionalLight = L3D::InitDirectionalLight(D3DXVECTOR3(1.f, 0.f, 0.f), L3D::WHITE);
		p3DDevice->SetLight(0, &DirectionalLight);
		p3DDevice->LightEnable(0, TRUE);

		// 定义立方体的三角形
		m_pIndexBuffer->Lock(0, 0, (void**)&pwIndices, 0);

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

		m_pIndexBuffer->Unlock();

		D3DXCreateTextureFromFile(p3DDevice, TEXT("res/texture.png"), &pTexture);
		BOOL_ERROR_BREAK(pTexture);

		hr = p3DDevice->SetTexture(0, pTexture);
		HRESULT_ERROR_BREAK(hr);

		// 渲染状态
		//p3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		p3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		p3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		p3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

		hResult = S_OK;
	} while (0);

	return hResult;
}

HRESULT Cube::Display(IDirect3DDevice9* p3DDevice, float fDeltaTime)
{
	HRESULT hr = E_FAIL;
	HRESULT hResult = E_FAIL;
	D3DXMATRIX matX;
	D3DXMATRIX matY;
	D3DXMATRIX matTransform;

	do 
	{
		BOOL_ERROR_BREAK(p3DDevice);

		D3DXMatrixRotationX(&matX, m_fAngleX);
		D3DXMatrixRotationY(&matY, m_fAngleY);

		m_fAngleY += fDeltaTime;
		if(m_fAngleY >= L3DX_2PI)
			m_fAngleY = 0.0f;

		matTransform = matX * matY;
		p3DDevice->SetTransform(D3DTS_WORLD, &matTransform);

		hr = p3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		HRESULT_ERROR_BREAK(hr);

		p3DDevice->BeginScene();
		p3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TexVertex));
		p3DDevice->SetIndices(m_pIndexBuffer);
		p3DDevice->SetFVF(TexVertex::TEX_VERTEX_FVF);
		//m_p3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);

		// 绘制存在连续内存中的12个顶点
		p3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
		p3DDevice->EndScene();

		hr = p3DDevice->Present(0, 0, 0, 0);
		HRESULT_ERROR_BREAK(hr);

		hResult = S_OK;

	} while (0);

	return hResult;
}
