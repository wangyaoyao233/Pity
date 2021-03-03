#pragma once
#include "GameObject.h"
class CModel;
class CBullet : public CGameObject
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void Uninit();

public:
	static void Load();
	static void Unload();

protected:
	static CModel* m_Model;
private:
	D3DXVECTOR3 m_Direction;
	int m_Life;
};
