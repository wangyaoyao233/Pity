#pragma once
#include "main.h"
class CGameObject
{
public:
	CGameObject() {}
	virtual ~CGameObject() {}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	/// <summary>
	/// getter
	/// </summary>
	/// <returns></returns>
	D3DXVECTOR3 GetPosition() const { return m_Position; }
	D3DXVECTOR3 GetRotation() const { return m_Rotation; }
	D3DXVECTOR3 GetScale() const { return m_Scale; }
	int GetPriority() const { return m_Priority; }
	std::string GetTag() const { return m_Tag; }
	bool GetActive() const { return m_IsActive; }
	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	/// <summary>
	/// setter
	/// </summary>
	/// <param name="value"></param>
	void SetPosition(D3DXVECTOR3 value) { m_Position = value; }
	void SetRotation(D3DXVECTOR3 value) { m_Rotation = value; }
	void SetScale(D3DXVECTOR3 value) { m_Scale = value; }
	virtual void SetDestroy() { m_Destroy = true; }
	void SetTag(std::string value) { m_Tag = value; }
	void SetActive(bool value) { m_IsActive = value; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

protected:
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR3 m_Scale;

	bool m_Destroy = false;
	int m_Priority = 0;
	bool m_IsActive = true;

	std::string m_Tag;
};
