#pragma once
#include "Scene.h"
class CAudio;
class CManager
{
public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static CScene* GetScene() { return m_Scene; }
	static CAudio* GetAudio() { return m_Audio; }

	template <typename T>
	static void SetScene()
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
			//CResource::UnloadAll();
		}
		T* scene = new T();
		m_Scene = scene;
		m_Scene->Init();
	}

private:
	static CScene* m_Scene;
	static CAudio* m_Audio;
};