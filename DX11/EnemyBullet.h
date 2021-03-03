#pragma once
#include "GameObject.h"
class CEnemyBullet :public CGameObject
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static void Load();
	static void Unload();

	void SetDirection(D3DXVECTOR3 dir) { m_Direction = dir; }

private:
	static class CModel* m_Model;

	D3DXVECTOR3 m_Direction;
	int m_Life;
};
