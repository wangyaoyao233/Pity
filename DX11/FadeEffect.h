#pragma once
#include "GameObject.h"
class CFadeEffect : public CGameObject
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetColor(D3DXVECTOR3 color) { m_Color = color; }
	void SetMode(int mode) { m_Mode = mode; }

private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture;

	int m_Cnt;//计数
	D3DXVECTOR3 m_Color;
	int m_Mode;// 0-FadeIn 1-FadeOut

	const int FrameCnts = 60;
};
