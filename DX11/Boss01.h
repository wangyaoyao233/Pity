#pragma once
#include "GameObject.h"
class CBoss :public CGameObject
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetIsUnBeatable(bool value) { m_IsUnBeatable = value; }
	bool GetIsUnBeatable() { return m_IsUnBeatable; }

	static int m_HP;

private:
	void Shoot(float shootAngle);

	class CModel* m_Model;

	int m_Cnt;
	bool m_IsUnBeatable;
	int m_ShootTiming;
	bool m_FistTime[6];
};
