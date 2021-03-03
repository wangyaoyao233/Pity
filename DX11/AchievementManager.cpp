/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/
#include "GameTimer.h"
#include "Mathf.h"
#include "input.h"
#include "Audio.h"
/*scenes header*/

/*gameobjects header*/
#include "AchievementEffect.h"
#include "Enemy.h"
#include "Goal.h"
/*self header*/
#include "AchievementManager.h"

bool CAchievementManager::isFistPlay = false;
bool CAchievementManager::isGoodJob = false;
bool CAchievementManager::isDefeatEnemy = false;
bool CAchievementManager::isClearAll = false;

void CAchievementManager::Init()
{
}

void CAchievementManager::Uninit()
{
	delete m_Audio;
}

void CAchievementManager::Update()
{
	auto scene = CManager::GetScene();
	//什么时候生成对应成就
	if (!isFistPlay)
	{
		auto achievement = scene->AddGameObject<CAchievementEffect>(2);
		achievement->Init(800, 630, 128, 128);
		achievement->SetFile(L"./asset/texture/firstplay.png");
		isFistPlay = true;
		CManager::GetAudio()->PlayOnce(7);
	}
	if (!isGoodJob && scene->m_SceneId == 5 && scene->GetGameObject<CGoal>()->GetActive())
	{
		auto achievement = scene->AddGameObject<CAchievementEffect>(2);
		achievement->Init(800, 630, 128, 128);
		achievement->SetFile(L"./asset/texture/goodjob.png");
		isGoodJob = true;
		CManager::GetAudio()->PlayOnce(7);
	}

	if (!isDefeatEnemy && scene->m_SceneId == 6 && scene->GetGameObject<CEnemy>() == nullptr)
	{
		auto achievement = scene->AddGameObject<CAchievementEffect>(2);
		achievement->Init(800, 630, 128, 128);
		achievement->SetFile(L"./asset/texture/defeatenemy.png");
		isDefeatEnemy = true;
		CManager::GetAudio()->PlayOnce(7);
	}
	if (!isClearAll && scene->m_SceneId == 8 && scene->GetGameObject<CGoal>()->GetActive())
	{
		auto achievement = scene->AddGameObject<CAchievementEffect>(2);
		achievement->Init(800, 630, 128, 128);
		achievement->SetFile(L"./asset/texture/clearall.png");
		isClearAll = true;
		CManager::GetAudio()->PlayOnce(7);
	}
}

void CAchievementManager::Draw()
{
}