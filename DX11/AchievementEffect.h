#pragma once
#include "GameObject.h"
class CAchievementEffect :public CGameObject
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetFile(const WCHAR* filename);

	void Init(float offset_x, float offset_y, float size_x, float size_y);

protected:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture;
	int m_Cnt;

	float m_PX;
	float m_PY;
	float m_SX;
	float m_SY;
};
