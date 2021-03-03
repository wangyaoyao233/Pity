#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "main.h"
#include "renderer.h"
#include "model.h"

void CModel::Draw()
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (unsigned int i = 0; i < m_SubsetNum; i++)
	{
		// マテリアル設定
		CRenderer::SetMaterial(m_SubsetArray[i].Material.Material);

		// テクスチャ設定
		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_SubsetArray[i].Material.Texture);

		// ポリゴン描画
		CRenderer::GetDeviceContext()->DrawIndexed(m_SubsetArray[i].IndexNum, m_SubsetArray[i].StartIndex, 0);
	}
}

void CModel::Load(const WCHAR* FileName)
{
	MODEL model;
	LoadObj(FileName, &model);

	// 頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * model.VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.VertexArray;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	// インデックスバッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * model.IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.IndexArray;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	// サブセット設定
	{
		m_SubsetArray = new SUBSET[model.SubsetNum];
		m_SubsetNum = model.SubsetNum;

		for (unsigned int i = 0; i < model.SubsetNum; i++)
		{
			m_SubsetArray[i].StartIndex = model.SubsetArray[i].StartIndex;
			m_SubsetArray[i].IndexNum = model.SubsetArray[i].IndexNum;

			m_SubsetArray[i].Material.Material = model.SubsetArray[i].Material.Material;

			m_SubsetArray[i].Material.Texture = NULL;

			D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
				model.SubsetArray[i].Material.TextureName,
				NULL,
				NULL,
				&m_SubsetArray[i].Material.Texture,
				NULL);

			assert(m_SubsetArray[i].Material.Texture);
		}
	}

	delete[] model.VertexArray;
	delete[] model.IndexArray;
	delete[] model.SubsetArray;
}

void CModel::Unload()
{
	m_VertexBuffer->Release();

	m_IndexBuffer->Release();

	for (unsigned int i = 0; i < m_SubsetNum; i++)
	{
		m_SubsetArray[i].Material.Texture->Release();
	}

	delete[] m_SubsetArray;
}

