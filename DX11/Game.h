#pragma once
#include "Scene.h"
class CGame : public CScene
{
public:
	void Init();
	void Update();
	void Uninit();

private:
	class CAudio* m_Audio;
};
