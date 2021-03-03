#include "main.h"
#include "renderer.h"
#include "Light.h"

CLight::CLight(D3DXVECTOR4 direction, D3DXCOLOR ambient, D3DXCOLOR diffuse)
{
	m_Light = new LIGHT();
	m_Light->Direction = direction;
	D3DXVec4Normalize(&m_Light->Direction, &m_Light->Direction);
	m_Light->Ambient = ambient;
	m_Light->Diffuse = diffuse;
	this->SetLight();
}

CLight::~CLight()
{
	if (m_Light)
	{
		delete m_Light;
	}
	m_Light = NULL;
}

void CLight::SetDirction(D3DXVECTOR4 direction)
{
	m_Light->Direction = direction;
	D3DXVec4Normalize(&m_Light->Direction, &m_Light->Direction);
	this->SetLight();
}

void CLight::SetAmbient(D3DXCOLOR color)
{
	m_Light->Ambient = color;
	this->SetLight();
}

void CLight::SetDiffuse(D3DXCOLOR color)
{
	m_Light->Diffuse = color;
	this->SetLight();
}

void CLight::SetEnable(bool flag)
{
	m_Light->Enable = flag;
	this->SetLight();
}

void CLight::SetLight()
{
	CRenderer::SetLight(*m_Light);
}