#pragma once
#include "GameObject.h"

class CCamera :public CGameObject
{
public:
	void Init();
	void Update();
	void Draw();
	void Uninit();
	D3DXMATRIX GetViewMatrix() { return m_viewMatrix; }
	D3DXMATRIX GetProjectionMatrix() { return m_ProjectionMatrix; }

	void SetSee(bool value) { m_IsSee = value; }
	bool GetSee() { return m_IsSee; }

protected:
	D3DXVECTOR3 m_Target;

	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_ProjectionMatrix;

	bool m_IsSee = false;//true为第一人称视角, false为第三视角

private://memo
	void Memo01();
};
