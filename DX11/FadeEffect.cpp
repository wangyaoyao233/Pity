/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/
#include "GameTimer.h"
#include "Mathf.h"
/*scenes header*/

/*gameobjects header*/
#include "Light.h"
/*self header*/
#include "FadeEffect.h"

void CFadeEffect::Init()
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

	const WCHAR* filename = L"./asset/texture/fade.png";
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(), filename, NULL, NULL, &m_Texture, NULL);

	m_Cnt = 0;
}

void CFadeEffect::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
}

void CFadeEffect::Update()
{
	m_Cnt++;

	if (m_Cnt >= FrameCnts)
	{
		this->SetDestroy();
	}
}

void CFadeEffect::Draw()
{
	//关闭灯光
	LIGHT light;
	light.Enable = false;
	CRenderer::SetLight(light);

	D3D11_MAPPED_SUBRESOURCE msr;
	CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	vertex[0].Position = D3DXVECTOR3(0, 0, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	if (m_Mode == 0)//FadeIn
	{
		vertex[0].Diffuse = D3DXVECTOR4(m_Color.x, m_Color.y, m_Color.z, m_Cnt * (1.0f / FrameCnts));
		vertex[1].Diffuse = D3DXVECTOR4(m_Color.x, m_Color.y, m_Color.z, m_Cnt * (1.0f / FrameCnts));
		vertex[2].Diffuse = D3DXVECTOR4(m_Color.x, m_Color.y, m_Color.z, m_Cnt * (1.0f / FrameCnts));
		vertex[3].Diffuse = D3DXVECTOR4(m_Color.x, m_Color.y, m_Color.z, m_Cnt * (1.0f / FrameCnts));
	}
	else if (m_Mode == 1)//FadeOut
	{
		vertex[0].Diffuse = D3DXVECTOR4(m_Color.x, m_Color.y, m_Color.z, 1.0f - m_Cnt * (1.0f / FrameCnts));
		vertex[1].Diffuse = D3DXVECTOR4(m_Color.x, m_Color.y, m_Color.z, 1.0f - m_Cnt * (1.0f / FrameCnts));
		vertex[2].Diffuse = D3DXVECTOR4(m_Color.x, m_Color.y, m_Color.z, 1.0f - m_Cnt * (1.0f / FrameCnts));
		vertex[3].Diffuse = D3DXVECTOR4(m_Color.x, m_Color.y, m_Color.z, 1.0f - m_Cnt * (1.0f / FrameCnts));
	}

	CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	//set world, view, projection matrix
	CRenderer::SetWorldViewProjection2D();

	//set vertex buffer
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//set texture(shader resources)
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//set material
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	//set primitive topology
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//draw
	CRenderer::GetDeviceContext()->Draw(4, 0);

	light.Enable = true;
	CRenderer::SetLight(light);
}