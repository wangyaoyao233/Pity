/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/
#include "GameTimer.h"
#include "Mathf.h"
#include "Audio.h"
#include "input.h"
#include "CCollisionHelper.h"
/*scenes header*/

/*gameobjects header*/
#include "Bullet.h"
#include "Polygon.h"
#include "GroundCube.h"
#include "Cube.h"
#include "Player.h"
#include "Line.h"
#include "MoveCube.h"
/*self header*/
#include "FirstPersonCamera.h"

//global value
static float g_yAxisSpeed = 0;
static D3DXVECTOR3 g_EnemyPos;
static float g_Fovy = 1.0f;

//const value
/// <summary>
/// 鼠标敏感度
/// </summary>
const float kMouseSensitivity = 0.01f;
/// <summary>
/// 避免万向锁, pitch在[-89.0,89.0], yaw在[0,360.0]
/// </summary>
const float kMaxPitchAngle = 89.0f;

void CFirstPersonCamera::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 3.0f, -6.0f);
	m_Forward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	D3DXVec3Normalize(&m_Forward, &m_Forward);
	D3DXVec3Normalize(&m_Right, &m_Right);
	D3DXVec3Normalize(&m_Up, &m_Up);

	//m_Player Init
	m_Player = CManager::GetScene()->AddGameObject<CPlayer>(1);

	m_IsSee = true;
}

void CFirstPersonCamera::Uninit()
{
	m_Player = NULL;
	delete m_Audio;
}

void CFirstPersonCamera::Update()
{
	//Get scene
	CScene* scene = CManager::GetScene();

	//1. 获取当前鼠标状态
	CMouse::State mouseState = CMouse::Get().GetState();
	//2. 使用当前状态更新
	m_MouseTracker.Update(mouseState);
	//根据鼠标相对坐标, 旋转
	float yawAngle = mouseState.x * kMouseSensitivity;
	float pitchAngle = mouseState.y * kMouseSensitivity;
	//第三视角的正确角度获得
	m_Rotation.x += pitchAngle;
	m_Rotation.y += yawAngle;
	//保证角度在合理范围
	if (pitchAngle > kMaxPitchAngle)
	{
		pitchAngle = kMaxPitchAngle;
	}
	if (pitchAngle < -kMaxPitchAngle)
	{
		pitchAngle = -kMaxPitchAngle;
	}

	RotateY(yawAngle);
	Pitch(pitchAngle);

	//如果点击mouse左键, 则发射子弹
	if (mouseState.leftButton == true && m_MouseTracker.leftButton == m_MouseTracker.PRESSED)
	{
		CScene* scene = CManager::GetScene();
		scene->AddGameObject<CBullet>(1)->SetPosition(m_Position);
		CManager::GetAudio()->PlayOnce(2);
	}

	//如果释放mouse右键, 使用正常瞄准镜UI
	((CPolygon*)scene->GetGameObjectWithTag("Aim"))->SetFile(L"./asset/texture/aim.png");

	g_Fovy = Mathf::Lerp(g_Fovy, 1.0f, 0.1f);
	//如果按住mouse右键
	if (mouseState.rightButton == true && m_MouseTracker.rightButton == m_MouseTracker.HELD)
	{
		g_Fovy = Mathf::Lerp(g_Fovy, 0.125f, 0.15f);
		//使用倍镜UI
		((CPolygon*)scene->GetGameObjectWithTag("Aim"))->SetFile(L"./asset/texture/sight.png");
	}

	//3. 获取上一帧鼠标事件, 应当在Update之前使用, 否则变为获取当前帧状态
	m_lastMouseState = m_MouseTracker.GetLastState();

	float moveSpeed = 5.0f * GameTimer::DeltaTime();
	if (Keyboard_IsPress(DIK_LSHIFT))
	{
		moveSpeed *= 1.5f;
	}

	//Collision
	auto grounds = scene->GetGameObjects<CGroundCube>();
	for (auto ground : grounds)
	{
		D3DXVECTOR3 groundPosition = ground->GetPosition();
		D3DXVECTOR3 distance = m_Position - groundPosition;

		//OnGround
		//if (D3DXVec3Length(&distance) <= 2.1f)
		//{
		//	m_OnGround = true;
		//	//(Self scale + Ground scale)*0.5f    //会出现抖动
		//	//m_Position.y = groundPosition.y + (2.0f);
		//	break;
		//}
		if (CCollisionHelper::IsCollision(m_Position, 2 * m_Scale, ground->GetPosition(), 2 * ground->GetScale()))
		{
			m_OnGround = true;
			m_Position.y = ground->GetPosition().y + 2;
			break;
		}
		else
		{
			m_OnGround = false;
		}
	}

	//Move
	if (Keyboard_IsPress(DIK_W))
	{
		Walk(moveSpeed);
	}
	if (Keyboard_IsPress(DIK_S))
	{
		Walk(-moveSpeed);
	}
	if (Keyboard_IsPress(DIK_A))
	{
		Strafe(-moveSpeed);
	}
	if (Keyboard_IsPress(DIK_D))
	{
		Strafe(moveSpeed);
	}

	//Not on Ground && Not on exchange
	if (!m_OnGround && !m_OnExchange)
	{
		//Down speed
		g_yAxisSpeed -= 0.01f;
	}
	if (m_OnGround)
	{
		g_yAxisSpeed = 0;
	}
	//Jump
	if (Keyboard_IsTrigger(DIK_SPACE) && m_OnGround)
	{
		//m_Position.y += 0.2f;
		g_yAxisSpeed = 0.2f;
		CManager::GetAudio()->PlayOnce(4);
	}

	m_Position.y += g_yAxisSpeed;

	//Exchange
	if (Keyboard_IsTrigger(DIK_F))
	{
		for (auto cube : scene->GetGameObjects<CCube>())
		{
			if (cube->GetLine() != NULL)
			{
				CManager::GetAudio()->PlayOnce(3);
				g_EnemyPos = cube->GetPosition();
				cube->SetPosition(this->m_Position);
				m_OnExchange = true;
				cube->GetLine()->SetDestroy();
				cube->SetLine(NULL);
				break;
			}
		}
		for (auto moveCube : scene->GetGameObjects<CMoveCube>())
		{
			if (moveCube->GetLine() != NULL)
			{
				CManager::GetAudio()->PlayOnce(3);
				g_EnemyPos = moveCube->GetPosition();
				moveCube->SetPosition(this->m_Position);
				m_OnExchange = true;
				moveCube->GetLine()->SetDestroy();
				moveCube->SetLine(NULL);

				moveCube->SetDestroy();
				break;
			}
		}
	}

	if (m_OnExchange)
	{
		m_Position = Mathf::Lerp<D3DXVECTOR3>(m_Position, g_EnemyPos, 0.08f);

		D3DXVECTOR3 distance = m_Position - g_EnemyPos;

		//重力失效

		if (D3DXVec3Length(&distance) < 1.0f)
		{
			m_OnExchange = false;
		}
	}

	//切换相机
	if (Keyboard_IsTrigger(DIK_Q))
	{
		m_IsSee = !m_IsSee;//切换视角
		auto camera = scene->GetGameObject<CCamera>();
		camera->SetSee(!(camera->GetSee()));
	}

	//m_Player Update
	{
		m_Player->SetPosition(m_Position);
		m_Player->SetRotation(m_Rotation);
	}
}

