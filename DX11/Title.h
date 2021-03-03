#pragma once
#include "Scene.h"
class CTitle : public CScene
{
public:
	void Init() override;
	void Update() override;
	void Uninit() override;
private:
	class CAudio* m_Audio;
};
