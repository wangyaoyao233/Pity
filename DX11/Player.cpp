/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/
#include "GameTimer.h"
#include "Mathf.h"
#include "input.h"
/*scenes header*/

/*gameobjects header*/
#include "FirstPersonCamera.h"
#include "Model.h"
/*self header*/
#include "Player.h"

void CPlayer::Init()
{
	m_Model = new CModel();
	m_Model->Load(L"asset\\model\\monkey.obj");

	m_Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	D3DXQuaternionIdentity(&m_Quaternion);
}

void CPlayer::Uninit()
{
	m_Model->Unload();

	delete m_Model;
	m_Model = NULL;
}

void CPlayer::Update()
{
	if (Keyboard_IsPress(DIK_LEFT)) {
		//m_Position.x -= 0.1f;
		m_Rotation.y -= 0.05f;
	}
	if (Keyboard_IsPress(DIK_RIGHT)) {
		//m_Position.x += 0.1f;
		m_Rotation.y += 0.05f;
	}
	D3DXVECTOR3 forward = GetForward();
	if (Keyboard_IsPress(DIK_UP)) {
		//m_Position.z += 0.1f;
		m_Position += forward * 0.1f;
	}
	if (Keyboard_IsPress(DIK_DOWN)) {
		//m_Position.z -= 0.1f;
		m_Position -= forward * 0.1f;
	}

	//{
	//	//1. 获取当前鼠标状态
	//	CMouse::State mouseState = CMouse::Get().GetState();
	//	//2. 使用当前状态更新
	//	m_MouseTracker.Update(mouseState);
	//	//如果按下mouse, 则发射子弹
	//	//if (mouseState.leftButton == true && m_MouseTracker.leftButton == m_MouseTracker.PRESSED)
	//	//{
	//	//	CScene* scene = CManager::GetScene();
	//	//	scene->AddGameObject<CBullet>()->SetPosition(m_Position);
	//	//}
	//	//3. 获取上一帧鼠标事件, 应当在Update之前使用, 否则变为获取当前帧状态
	//	m_lastMouseState = m_MouseTracker.GetLastState();
	//}
}

void CPlayer::Draw()
{
	bool flag = CManager::GetScene()->GetGameObject<CFirstPersonCamera>()->GetSee();
	if (flag)
	{
		return;
	}

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

void CPlayer::memo01()
{
	if (Keyboard_IsPress(DIK_LEFT)) {
		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &D3DXVECTOR3(0, 0, 1), 0.1f);
		m_Quaternion *= quat;
	}
	if (Keyboard_IsPress(DIK_RIGHT)) {
		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &D3DXVECTOR3(0, 0, 1), -0.1f);
		m_Quaternion *= quat;
	}
	if (Keyboard_IsPress(DIK_UP)) {
		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &D3DXVECTOR3(1, 0, 0), 0.1f);
		m_Quaternion *= quat;
	}
	if (Keyboard_IsPress(DIK_DOWN)) {
		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &D3DXVECTOR3(1, 0, 0), -0.1f);
		m_Quaternion *= quat;
	}
	D3DXMATRIX rot;
	D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);

	D3DXQuaternionNormalize(&m_Quaternion, &m_Quaternion);
}