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
#include "Game06.h"
/*gameobjects header*/
#include "Player.h"
#include "Polygon.h"
#include "Bullet.h"
#include "Cube.h"
#include "FirstPersonCamera.h"
#include "Explosion.h"
#include "SkyBox.h"
#include "Line.h"
#include "GroundCube.h"
#include "Goal.h"
#include "MoveCube.h"
#include "FadeEffect.h"
#include "AchievementManager.h"
/*self header*/
#include "Game05.h"

bool g_IsFirstTime;
void CGame05::Init()
{
	this->m_SceneId = 5;

	CCube::Load();
	CBullet::Load();
	CExplosion::Load();
	CGroundCube::Load();

	AddGameObject<CFirstPersonCamera>(0);
	AddGameObject<CCamera>(0);

	{//ground
		std::string filename = "./asset/map/map5.map";
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
				auto goal = AddGameObject<CGoal>(1);
				goal->SetPosition(D3DXVECTOR3(x, y, z));
				goal->SetActive(false);
			}
			if (value.name == "movecube")
			{
				float x = std::stof(value.info.at(0));
				float y = std::stof(value.info.at(1));
				float z = std::stof(value.info.at(2));
				auto moveCube = AddGameObject<CMoveCube>(1);
				moveCube->SetPosition(D3DXVECTOR3(x, y, z));
				moveCube->SetStartPos(D3DXVECTOR3(x, y, z));
				x = std::stof(value.info.at(3));
				y = std::stof(value.info.at(4));
				z = std::stof(value.info.at(5));
				moveCube->SetEndPos(D3DXVECTOR3(x, y, z));
			}
		}
	}

	//Aim polygon
	CPolygon* aimPolygon = AddGameObject<CPolygon>(2);
	aimPolygon->SetTag("Aim");
	aimPolygon->SetFile(L"./asset/texture/aim.png");

	//SkyBox
	AddGameObject<CSkyBox>(1);
	//Fade初始化
	auto fadeOut = AddGameObject<CFadeEffect>(1);
	fadeOut->SetMode(1);
	fadeOut->SetColor(D3DXVECTOR3(0.53f, 0.81f, 0.98f));

	//Achievement
	AddGameObject<CAchievementManager>(0);

	g_IsFirstTime = true;
}

void CGame05::Update()
{
	CScene::Update();

	auto cameraPos = GetGameObject<CFirstPersonCamera>()->GetPosition();
	if (cameraPos.y <= -10)
	{
		CManager::GetAudio()->PlayOnce(9);
		CManager::SetScene<CGame05>();
		return;
	}

	//到达指定高度
	if (cameraPos.y >= 15 && g_IsFirstTime)
	{
		GetGameObject<CGoal>()->SetActive(true);
		g_IsFirstTime = false;
	}

	//画面遷移
	auto goalPos = GetGameObject<CGoal>()->GetPosition();
	if (D3DXVec3Length(&(cameraPos - goalPos)) <= 1.0f && GetGameObject<CGoal>()->GetActive())
	{
		CManager::GetAudio()->PlayOnce(6);
		CManager::SetScene<CGame06>();
		return;
	}
	if (Keyboard_IsPress(DIK_F1))
	{
		CManager::SetScene<CGame05>();
		return;
	}
	if (Keyboard_IsPress(DIK_F2))
	{
		CManager::SetScene<CGame06>();
		return;
	}
}

void CGame05::Uninit()
{
	delete m_Audio;
	CScene::Uninit();

	CCube::Unload();
	CBullet::Unload();
	CExplosion::Unload();
	CGroundCube::Unload();
}