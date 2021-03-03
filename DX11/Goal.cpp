/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/
#include "GameTimer.h"
#include "Mathf.h"
/*gameobjects header*/
#include "Model.h"
/*scenes header*/
#include "Goal.h"

void CGoal::Init()
{
	m_Model = new CModel();
	m_Model->Load(L"asset\\model\\Goal.obj");

	m_Position = D3DXVECTOR3(0.0f, 1.0f, 1.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void CGoal::Uninit()
{
	m_Model->Unload();

	delete m_Model;
	m_Model = NULL;
}

void CGoal::Update()
{
}

void CGoal::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	//set word matrix
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
}