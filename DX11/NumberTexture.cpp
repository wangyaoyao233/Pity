/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/

/*scenes header*/

/*gameobjects header*/

/*self header*/
#include "NumberTexture.h"

std::pair<float, float> GetUVFromNum(int n)
{
	float u = (n % 5) * 0.2f;
	float v = (n / 5) * 0.2f;
	return std::make_pair(u, v);
}

void CNumberTexture::Init()
{
}

void CNumberTexture::Init(float offset_x, float offset_y, float size_x, float size_y)
{
	VERTEX_3D vertex[4];

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DEFAULT;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bd.CPUAccessFlags = 0;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	const WCHAR* filename = L"./asset/texture/number.png";
	//const WCHAR* filename = L"./asset/texture/hp.png";
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(), filename, NULL, NULL, &m_Texture, NULL);

	m_PX = offset_x;
	m_PY = offset_y;
	m_SX = size_x;
	m_SY = size_y;
}

void CNumberTexture::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexBuffer = NULL;
	m_Texture->Release();
	m_Texture = NULL;
}

void CNumberTexture::Update()
{
	m_Ten = m_Number / 10;
	m_One = m_Number % 10;
}

void CNumberTexture::Draw()
{
	//10位
	//Map & Unmap
	{
		float u = GetUVFromNum(m_Ten).first;
		float v = GetUVFromNum(m_Ten).second;

		D3D11_MAPPED_SUBRESOURCE msr;
		CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = D3DXVECTOR3(m_PX, m_PY, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(u, v);

		vertex[1].Position = D3DXVECTOR3(m_PX + m_SX, m_PY, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(u + 0.2f, v);

		vertex[2].Position = D3DXVECTOR3(m_PX, m_PY + m_SY, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(u, v + 0.2f);

		vertex[3].Position = D3DXVECTOR3(m_PX + m_SX, m_PY + m_SY, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(u + 0.2f, v + 0.2f);

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

	//个位
		//Map & Unmap
	{
		float u = GetUVFromNum(m_One).first;
		float v = GetUVFromNum(m_One).second;

		D3D11_MAPPED_SUBRESOURCE msr;
		CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = D3DXVECTOR3(m_PX + m_SX * 0.7, m_PY, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(u, v);

		vertex[1].Position = D3DXVECTOR3(m_PX + m_SX * 1.7, m_PY, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(u + 0.2f, v);

		vertex[2].Position = D3DXVECTOR3(m_PX + m_SX * 0.7, m_PY + m_SY, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(u, v + 0.2f);

		vertex[3].Position = D3DXVECTOR3(m_PX + m_SX * 1.7, m_PY + m_SY, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(u + 0.2f, v + 0.2f);

		CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}

	//set world, view, projection matrix
	CRenderer::SetWorldViewProjection2D();

	//set vertex buffer
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//set material
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