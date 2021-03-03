/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/

/*scenes header*/

/*gameobjects header*/
#include "Boss01.h"
/*self header*/
#include "HPTexture.h"

void CHPTexture::Init()
{
}

void CHPTexture::Init(float offset_x, float offset_y, float size_x, float size_y)
{
	VERTEX_3D vertex[4];

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	const WCHAR* filename = L"./asset/texture/hp.png";
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(), filename, NULL, NULL, &m_Texture, NULL);

	m_PX = offset_x;
	m_PY = offset_y;
	m_SX = size_x;
	m_SY = size_y;

	m_OneSize = (float)m_SX / (float)CBoss::m_HP;
}

void CHPTexture::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexBuffer = NULL;
	m_Texture->Release();
	m_Texture = NULL;
}

void CHPTexture::Update()
{
	m_SX = m_OneSize * m_Number;
}

void CHPTexture::Draw()
{
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = D3DXVECTOR3(m_PX, m_PY, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(0, 0);

		vertex[1].Position = D3DXVECTOR3(m_PX + m_SX, m_PY, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1, 0);

		vertex[2].Position = D3DXVECTOR3(m_PX, m_PY + m_SY, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0, 1);

		vertex[3].Position = D3DXVECTOR3(m_PX + m_SX, m_PY + m_SY, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1, 1);

		CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}

	//set world, view, projection matrix
	CRenderer::SetWorldViewProjection2D();

	//set vertex buffer
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//set material
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	//set texture(shader resources)
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//set primitive topology
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//draw
	CRenderer::GetDeviceContext()->Draw(4, 0);
}