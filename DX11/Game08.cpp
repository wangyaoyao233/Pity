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
#include "Result.h"
/*gameobjects header*/
#include "Player.h"
#include "Polygon.h"
#include "Bullet.h"
#include "Cube.h"
#include "FirstPersonCamera.h"
#include "Explosion.h"
#include "SkyBox.h"
#include "GroundCube.h"
#include "Goal.h"
#include "MoveCube.h"
#include "FadeEffect.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Boss01.h"
#include "NumberTexture.h"
#include "AchievementManager.h"
#include "HPTexture.h"
/*self header*/
#include "Game08.h"

void CGame08::Init()
{
	this->m_SceneId = 8;

	CCube::Load();
	CBullet::Load();
	CExplosion::Load();
	CGroundCube::Load();
	CEnemy::Load();
	CEnemyBullet::Load();

	AddGameObject<CFirstPersonCamera>(0);
	AddGameObject<CCamera>(0);

	{//ground
		std::string filename = "./asset/map/map8.map";
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
			if (value.name == "enemy")
			{
				float x = std::stof(value.info.at(0));
				float y = std::stof(value.info.at(1));
				float z = std::stof(value.info.at(2));
				auto enemy = AddGameObject<CEnemy>(1);
				enemy->SetPosition(D3DXVECTOR3(x, y, z));
				float angleMin = std::stof(value.info.at(3));
				float angleMax = std::stof(value.info.at(4));
				enemy->SetShootAngle(D3DXVECTOR2(angleMin, angleMax));
			}
			if (value.name == "boss")
			{
				float x = std::stof(value.info.at(0));
				float y = std::stof(value.info.at(1));
				float z = std::stof(value.info.at(2));
				auto boss = AddGameObject<CBoss>(1);
				boss->SetPosition(D3DXVECTOR3(x, y, z));
			}
		}
	}

	//Aim polygon
	CPolygon* aimPolygon = AddGameObject<CPolygon>(2);
	aimPolygon->SetTag("Aim");
	aimPolygon->SetFile(L"./asset/texture/aim.png");

	//game8 info
	CPolygon* info = AddGameObject<CPolygon>(2);
	info->Init(100, 100, 256, 256);
	info->SetTag("Info");
	info->SetFile(L"./asset/texture/game8info.png");
	info->SetActive(false);

	//SkyBox
	AddGameObject<CSkyBox>(1);
	//Fade初始化
	auto fadeOut = AddGameObject<CFadeEffect>(1);
	fadeOut->SetMode(1);
	fadeOut->SetColor(D3DXVECTOR3(0.53f, 0.81f, 0.98f));

	//number
	//auto number = AddGameObject<CNumberTexture>(2);
	//number->Init(50, 50, 32, 32);
	//number->SetNumber(CBoss::m_HP);
	auto bossHP = AddGameObject<CHPTexture>(2);
	bossHP->Init(30, 50, 1024, 32);
	bossHP->SetNumber(CBoss::m_HP);

	//Achievement
	AddGameObject<CAchievementManager>(0);
}

void CGame08::Update()
{
	CScene::Update();
	auto cameraPos = GetGameObject<CFirstPersonCamera>()->GetPosition();
	if (cameraPos.y <= -10)
	{
		CManager::GetAudio()->PlayOnce(9);
		CManager::SetScene<CGame08>();
		return;
	}

	if (GetGameObject<CBoss>() == nullptr)
	{
		GetGameObject<CGoal>()->SetActive(true);
	}

	//Collision
	auto bullets = GetGameObjects<CBullet>();
	auto enemys = GetGameObjects<CEnemy>();
	auto enemyBullets = GetGameObjects<CEnemyBullet>();
	auto boss = GetGameObject<CBoss>();

	//有敌人则处于无敌
	if (enemys.size() != 0 && boss != nullptr)
	{
		GetGameObject<CBoss>()->SetIsUnBeatable(true);
	}
	else if (enemys.size() == 0 && boss != nullptr)
	{
		GetGameObject<CBoss>()->SetIsUnBeatable(false);
	}

	if (CBoss::m_HP % 10 == 0)
	{
		GetGameObjectWithTag("Info")->SetActive(true);
	}
	else
	{
		GetGameObjectWithTag("Info")->SetActive(false);
	}

	//bullet - boss
	for (auto bullet : bullets)
	{
		if (boss)
		{
			float distance = D3DXVec3Length(&(bullet->GetPosition() - boss->GetPosition()));
			if (distance <= 4.0f && !boss->GetIsUnBeatable())
			{
				CBoss::m_HP--;
				//GetGameObject<CNumberTexture>()->SetNumber(CBoss::m_HP);
				GetGameObject<CHPTexture>()->SetNumber(CBoss::m_HP);
				bullet->SetDestroy();
				CExplosion* explosion = AddGameObject<CExplosion>(1);
				explosion->SetPosition(bullet->GetPosition());
				break;
			}
		}
	}

	//bullet - enemy
	for (auto bullet : bullets)
	{
		for (auto enemy : enemys)
		{
			float distance = D3DXVec3Length(&(bullet->GetPosition() - enemy->GetPosition()));
			if (distance <= 1.3f)
			{
				enemy->SetDestroy();
				bullet->SetDestroy();
				//explosion
				CExplosion* explosion = AddGameObject<CExplosion>(1);
				explosion->SetPosition(bullet->GetPosition());
				break;
			}
		}
	}
	//boss - player

	if (boss)
	{
		float distance = D3DXVec3Length(&(cameraPos - boss->GetPosition()));
		if (distance <= 4.0f)
		{
			CManager::GetAudio()->PlayOnce(11);
			CManager::SetScene<CGame08>();
			return;
		}
	}

	//enemy - player
	for (auto enemy : enemys)
	{
		float distance = D3DXVec3Length(&(cameraPos - enemy->GetPosition()));
		if (distance <= 1.5f)
		{
			CManager::GetAudio()->PlayOnce(11);
			CManager::SetScene<CGame08>();
			return;
		}
	}
	//enemyBullet - player
	for (auto enemyBullet : enemyBullets)
	{
		float distance = D3DXVec3Length(&(cameraPos - enemyBullet->GetPosition()));
		if (distance <= 1.5f)
		{
			CManager::GetAudio()->PlayOnce(11);
			CManager::SetScene<CGame08>();
			return;
		}
	}

	//画面遷移
	auto goal = GetGameObject<CGoal>();
	auto goalPos = goal->GetPosition();
	if (D3DXVec3Length(&(cameraPos - goalPos)) <= 1.0f && goal->GetActive())
	{
		CManager::GetAudio()->PlayOnce(6);
		CManager::SetScene<CResult>();
		return;
	}
	if (Keyboard_IsPress(DIK_F1))
	{
		CManager::SetScene<CGame08>();
		return;
	}
	if (Keyboard_IsPress(DIK_F2))
	{
		CManager::SetScene<CResult>();
		return;
	}
}

void CGame08::Uninit()
{
	delete m_Audio;
	CScene::Uninit();

	CCube::Unload();
	CBullet::Unload();
	CExplosion::Unload();
	CGroundCube::Unload();
	CEnemy::Unload();
	CEnemyBullet::Unload();
}