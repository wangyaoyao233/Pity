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
#include "Model.h"
#include "FirstPersonCamera.h"
#include "Player.h"
/*self header*/
#include "Camera.h"

void CCamera::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 3.0f, -6.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void CCamera::Uninit()
{
}

void CCamera::Update()
{
	//get scene
	CScene* scene = CManager::GetScene();
	CFirstPersonCamera* fir = scene->GetGameObject<CFirstPersonCamera>();
	//D3DXVECTOR3 distance = D3DXVECTOR3(10, 0, 0);
	//m_Position = Mathf::Lerp<D3DXVECTOR3>(m_Position, fir->GetPosition() + distance, 0.05f);
	//m_Target = m_Position + D3DXVECTOR3(-1, 0, 0);
	D3DXVECTOR3 distance = fir->GetRight() * 8;
	m_Position = Mathf::Lerp<D3DXVECTOR3>(m_Position, fir->GetPosition() + distance, 0.1f);
	m_Target = m_Position - fir->GetRight();

	if (Keyboard_IsPress(DIK_E))
	{
		D3DXVECTOR3 dir = fir->GetForward();
		m_Position += dir * 2;
		m_Target += dir * 2;
	}
}

void CCamera::Draw()
{
	if (!m_IsSee)
	{
		return;
	}
	//set view matrix
	D3DXMatrixLookAtLH(&m_viewMatrix, &m_Position, &m_Target, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	CRenderer::SetViewMatrix(&m_viewMatrix);

	//set projection matrix
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
	CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

void CCamera::Memo01()
{
	CPlayer* player = CManager::GetScene()->GetGameObject<CPlayer>();
	m_Target = player->GetPosition();
	//top view
	//m_Position = m_Target + D3DXVECTOR3(0, 20, -10);

	//サードパーソン　ビュー.　third person veiew
	D3DXVECTOR3 forward = player->GetForward();
	m_Position = m_Target - forward * 5.0f + D3DXVECTOR3(0, 3, 0);
}