void CFirstPersonCamera::Draw()
{
	if (!m_IsSee)
	{
		return;
	}

	D3DXVECTOR3 eye = m_Position - m_Forward * 1.0f;
	//set view matrix
	D3DXMatrixLookAtLH(&m_viewMatrix, &eye, &m_Position, &m_Up);
	CRenderer::SetViewMatrix(&m_viewMatrix);

	//set projection matrix
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, g_Fovy, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
	CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

void CFirstPersonCamera::Walk(float dist)
{
	D3DXVECTOR3 forward = m_Forward;
	forward.y = 0;
	m_Position += forward * dist;
}

void CFirstPersonCamera::Strafe(float dist)
{
	m_Position += m_Right * dist;
}

void CFirstPersonCamera::Pitch(float angle)
{
	//获取旋转矩阵
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationAxis(&mtxRotate, &m_Right, angle);
	//将旋转矩阵应用到m_Forward向量
	D3DXVec3TransformNormal(&m_Forward, &m_Forward, &mtxRotate);
	//方法1: 利用cross求m_Up向量
	D3DXVec3Cross(&m_Up, &m_Forward, &m_Right);

	D3DXVec3Normalize(&m_Right, &m_Right);
	D3DXVec3Normalize(&m_Forward, &m_Forward);
	D3DXVec3Normalize(&m_Up, &m_Up);
}

void CFirstPersonCamera::RotateY(float angle)
{
	//获取旋转矩阵
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationY(&mtxRotate, angle);
	//将旋转矩阵应用到m_Forward向量
	D3DXVec3TransformNormal(&m_Forward, &m_Forward, &mtxRotate);
	//将旋转矩阵应用到m_Right向量
	D3DXVec3TransformNormal(&m_Right, &m_Right, &mtxRotate);
	//方法1: 利用cross求m_Up向量
	D3DXVec3Cross(&m_Up, &m_Forward, &m_Right);

	D3DXVec3Normalize(&m_Right, &m_Right);
	D3DXVec3Normalize(&m_Forward, &m_Forward);
	D3DXVec3Normalize(&m_Up, &m_Up);
}