//モデル読込////////////////////////////////////////////
void CModel::LoadObj(const WCHAR* FileName, MODEL* Model)
{
	WCHAR dir[MAX_PATH];
	wcscpy(dir, FileName);
	PathRemoveFileSpec(dir);

	D3DXVECTOR3* positionArray;
	D3DXVECTOR3* normalArray;
	D3DXVECTOR2* texcoordArray;

	unsigned int	positionNum = 0;
	unsigned int	normalNum = 0;
	unsigned int	texcoordNum = 0;
	unsigned int	vertexNum = 0;
	unsigned int	indexNum = 0;
	unsigned int	in = 0;
	unsigned int	subsetNum = 0;

	MODEL_MATERIAL* materialArray = NULL;
	unsigned int	materialNum = 0;

	WCHAR str[256];
	WCHAR* s;
	WCHAR c;

	FILE* file;
	file = _wfopen(FileName, L"rt");
	assert(file);

	//要素数カウント
	while (true)
	{
		fwscanf(file, L"%s", str);

		if (feof(file) != 0)
			break;

		if (wcscmp(str, L"v") == 0)
		{
			positionNum++;
		}
		else if (wcscmp(str, L"vn") == 0)
		{
			normalNum++;
		}
		else if (wcscmp(str, L"vt") == 0)
		{
			texcoordNum++;
		}
		else if (wcscmp(str, L"usemtl") == 0)
		{
			subsetNum++;
		}
		else if (wcscmp(str, L"f") == 0)
		{
			in = 0;

			do
			{
				fwscanf(file, L"%s", str);
				vertexNum++;
				in++;
				c = fgetc(file);
			} while (c != L'\n' && c != L'\r');

			//四角は三角に分割
			if (in == 4)
				in = 6;

			indexNum += in;
		}
	}

	//メモリ確保
	positionArray = new D3DXVECTOR3[positionNum];
	normalArray = new D3DXVECTOR3[normalNum];
	texcoordArray = new D3DXVECTOR2[texcoordNum];

	Model->VertexArray = new VERTEX_3D[vertexNum];
	Model->VertexNum = vertexNum;

	Model->IndexArray = new unsigned int[indexNum];
	Model->IndexNum = indexNum;

	Model->SubsetArray = new SUBSET[subsetNum];
	Model->SubsetNum = subsetNum;

	//要素読込
	D3DXVECTOR3* position = positionArray;
	D3DXVECTOR3* normal = normalArray;
	D3DXVECTOR2* texcoord = texcoordArray;

	unsigned int vc = 0;
	unsigned int ic = 0;
	unsigned int sc = 0;

	fseek(file, 0, SEEK_SET);

	while (true)
	{
		fwscanf(file, L"%s", str);

		if (feof(file) != 0)
			break;

		if (wcscmp(str, L"mtllib") == 0)
		{
			//マテリアルファイル
			fwscanf(file, L"%s", str);

			WCHAR path[256];
			wcscpy(path, dir);
			wcscat(path, L"\\");
			wcscat(path, str);

			LoadMaterial(path, &materialArray, &materialNum);
		}
		else if (wcscmp(str, L"o") == 0)
		{
			//オブジェクト名
			fwscanf(file, L"%s", str);
		}
		else if (wcscmp(str, L"v") == 0)
		{
			//頂点座標
			fwscanf(file, L"%f", &position->x);
			fwscanf(file, L"%f", &position->y);
			fwscanf(file, L"%f", &position->z);
			position++;
		}
		else if (wcscmp(str, L"vn") == 0)
		{
			//法線
			fwscanf(file, L"%f", &normal->x);
			fwscanf(file, L"%f", &normal->y);
			fwscanf(file, L"%f", &normal->z);
			normal++;
		}
		else if (wcscmp(str, L"vt") == 0)
		{
			//テクスチャ座標
			fwscanf(file, L"%f", &texcoord->x);
			fwscanf(file, L"%f", &texcoord->y);
			texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if (wcscmp(str, L"usemtl") == 0)
		{
			//マテリアル
			fwscanf(file, L"%s", str);

			if (sc != 0)
				Model->SubsetArray[sc - 1].IndexNum = ic - Model->SubsetArray[sc - 1].StartIndex;

			Model->SubsetArray[sc].StartIndex = ic;

			for (unsigned int i = 0; i < materialNum; i++)
			{
				if (wcscmp(str, materialArray[i].Name) == 0)
				{
					Model->SubsetArray[sc].Material.Material = materialArray[i].Material;
					wcscpy(Model->SubsetArray[sc].Material.TextureName, materialArray[i].TextureName);
					wcscpy(Model->SubsetArray[sc].Material.Name, materialArray[i].Name);

					break;
				}
			}

			sc++;
		}
		else if (wcscmp(str, L"f") == 0)
		{
			//面
			in = 0;

			do
			{
				fwscanf(file, L"%s", str);

				s = _wcstok(str, L"/");
				Model->VertexArray[vc].Position = positionArray[_wtoi(s) - 1];
				if (s[wcslen(s) + 1] != L'/')
				{
					//テクスチャ座標が存在しない場合もある
					s = _wcstok(NULL, L"/");
					Model->VertexArray[vc].TexCoord = texcoordArray[_wtoi(s) - 1];
				}
				s = _wcstok(NULL, L"/");
				Model->VertexArray[vc].Normal = normalArray[_wtoi(s) - 1];

				Model->VertexArray[vc].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

				Model->IndexArray[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc(file);
			} while (c != L'\n' && c != L'\r');

			//四角は三角に分割
			if (in == 4)
			{
				Model->IndexArray[ic] = vc - 4;
				ic++;
				Model->IndexArray[ic] = vc - 2;
				ic++;
			}
		}
	}

	if (sc != 0)
		Model->SubsetArray[sc - 1].IndexNum = ic - Model->SubsetArray[sc - 1].StartIndex;

	fclose(file);

	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;
}

//マテリアル読み込み///////////////////////////////////////////////////////////////////
void CModel::LoadMaterial(const WCHAR* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum)
{
	WCHAR dir[MAX_PATH];
	wcscpy(dir, FileName);
	PathRemoveFileSpec(dir);

	WCHAR str[256];

	FILE* file;
	file = _wfopen(FileName, L"rt");
	assert(file);

	MODEL_MATERIAL* materialArray;
	unsigned int materialNum = 0;

	//要素数カウント
	while (true)
	{
		fwscanf(file, L"%s", str);

		if (feof(file) != 0)
			break;

		if (wcscmp(str, L"newmtl") == 0)
		{
			materialNum++;
		}
	}

	//メモリ確保
	materialArray = new MODEL_MATERIAL[materialNum];

	//要素読込
	int mc = -1;

	fseek(file, 0, SEEK_SET);

	while (true)
	{
		fwscanf(file, L"%s", str);

		if (feof(file) != 0)
			break;

		if (wcscmp(str, L"newmtl") == 0)
		{
			//マテリアル名
			mc++;
			fwscanf(file, L"%s", materialArray[mc].Name);
			wcscpy(materialArray[mc].TextureName, L"");

			materialArray[mc].Material.Emission.r = 0.0f;
			materialArray[mc].Material.Emission.g = 0.0f;
			materialArray[mc].Material.Emission.b = 0.0f;
			materialArray[mc].Material.Emission.a = 0.0f;
		}
		else if (wcscmp(str, L"Ka") == 0)
		{
			//アンビエント
			fwscanf(file, L"%f", &materialArray[mc].Material.Ambient.r);
			fwscanf(file, L"%f", &materialArray[mc].Material.Ambient.g);
			fwscanf(file, L"%f", &materialArray[mc].Material.Ambient.b);
			materialArray[mc].Material.Ambient.a = 1.0f;
		}
		else if (wcscmp(str, L"Kd") == 0)
		{
			//ディフューズ
			fwscanf(file, L"%f", &materialArray[mc].Material.Diffuse.r);
			fwscanf(file, L"%f", &materialArray[mc].Material.Diffuse.g);
			fwscanf(file, L"%f", &materialArray[mc].Material.Diffuse.b);
			materialArray[mc].Material.Diffuse.a = 1.0f;
		}
		else if (wcscmp(str, L"Ks") == 0)
		{
			//スペキュラ
			fwscanf(file, L"%f", &materialArray[mc].Material.Specular.r);
			fwscanf(file, L"%f", &materialArray[mc].Material.Specular.g);
			fwscanf(file, L"%f", &materialArray[mc].Material.Specular.b);
			materialArray[mc].Material.Specular.a = 1.0f;
		}
		else if (wcscmp(str, L"Ns") == 0)
		{
			//スペキュラ強度
			fwscanf(file, L"%f", &materialArray[mc].Material.Shininess);
		}
		else if (wcscmp(str, L"d") == 0)
		{
			//アルファ
			fwscanf(file, L"%f", &materialArray[mc].Material.Diffuse.a);
		}
		else if (wcscmp(str, L"map_Kd") == 0)
		{
			//テクスチャ
			fwscanf(file, L"%s", str);

			WCHAR path[256];
			wcscpy(path, dir);
			wcscat(path, L"\\");
			wcscat(path, str);

			wcscat(materialArray[mc].TextureName, path);
		}
	}

	fclose(file);

	*MaterialArray = materialArray;
	*MaterialNum = materialNum;
}