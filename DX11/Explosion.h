#pragma once
#include "GameObject.h"
class CExplosion :public CGameObject
{
public:
	virtual void Init();
	virtual void Draw();
	virtual void Update();
	virtual void Uninit();

public:
	static void Load();
	static void Unload();

private:
	static ID3D11ShaderResourceView* m_Texture;
	static ID3D11Buffer* m_VertexBuffer;
private:
	int m_Cnt = 0;

private:
	//之前的方法需要 bd, sd
	void SetUV(float x, float y, float u, float v);
	void Update(float u, float v, int counts, int speed);
};
