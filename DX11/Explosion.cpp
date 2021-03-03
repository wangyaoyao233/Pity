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
#include "Light.h"

/*self header*/
#include "Explosion.h"

ID3D11ShaderResourceView* CExplosion::m_Texture = NULL;
ID3D11Buffer* CExplosion::m_VertexBuffer = NULL;
/// <summary>
/// static method
/// </summary>
void CExplosion::Load()
{
	/// <summary>
	/// Texture
	/// </summary>
	const WCHAR* filename = L"asset/texture/explosion.png";
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		filename,
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	/// <summary>
	/// Vertex
	/// </summary>
	VERTEX_3D vertex[4];
	vertex[0].Position = D3DXVECTOR3(-2.0f, 2.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(2.0f, 2.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	/// <summary>
	/// VertexBuffer
	/// </summary>
	D3D11_BUFFER_DESC bd;//局部变量会优先
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;//局部变量会优先
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
}

void CExplosion::Unload()
{
	m_Texture->Release();
	m_Texture = NULL;

	m_VertexBuffer->Release();
	m_VertexBuffer = NULL;
}

void CExplosion::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void CExplosion::Uninit()
{
}

void CExplosion::Update()
{
	m_Cnt++;
	if (m_Cnt > (15 * 3))
	{
		SetDestroy();
		return;
	}
}

void CExplosion::Draw()
{
	//Map & Unmap

		//Update(0.25f, 0.25f, 16, 3);
	int speed = 3;//play speed
	int counts = 15;//the sum of frame

	float u = 0.25f;
	float v = 0.25f;
	float x = (m_Cnt / speed) % int(1 / u) * u;
	float y = (m_Cnt / speed) / int(1 / v) * v;

	D3D11_MAPPED_SUBRESOURCE msr;
	CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(-2.0f, 2.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(x, y);

	vertex[1].Position = D3DXVECTOR3(2.0f, 2.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(x + u, y);

	vertex[2].Position = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(x, y + v);

	vertex[3].Position = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(x + u, y + v);

	CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	///D3DXMATRIX
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixIdentity(&world);
	//scaling rotation translation
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	///get camera viewMatrix
	D3DXMATRIX viewInverse;
	///get scene
	CScene* scene = CManager::GetScene();
	viewInverse = scene->GetGameObject<CFirstPersonCamera>()->GetViewMatrix();
#if 0
	///calculate inverseMatrix (method 1)
	{
		D3DXMatrixInverse(&viewInverse, NULL, &viewInverse);
		//位置移动无效
		viewInverse._41 = 0.0f;
		viewInverse._42 = 0.0f;
		viewInverse._43 = 0.0f;
		world = viewInverse * world;
	}
#elif 1
	///calculate inverseMatrix (method 2)
	{
		//正規直行行列の場合のみ逆行列と転置行列が等しくなる
		world._11 = viewInverse._11;
		world._12 = viewInverse._21;
		world._13 = viewInverse._31;
		world._21 = viewInverse._12;// ←
		world._22 = viewInverse._22;// ←ここをコメントアウトすると垂直方向は無視
		world._23 = viewInverse._32;// ←
		world._31 = viewInverse._13;
		world._32 = viewInverse._23;
		world._33 = viewInverse._33;
	}
#endif
	///set worldMatrix
	CRenderer::SetWorldMatrix(reinterpret_cast<D3DXMATRIX*>(&world));

	///set vertex buffer
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	///set material
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	///set texture(shader resources)
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	///set primitive topology
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	/// <summary>
	/// light unenable
	/// </summary>
	CLight* light = new CLight(D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f));
	light->SetEnable(false);

	///draw
	CRenderer::SetBlendState(1);
	CRenderer::GetDeviceContext()->Draw(4, 0);
	CRenderer::SetBlendState(0);

	/// <summary>
	/// light enable
	/// </summary>
	light->SetEnable(true);
	delete light;
	light = NULL;
}

void CExplosion::SetUV(float x, float y, float u, float v)
{
	//vertex[0].TexCoord.x = x;
	//vertex[0].TexCoord.y = y;

	//vertex[1].TexCoord.x = x + u;
	//vertex[1].TexCoord.y = y;

	//vertex[2].TexCoord.x = x;
	//vertex[2].TexCoord.y = y + v;

	//vertex[3].TexCoord.x = x + u;
	//vertex[3].TexCoord.y = y + v;

	//if (m_VertexBuffer)
	//{
	//	m_VertexBuffer->Release();
	//	m_VertexBuffer = 0;
	//}
	//CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
}

void CExplosion::Update(float u, float v, int counts, int speed)
{
	//int x = 1 / u;
	//int y = 1 / v;
	//SetUV((m_Cnt / speed) % x * u, (m_Cnt / speed) / y * v, u, v);
}