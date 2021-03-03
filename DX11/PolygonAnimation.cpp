/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/

/*scenes header*/

/*gameobjects header*/

/*self header*/
#include "PolygonAnimation.h"

void CPolygonAnimation::Init()
{
	float offset_x = SCREEN_WIDTH / 2;
	float offset_y = 10;
	float size_x = 32;
	float size_y = 32;

	Init(offset_x, offset_y, size_x, size_y, 10, 19);
	SetUV(0.2f, 0.2f);
}

void CPolygonAnimation::Init(float offset_x, float offset_y, float size_x, float size_y, int speed, int total_pictures)
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

	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(), L"./asset/texture/number.png", NULL, NULL, &m_Texture, NULL);
	assert(m_Texture);

	m_PX = offset_x;
	m_PY = offset_y;
	m_SX = size_x;
	m_SY = size_y;
	m_Speed = speed;
	m_TotalPictures = total_pictures;
}

void CPolygonAnimation::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexBuffer = NULL;
	m_Texture->Release();
	m_Texture = NULL;
}

void CPolygonAnimation::SetFile(const WCHAR* filename)
{
	m_Texture->Release();
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(), filename, NULL, NULL, &m_Texture, NULL);
	assert(m_Texture);
}

void CPolygonAnimation::Update()
{
	m_Cnt++;
	if (m_Cnt > (m_TotalPictures * m_Speed))
	{
		//SetDestroy();
		m_Cnt = 0;
	}
}

void CPolygonAnimation::Draw()
{
	//Map & Unmap
	{
		float x = (m_Cnt / m_Speed) % int(1 / m_Uvalue) * m_Uvalue;
		float y = (m_Cnt / m_Speed) / int(1 / m_Vvalue) * m_Vvalue;

		D3D11_MAPPED_SUBRESOURCE msr;
		CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = D3DXVECTOR3(m_PX, m_PY, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(x, y);

		vertex[1].Position = D3DXVECTOR3(m_PX + m_SX, m_PY, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(x + m_Uvalue, y);

		vertex[2].Position = D3DXVECTOR3(m_PX, m_PY + m_SY, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(x, y + m_Vvalue);

		vertex[3].Position = D3DXVECTOR3(m_PX + m_SX, m_PY + m_SY, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(x + m_Uvalue, y + m_Vvalue);

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

void CPolygonAnimation::SetUV(float x, float y, float u, float v)
{
	//m_vertex[0].TexCoord.x = x;
	//m_vertex[0].TexCoord.y = y;

	//m_vertex[1].TexCoord.x = x + u;
	//m_vertex[1].TexCoord.y = y;

	//m_vertex[2].TexCoord.x = x;
	//m_vertex[2].TexCoord.y = y + u;

	//m_vertex[3].TexCoord.x = x + u;
	//m_vertex[3].TexCoord.y = y + u;

	//if (m_VertexBuffer)
	//{
	//	m_VertexBuffer->Release();
	//	m_VertexBuffer = 0;
	//}
	//CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
}
void CPolygonAnimation::Update(float u, float v, int counts, int speed)
{
	//int x = 1 / u;
	//int y = 1 / v;
	//SetUV((m_Cnt / m_Speed) % x * u, (m_Cnt / m_Speed) / y * v, u, v);
	//m_Cnt++;
	//if (m_Cnt > (counts * m_Speed))
	//	m_Cnt = 0;
}