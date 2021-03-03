#pragma once
#include "Scene.h"
class CResult : public CScene
{
public:
	void Init() override;
	void Update() override;
	void Uninit() override;
};
