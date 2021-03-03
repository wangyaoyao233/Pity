#pragma once

class CLight
{
public:
	CLight(D3DXVECTOR4 direction, D3DXCOLOR ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f), D3DXCOLOR diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	~CLight();

	void SetDirction(D3DXVECTOR4 direction);
	void SetAmbient(D3DXCOLOR color);
	void SetDiffuse(D3DXCOLOR color);
	void SetEnable(bool flag);
	void SetLight();

private:
	LIGHT* m_Light;
};