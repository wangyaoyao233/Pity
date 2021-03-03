#pragma once

#include "GameObject.h"

class CField :public CGameObject
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void Uninit();

	void SetFile(const WCHAR* filename);
	void SetSize(float size) { m_Size = size; }

private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;
	float m_Size;
};
