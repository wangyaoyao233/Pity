/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/
#include "GameTimer.h"
#include "Mathf.h"
/*scenes header*/

/*gameobjects header*/

/*self header*/
#include "MoveCube.h"

void CMoveCube::Update()
{
	float speed = 0.01f;

	if (m_Mode == 0)
	{
		if (D3DXVec3Length(&(m_EndPostion - m_Position)) <= 0.1f)
		{
			m_Mode = 1;
			return;
		}
		D3DXVECTOR3 dir = m_EndPostion - m_StartPostion;
		m_Position += dir * speed;
	}
	else if (m_Mode == 1)
	{
		if (D3DXVec3Length(&(m_StartPostion - m_Position)) <= 0.1f)
		{
			m_Mode = 0;
			return;
		}
		D3DXVECTOR3 dir = m_StartPostion - m_EndPostion;
		m_Position += dir * speed;
	}
}