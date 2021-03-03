/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/
#include "GameTimer.h"
#include "Mathf.h"
#include "Audio.h"
/*gameobjects header*/
#include "Model.h"
#include "EnemyBullet.h"
#include "Enemy.h"
#include "FadeEffect.h"
/*scenes header*/
#include "Boss01.h"

int CBoss::m_HP = 55;

void CBoss::Shoot(float shootAngle)
{
	CScene* scene = CManager::GetScene();
	//发射
	auto enemyBullet = scene->AddGameObject<CEnemyBullet>(1);
	D3DXVECTOR3 pos = m_Position;
	pos.y -= 1;
	enemyBullet->SetPosition(pos);
	enemyBullet->SetDirection(D3DXVECTOR3(cosf(shootAngle), 0, sinf(shootAngle)));
}

void CBoss::Init()
{
	m_Model = new CModel();
	m_Model->Load(L"asset\\model\\boss.obj");

	m_Position = D3DXVECTOR3(0.0f, 1.0f, 1.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_Cnt = 0;
	m_HP = 55;
	m_IsUnBeatable = false;
	m_ShootTiming = 120;

	for (int i = 0; i < 6; i++)
	{
		m_FistTime[i] = true;
	}
}

void CBoss::Uninit()
{
	m_Model->Unload();

	delete m_Model;
	m_Model = NULL;
}

void CBoss::Update()
{
	if (m_HP <= 0)
	{
		SetDestroy();
	}

	//不同的FightMode
	//1. 大范围攻击
	if (m_Cnt > m_ShootTiming)
	{
		Shoot(Mathf::Random<float>(0.78f, 2.36f));
		Shoot(Mathf::Random<float>(0.78f, 2.36f));
		Shoot(Mathf::Random<float>(2.36f, 3.9f));
		Shoot(Mathf::Random<float>(2.36f, 3.9f));
		Shoot(Mathf::Random<float>(3.9f, 5.49f));
		Shoot(Mathf::Random<float>(3.9f, 5.49f));
		Shoot(Mathf::Random<float>(5.49f, 6.28f));
		Shoot(Mathf::Random<float>(0, 0.78f));

		//随机时间发射 3s = 180
		m_ShootTiming = Mathf::Random(80, 90);
		m_Cnt = 0;
	}

	m_Cnt++;
	//2. 生命值每到10的倍数, 生成小怪
	if (m_HP > 0 && (m_HP % 10 == 0) && m_FistTime[m_HP / 10 - 1])
	{
		CManager::GetAudio()->PlayOnce(8);

		m_FistTime[m_HP / 10 - 1] = false;

		auto scene = CManager::GetScene();

		auto fadeOut = scene->AddGameObject<CFadeEffect>(1);
		fadeOut->SetMode(1);
		fadeOut->SetColor(D3DXVECTOR3(1.0f, 0.0f, 0.0f));

		CEnemy* enemy = nullptr;
		enemy = scene->AddGameObject<CEnemy>(1);
		enemy->SetPosition(D3DXVECTOR3(-18, 3, -6));
		enemy->SetShootAngle(D3DXVECTOR2(-0.78f, 0.78f));

		enemy = scene->AddGameObject<CEnemy>(1);
		enemy->SetPosition(D3DXVECTOR3(18, 3, -6));
		enemy->SetShootAngle(D3DXVECTOR2(2.36f, 3.9f));

		enemy = scene->AddGameObject<CEnemy>(1);
		enemy->SetPosition(D3DXVECTOR3(0, 3, 22));
		enemy->SetShootAngle(D3DXVECTOR2(3.9f, 5.49f));

		enemy = scene->AddGameObject<CEnemy>(1);
		enemy->SetPosition(D3DXVECTOR3(0, 3, -22));
		enemy->SetShootAngle(D3DXVECTOR2(0.78f, 2.36f));
	}
}

void CBoss::Draw()
{
	//set word matrix
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
}