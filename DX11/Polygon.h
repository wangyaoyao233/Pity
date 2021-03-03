#pragma once
#include "GameObject.h"
class CPolygon :public CGameObject
{
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;

public:
	void Init(float offset_x, float offset_y, float size_x, float size_y);
	void SetFile(const WCHAR* filename);

protected:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture;
};