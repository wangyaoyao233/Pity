/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "GameObject.h"
/*tools header*/
#include "input.h"
#include "GameTimer.h"
#include "Mathf.h"
#include "ReadMap.h"
#include "Audio.h"
/*scenes header*/
#include "Title.h"
#include "Game02.h"
/*gameobjects header*/
#include "Player.h"
#include "Polygon.h"
#include "PolygonAnimation.h"
#include "Bullet.h"
#include "Cube.h"
#include "FirstPersonCamera.h"
#include "Explosion.h"
#include "SkyBox.h"
#include "Line.h"
#include "GroundCube.h"
#include "Goal.h"
#include "FadeEffect.h"
#include "AchievementManager.h"
/*self header*/
#include "Game.h"

void CGame::Init()
{
	this->m_SceneId = 1;

	/// <summary>
	/// Load resource
	/// </summary>

	CCube::Load();
	CBullet::Load();
	CExplosion::Load();
	CGroundCube::Load();

	AddGameObject<CFirstPersonCamera>(0);
	AddGameObject<CCamera>(0);

	{//ground
		std::string filename = "./asset/map/map1.map";
		std::vector<DataInfo> mapData = CReadMap::LoadMap(filename);

		for (auto value : mapData)
		{
			if (value.name == "ground")
			{
				float x = std::stof(value.info.at(0));
				float y = std::stof(value.info.at(1));
				float z = std::stof(value.info.at(2));
				AddGameObject<CGroundCube>(1)->SetPosition(D3DXVECTOR3(x, y, z));
			}
			if (value.name == "cube")
			{
				float x = std::stof(value.info.at(0));
				float y = std::stof(value.info.at(1));
				float z = std::stof(value.info.at(2));
				AddGameObject<CCube>(1)->SetPosition(D3DXVECTOR3(x, y, z));
			}
			if (value.name == "goal")
			{
				float x = std::stof(value.info.at(0));
				float y = std::stof(value.info.at(1));
				float z = std::stof(value.info.at(2));
				AddGameObject<CGoal>(1)->SetPosition(D3DXVECTOR3(x, y, z));
			}
		}
	}

	//Aim polygon
	CPolygon* aimPolygon = AddGameObject<CPolygon>(2);
	aimPolygon->SetTag("Aim");
	aimPolygon->SetFile(L"./asset/texture/aim.png");

	//Guide polygon
	CPolygon* guidePolygon = AddGameObject<CPolygon>(2);
	guidePolygon->Init(100, 100, 256, 256);
	guidePolygon->SetTag("Guide01");
	guidePolygon->SetFile(L"./asset/texture/Guide01.png");

	//SkyBox
	AddGameObject<CSkyBox>(1);

	//Fade初始化
	auto fadeOut = AddGameObject<CFadeEffect>(1);
	fadeOut->SetMode(1);
	fadeOut->SetColor(D3DXVECTOR3(0.53f, 0.81f, 0.98f));

	//Achievement
	AddGameObject<CAchievementManager>(0);

	//Audio
	CManager::GetAudio()->Loop(5);
}

void CGame::Update()
{
	CScene::Update();

	auto cameraPos = GetGameObject<CFirstPersonCamera>()->GetPosition();
	if (cameraPos.x >= 1 || cameraPos.x <= -1 || cameraPos.z >= -5 || cameraPos.z <= -7)
	{
		if (GetGameObjectWithTag("Guide01") != nullptr)
		{
			((CPolygon*)GetGameObjectWithTag("Guide01"))->SetFile(L"./asset/texture/Guide02.png");
		}
	}

	//画面遷移
	auto goalPos = GetGameObject<CGoal>()->GetPosition();
	if (D3DXVec3Length(&(cameraPos - goalPos)) <= 1.0f)
	{
		CManager::GetAudio()->PlayOnce(6);
		CManager::SetScene<CGame02>();
		return;
	}
	if (cameraPos.y <= -10)
	{
		CManager::GetAudio()->PlayOnce(9);
		CManager::SetScene<CGame>();
		return;
	}
	if (Keyboard_IsPress(DIK_F1))
	{
		CManager::SetScene<CGame>();
		return;
	}
	if (Keyboard_IsPress(DIK_F2))
	{
		CManager::SetScene<CGame02>();
		return;
	}
}

void CGame::Uninit()
{
	delete m_Audio;
	CScene::Uninit();
	/// <summary>
	/// Unload resource
	/// </summary>

	CCube::Unload();
	CBullet::Unload();
	CExplosion::Unload();
	CGroundCube::Unload();
}