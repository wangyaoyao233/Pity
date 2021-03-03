/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/
#include "GameTimer.h"
#include "Mathf.h"
/*scenes header*/

/*gameobjects header*/
#include "Model.h"
#include "FirstPersonCamera.h"
#include "Cube.h"
#include "Explosion.h"
#include "Line.h"
#include "MoveCube.h"
/*self header*/
#include "Bullet.h"

CModel* CBullet::m_Model = NULL;

void CBullet::Load()
{
	m_Model = new CModel();
	m_Model->Load(L"asset\\model\\pinkball.obj");
}

void CBullet::Unload()
{
	m_Model->Unload();

	delete m_Model;
	m_Model = NULL;
}

void CBullet::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_Direction = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Life = 0;
}

void CBullet::Uninit()
{
}

void CBullet::Update()
{
	m_Life++;
	//get scene
	CScene* scene = CManager::GetScene();

	//get move direction
	float moveSpeed = 60.0f * GameTimer::DeltaTime();
	if (m_Direction.x == 0 && m_Direction.y == 0 && m_Direction.z == 0)
	{
		m_Direction = scene->GetGameObject<CFirstPersonCamera>()->GetForward();
	}

	m_Position += m_Direction * moveSpeed;

	//0.25 seconds Destory
	if (m_Life > 10)
	{
		SetDestroy();
		return;
	}

	//collision
	for (CCube* cube : scene->GetGameObjects<CCube>())
	{
		D3DXVECTOR3 enemyPosition = cube->GetPosition();
		D3DXVECTOR3 dist = m_Position - enemyPosition;

		if (D3DXVec3Length(&dist) < 1.3f)
		{
			//cube->SetDestroy();
			if (cube->GetLine() == NULL)
			{
				CLine* line = scene->AddGameObject<CLine>(1);
				cube->SetLine(line);
				line->SetParent(cube);
			}
			this->SetDestroy();
			/// <summary>
			/// set explosion
			/// </summary>
			CExplosion* explosion = scene->AddGameObject<CExplosion>(1);
			explosion->SetPosition(m_Position);
			return;
		}
	}
	for (CMoveCube* moveCube : scene->GetGameObjects<CMoveCube>())
	{
		D3DXVECTOR3 enemyPosition = moveCube->GetPosition();
		D3DXVECTOR3 dist = m_Position - enemyPosition;

		if (D3DXVec3Length(&dist) < 1.3f)
		{
			if (moveCube->GetLine() == NULL)
			{
				CLine* line = scene->AddGameObject<CLine>(1);
				moveCube->SetLine(line);
				line->SetParent(moveCube);
			}
			this->SetDestroy();
			/// <summary>
			/// set explosion
			/// </summary>
			CExplosion* explosion = scene->AddGameObject<CExplosion>(1);
			explosion->SetPosition(m_Position);
			return;
		}
	}
}

void CBullet::Draw()
{
	//set word matrix
	//D3DXMATRIX
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
}