#pragma once

// マテリアル構造体
struct MODEL_MATERIAL
{
	WCHAR						Name[256];
	MATERIAL					Material;
	WCHAR						TextureName[256];
	ID3D11ShaderResourceView* Texture;
};

// 描画サブセット構造体
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};

// モデル構造体
struct MODEL
{
	VERTEX_3D* VertexArray;
	unsigned int	VertexNum;

	unsigned int* IndexArray;
	unsigned int	IndexNum;

	SUBSET* SubsetArray;
	unsigned int	SubsetNum;
};

class CModel
{
private:

	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;

	SUBSET* m_SubsetArray;
	unsigned int	m_SubsetNum;

	void LoadObj(const WCHAR* FileName, MODEL* Model);
	void LoadMaterial(const WCHAR* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum);

public:

	void Draw();

	void Load(const WCHAR* FileName);
	void Unload();
};