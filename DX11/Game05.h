#pragma once
#include "Scene.h"
class CGame05 : public CScene
{
public:
	void Init();
	void Update();
	void Uninit();
private:
	class CAudio* m_Audio;
};
