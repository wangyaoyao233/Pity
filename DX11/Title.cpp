/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/
#include "input.h"
#include "Audio.h"
/*scenes header*/
#include "Game.h"
/*gameobjects header*/
#include "Polygon.h"
#include "PolygonAnimation.h"
/*self header*/
#include "Title.h"

void CTitle::Init()
{
	this->m_SceneId = 0;

	CPolygon* logoPolygon = AddGameObject<CPolygon>(2);
	logoPolygon->Init(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	logoPolygon->SetFile(L"./asset/texture/logo3.png");

	CPolygonAnimation* pressEnter = AddGameObject<CPolygonAnimation>(2);
	pressEnter->Init(800, 500, 128, 128, 20, 2);
	pressEnter->SetFile(L"./asset/texture/PressSpace.png");
	pressEnter->SetUV(0.5f, 1.0f);

	//Audio
	CManager::GetAudio()->Loop(0);
}

void CTitle::Update()
{
	CScene::Update();
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		CManager::GetAudio()->Stop(0);
		CManager::GetAudio()->PlayOnce(1);
		CManager::SetScene<CGame>();
	}
}

void CTitle::Uninit()
{
	delete m_Audio;
}