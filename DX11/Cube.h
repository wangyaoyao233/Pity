#pragma once
#include "GameObject.h"
class CModel;
class CLine;
class CCube : public CGameObject
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void Uninit();

public:
	static void Load();
	static void Unload();

public:
	CLine* GetLine() { return m_Line; }
	void SetLine(CLine* line) { m_Line = line; }

protected:
	static CModel* m_Model;
	CLine* m_Line = NULL;//设置子物体line
};
