#pragma once
#include "Cube.h"
class CMoveCube :public CCube
{
public:
	void Update() override;

	void SetStartPos(D3DXVECTOR3 pos) { m_StartPostion = pos; }
	void SetEndPos(D3DXVECTOR3 pos) { m_EndPostion = m_StartPostion + pos; }

private:
	D3DXVECTOR3 m_StartPostion;
	D3DXVECTOR3 m_EndPostion;
	int m_Mode = 0;
};
