/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/
#include "GameTimer.h"
#include "Mathf.h"
/*gameobjects header*/
#include "Model.h"
#include "EnemyBullet.h"
/*scenes header*/
#include "Enemy.h"

CModel* CEnemy::m_Model = NULL;

void CEnemy::Load()
{
	m_Model = new CModel();
	m_Model->Load(L"asset\\model\\enemy.obj");
}

void CEnemy::Unload()
{
	m_Model->Unload();

	delete m_Model;
	m_Model = NULL;
}
void CEnemy::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 1.0f, 1.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_Cnt = 0;
	m_ShootTiming = 0;
}

void CEnemy::Uninit()
{
}

void CEnemy::Update()
{
	if (m_Cnt > m_ShootTiming)
	{
		CScene* scene = CManager::GetScene();
		//发射
		auto enemyBullet = scene->AddGameObject<CEnemyBullet>(1);
		float shootAngle = Mathf::Random<float>(m_ShootAngle.x, m_ShootAngle.y);
		enemyBullet->SetPosition(this->m_Position);
		enemyBullet->SetDirection(D3DXVECTOR3(cosf(shootAngle), 0, sinf(shootAngle)));
		//随机时间发射 3s = 180
		m_ShootTiming = Mathf::Random(30, 60);
		m_Cnt = 0;
	}

	m_Cnt++;
}

void CEnemy::Draw()
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