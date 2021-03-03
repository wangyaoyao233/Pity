/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/

/*scenes header*/

/*gameobjects header*/

/*self header*/
#include "Polygon.h"

//ID3D11ShaderResourceView* CPolygon::m_Texture = NULL;
//ID3D11ShaderResourceView* CPolygon::m_Texture1 = NULL;
//void CPolygon::Load()
//{
//	const WCHAR* filename = L"./asset/texture/aim.png";
//	const WCHAR* filename1 = L"./asset/texture/sight.png";
//
//	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(), filename, NULL, NULL, &m_Texture, NULL);
//	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(), filename1, NULL, NULL, &m_Texture1, NULL);
//}
//
//void CPolygon::Unload()
//{
//	m_Texture->Release();
//	m_Texture = NULL;
//
//	m_Texture1->Release();
//	m_Texture1 = NULL;
//}

void CPolygon::SetFile(const WCHAR* filename)
{
	m_Texture->Release();
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(), filename, NULL, NULL, &m_Texture, NULL);
}

void CPolygon::Init()
{
	Init(SCREEN_WIDTH / 2 - 128, SCREEN_HEIGHT / 2 - 128, 256, 256);
}

void CPolygon::Init(float offset_x, float offset_y, float size_x, float size_y)
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(offset_x, offset_y, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(offset_x + size_x, offset_y, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(offset_x, offset_y + size_y, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(offset_x + size_x, offset_y + size_y, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	const WCHAR* filename = L"./asset/texture/aim.png";
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(), filename, NULL, NULL, &m_Texture, NULL);

	//layer = 1
	m_Priority = 1;
}

void CPolygon::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexBuffer = NULL;
	m_Texture->Release();
	m_Texture = NULL;
}

void CPolygon::Update()
{
}

void CPolygon::Draw()
{
	if (!m_IsActive)
	{
		return;
	}

	//LIGHT light;
	//light.Enable = false;
	//CRenderer::SetLight(light);

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

/*
void CPolygon::Map(D3DXMATRIX* matrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(mappedResource));

	//Calculate the transform matrix
	MatrixMultiVec3(matrix, &p0);
	MatrixMultiVec3(matrix, &p1);
	MatrixMultiVec3(matrix, &p2);
	MatrixMultiVec3(matrix, &p3);

	vertex[0].Position = p0;
	vertex[1].Position = p1;
	vertex[2].Position = p2;
	vertex[3].Position = p3;

	//disable gpu access to the vertex buffer data
	CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//update the vertex buffer here
	memcpy(mappedResource.pData, vertex, sizeof(vertex));
	//reenable gpu access to the vertex buffer data
	CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
}
*/
/*
void CPolygon::MatrixMultiVec3(D3DXMATRIX* matrix, D3DXVECTOR3* vec)
{
	D3DXVECTOR3 tmp = *vec;
	vec->x = tmp.x * matrix->_11 + tmp.y * matrix->_12 + tmp.z * matrix->_13 + matrix->_41;
	vec->y = tmp.x * matrix->_21 + tmp.y * matrix->_22 + tmp.z * matrix->_23 + matrix->_42;
	vec->z = tmp.x * matrix->_31 + tmp.y * matrix->_32 + tmp.z * matrix->_33 + matrix->_43;
}
*/
/*
void CPolygon::Translate(float x, float y, float z = 0.0f)
{
	D3DXMATRIX translateMatrix;
	D3DXMatrixTranslation(&translateMatrix, x, y, z);
	Map(&translateMatrix);
}
*/
/*
//テスト
void CPolygon::Rotate(float angle)
{
	D3DXMATRIX rotateMatrix;
	D3DXMATRIX translateMatrix1;
	D3DXMATRIX translateMatrix2;
	//1.移动到原点
	float x = (p3.x - p0.x) * 0.5f + p0.x;
	float y = (p3.y - p0.y) * 0.5f + p0.y;
	float z = (p3.z - p0.z) * 0.5f + p0.z;
	D3DXMatrixTranslation(&translateMatrix1, -x, -y, z);
	//2.旋转
	D3DXMatrixRotationZ(&rotateMatrix, angle);
	//3.移动到原来位置
	D3DXMatrixTranslation(&translateMatrix2, x, y, z);
	//rotateMatrix = m2 * rotateMatrix * m1;//x
	Map(&translateMatrix1);
	Map(&rotateMatrix);
	Map(&translateMatrix2);
}
*/