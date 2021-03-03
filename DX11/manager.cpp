/*system header*/
#include "main.h"
#include "renderer.h"
/*tools header*/
#include "Mouse.h"
#include "input.h"
#include "DirectWrite.h"
#include "GameTimer.h"
#include "Audio.h"
/*scenes header*/
#include "Scene.h"
#include "Title.h"
#include "Game.h"
#include "Result.h"
#include "Game08.h"
/*gameobjects header*/

/*self header*/
#include "manager.h"

CScene* CManager::m_Scene = NULL;
CAudio* CManager::m_Audio = nullptr;

//CDirectWrite* g_Text = NULL;

void CManager::Init()
{
	CRenderer::Init();

	{
		//Init mouse
		CMouse::Get().SetWindow(GetWindow());
		CMouse::Get().SetMode(CMouse::MODE_RELATIVE);
	}

	//Audio
	m_Audio = new CAudio();
	m_Audio->Init();
	m_Audio->LoadFile(L"asset\\audio\\title.wav", 0);//title
	m_Audio->LoadFile(L"asset\\audio\\confirm.wav", 1);//confirm
	m_Audio->LoadFile(L"asset\\audio\\shot.wav", 2);//shoot
	m_Audio->LoadFile(L"asset\\audio\\exchange.wav", 3);//exchange
	m_Audio->LoadFile(L"asset\\audio\\jump.wav", 4);//jump
	m_Audio->LoadFile(L"asset\\audio\\BGM.wav", 5);//game bgm
	m_Audio->LoadFile(L"asset\\audio\\GameClear.wav", 6);//game clear
	m_Audio->LoadFile(L"asset\\audio\\found.wav", 7);//achievement
	m_Audio->LoadFile(L"asset\\audio\\alert.wav", 8);//alert
	m_Audio->LoadFile(L"asset\\audio\\death.wav", 9);//death
	m_Audio->LoadFile(L"asset\\audio\\result.wav", 10);//result
	m_Audio->LoadFile(L"asset\\audio\\death3.wav", 11);//death3

	SetScene<CTitle>();

	//g_Text = new CDirectWrite();
	//g_Text->CreateDeviceIndependentResources();
}

void CManager::Uninit()
{
	m_Scene->Uninit();

	delete m_Scene;
	delete m_Audio;

	//g_Text->DiscardDeviceResources();
	//delete g_Text;

	CRenderer::Uninit();
}

void CManager::Update()
{
	m_Scene->Update();
}

void CManager::Draw()
{
	CRenderer::Begin();

	m_Scene->Draw();

	CRenderer::End();
}

/*set light*/

//LIGHT light;
//light.Enable = true;
//light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
//D3DXVec4Normalize(&light.Direction, &light.Direction);
//light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
//light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//CRenderer::SetLight(light);

//3d

//light off
//light.Enable = false;
//CRenderer::SetLight(light);

//2d