#pragma once
#include "GameObject.h"
class CHPTexture :public CGameObject
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetNumber(int n) { m_Number = n; }
	void Init(float offset_x, float offset_y, float size_x, float size_y);

private:
	ID3D11ShaderResourceView* m_Texture = NULL;
	ID3D11Buffer* m_VertexBuffer = NULL;

	float m_PX;//起始点x坐标
	float m_PY;//起始点y坐标
	float m_SX;//图片大小x
	float m_SY;//图片大小y

	float m_OneSize;
	int m_Number;
};
