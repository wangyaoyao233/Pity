#pragma once
#include "GameObject.h"
class CCube;
class CLine :public CGameObject
{
public:
	void Init();
	void Update();
	void Draw();
	void Uninit();
public:
	void SetParent(CCube* parent) { m_Parent = parent; }

private:
	ID3D11Buffer* m_VertexBuffer = NULL;

	CCube* m_Parent;//设置线的父物体, 以获取起始点位置
	D3DXVECTOR3 m_End;
};
