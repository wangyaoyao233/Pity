#pragma once

#include "GameObject.h"
#include "Mouse.h"
class CModel;
class CBullet;

class CPlayer :public CGameObject
{
public:

	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void Uninit();

private:
	CModel* m_Model;
	//CMouse::State m_lastMouseState;
	//CMouse::ButtonStateTracker m_MouseTracker;

	D3DXQUATERNION m_Quaternion;

	void memo01();
};