#pragma once
#include "GameObject.h"

class CGoal :public CGameObject
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	class CModel* m_Model;
};
