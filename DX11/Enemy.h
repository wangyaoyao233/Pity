#pragma once
#include "GameObject.h"
class CEnemy :public CGameObject
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetShootAngle(D3DXVECTOR2 angle) { m_ShootAngle = angle; }

	static void Load();
	static void Unload();

private:
	static class CModel* m_Model;
	D3DXVECTOR2 m_ShootAngle;
	int m_Cnt;
	int m_ShootTiming;
};
