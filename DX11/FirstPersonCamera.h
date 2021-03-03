#pragma once
#include "Camera.h"
#include "Mouse.h"
class CPlayer;
class CAudio;
class CFirstPersonCamera : public CCamera
{
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;
	D3DXVECTOR3 GetForward() const { return m_Forward; }
	D3DXVECTOR3 GetRight() const { return m_Right; }
	D3DXVECTOR3 GetUp() const { return m_Up; }

private:
	void Walk(float dist);
	void Strafe(float dist);
	void Pitch(float angle);//up down
	void RotateY(float angle);//left right

private:
	D3DXVECTOR3 m_Forward;
	D3DXVECTOR3 m_Right;
	D3DXVECTOR3 m_Up;
	//Mouse
	CMouse::State m_lastMouseState;
	CMouse::ButtonStateTracker m_MouseTracker;

	bool m_OnExchange = false;
	bool m_OnGround = false;

	CPlayer* m_Player = NULL;//设置子物体player

	CAudio* m_Audio;
};
