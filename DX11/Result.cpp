/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/
#include "input.h"
#include "Audio.h"
/*scenes header*/
#include "Title.h"
/*gameobjects header*/
#include "Polygon.h"
#include "FadeEffect.h"
#include "AchievementManager.h"
/*self header*/
#include "Result.h"

void CResult::Init()
{
	this->m_SceneId = 9;

	CPolygon* polygon;
	polygon = AddGameObject<CPolygon>(2);
	polygon->Init(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	polygon->SetFile(L"./asset/texture/result1.png");

	polygon = AddGameObject<CPolygon>(2);
	polygon->Init(300, 30, 512, 512);
	polygon->SetFile(L"./asset/texture/result2.png");

	//achievement show
	if (CAchievementManager::isFistPlay)
	{
		polygon = AddGameObject<CPolygon>(2);
		polygon->Init(100, 500, 128, 128);
		polygon->SetFile(L"./asset/texture/firstplay.png");
	}
	if (CAchievementManager::isGoodJob)
	{
		polygon = AddGameObject<CPolygon>(2);
		polygon->Init(300, 500, 128, 128);
		polygon->SetFile(L"./asset/texture/goodjob.png");
	}
	if (CAchievementManager::isDefeatEnemy)
	{
		polygon = AddGameObject<CPolygon>(2);
		polygon->Init(500, 500, 128, 128);
		polygon->SetFile(L"./asset/texture/defeatenemy.png");
	}
	if (CAchievementManager::isClearAll)
	{
		polygon = AddGameObject<CPolygon>(2);
		polygon->Init(700, 500, 128, 128);
		polygon->SetFile(L"./asset/texture/clearall.png");
	}

	//Fade初始化
	auto fadeOut = AddGameObject<CFadeEffect>(1);
	fadeOut->SetMode(1);
	fadeOut->SetColor(D3DXVECTOR3(0.53f, 0.81f, 0.98f));

	CManager::GetAudio()->Stop(5);
	CManager::GetAudio()->Play(10);
}

void CResult::Update()
{
	CScene::Update();
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		CManager::GetAudio()->Stop(10);
		CManager::GetAudio()->PlayOnce(1);
		CManager::SetScene<CTitle>();
	}
}

void CResult::Uninit()
{
	CScene::Uninit();
}