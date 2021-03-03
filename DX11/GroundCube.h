#pragma once
#include "GameObject.h"

class CGroundCube :public CGameObject
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static void Load();
	static void Unload();

private:
	static class CModel* m_Model;
};
