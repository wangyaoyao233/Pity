#pragma once
#include "GameObject.h"
class CAchievementManager :public CGameObject
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

public:
	static bool isFistPlay;
	static bool isGoodJob;
	static bool isDefeatEnemy;
	static bool isClearAll;

	class CAudio* m_Audio;
};
