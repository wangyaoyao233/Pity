#pragma once
#include <typeinfo>

#include "GameObject.h"

//Layer 0:  Camera
//Layer 1:  3D Object
//Layer 2:  2D Object
#define LAYERNUM	3

class CScene
{
public:
	CScene() {}
	virtual ~CScene() {}

	virtual void Init() = 0;

	///// <summary>
	///// Load resource
	///// </summary>
	//{
	//	CCube::Load();
	//	CBullet::Load();
	//	CExplosion::Load();
	//	CPolygon::Load();
	//}
	//AddGameObject<CFirstPersonCamera>(0);
	//AddGameObject<CCamera>(0);
	//AddGameObject<CField>(1);

	//for (int i = 0; i < 10; i++)
	//{
	//	AddGameObject<CCube>(1)->SetPosition(D3DXVECTOR3(Mathf::Random<int>(-10, 10), Mathf::Random<int>(0, 10), Mathf::Random<int>(8, 10)));
	//}

	//AddGameObject<CPolygon>(2);
	//AddGameObject<CPolygonAnimation>(2);
	//AddGameObject<CSkyBox>(1);

	virtual void Uninit()
	{
		for (int i = 0; i < LAYERNUM; i++)
		{
			for (auto object : m_GameObject[i])
			{
				object->Uninit();
				delete object;
			}
			m_GameObject[i].clear();
		}
	}

	virtual void Update()
	{
		for (int i = 0; i < LAYERNUM; i++)
		{
			for (auto object : m_GameObject[i])
			{
				object->Update();
			}

			m_GameObject[i].remove_if(
				[](CGameObject* object)
				{
					return object->Destroy();
				});
		}

		//sort by layer
		//m_GameObject.sort(
		//	[](CGameObject* o1, CGameObject* o2)
		//	{
		//		return (o1->GetPriority() < o2->GetPriority());
		//	});
	}

	virtual void Draw();

	template<typename T>
	T* AddGameObject(int layer)
	{
		T* gameObject = new T();
		m_GameObject[layer].push_back(gameObject);
		gameObject->Init();

		return gameObject;
	}

	template<typename T>
	T* AddGameObjectNoInit(int layer)
	{
		T* gameObject = new T();
		m_GameObject[layer].push_back(gameObject);

		return gameObject;
	}

	template<typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < LAYERNUM; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))
				{
					return (T*)object;
				}
			}
		}

		return NULL;
	}

	template<typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> result;
		for (int i = 0; i < LAYERNUM; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))
				{
					result.push_back((T*)object);
				}
			}
		}

		return result;
	}

	CGameObject* GetGameObjectWithTag(std::string tag)
	{
		for (int i = 0; i < LAYERNUM; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				if (object->GetTag() == tag)
				{
					return object;
				}
			}
		}

		return NULL;
	}

	int m_SceneId;//Record SceneId
protected:
	std::list<CGameObject*> m_GameObject[LAYERNUM];//layer: 0:camera  1:3d object  2:2d object
};