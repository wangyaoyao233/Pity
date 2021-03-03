/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "GameObject.h"
/*tools header*/
#include "input.h"
#include "GameTimer.h"
#include "Mathf.h"
/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/
#include "GameTimer.h"
#include "Mathf.h"
/*scenes header*/

/*gameobjects header*/
#include "FirstPersonCamera.h"
#include "Cube.h"
/*self header*/
#include "Line.h"

void CLine::Init()
{
	VERTEX_3D vertex[4];
	{
	}
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

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void CLine::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexBuffer = NULL;
}

void CLine::Update()
{
}

void CLine::Draw()
{
	//GetPlayerPosition
	//get scene
	CScene* scene = CManager::GetScene();
	D3DXVECTOR3 endPoint = scene->GetGameObject<CFirstPersonCamera>()->GetPosition();

	/// <summary>
	/// map & unmap
	/// </summary>
	D3D11_MAPPED_SUBRESOURCE msr;
	CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	{
		vertex[0].Position = m_Parent->GetPosition();
		vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

		vertex[1].Position = D3DXVECTOR3(endPoint.x, endPoint.y, endPoint.z);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
	}
	CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	///D3DXMATRIX
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixIdentity(&world);
	//scaling rotation translation
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	///set worldMatrix
	CRenderer::SetWorldMatrix(reinterpret_cast<D3DXMATRIX*>(&world));

	///set material
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	///set vertex buffer
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	///set primitive topology
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	CRenderer::GetDeviceContext()->Draw(4, 0);
}