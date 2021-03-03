#pragma once
#include"GameObject.h"
class CPolygonAnimation : public CGameObject
{
public:
	void Init();
	void Update();
	void Draw();
	void Uninit();

	void Init(float offset_x, float offset_y, float size_x, float size_y, int speed, int total_pictures);
	void SetFile(const WCHAR* filename);
	void SetUV(float u, float v) { m_Uvalue = u; m_Vvalue = v; }

private:
	ID3D11ShaderResourceView* m_Texture = NULL;
	ID3D11Buffer* m_VertexBuffer = NULL;
	int m_Cnt = 0;
	int m_Speed;
	int m_TotalPictures;
	float m_Uvalue;
	float m_Vvalue;

	float m_PX;//起始点x坐标
	float m_PY;//起始点y坐标
	float m_SX;//图片大小x
	float m_SY;//图片大小y

private:
	//Obsolete
	/// <summary>
	/// 重载Update函数
	/// </summary>
	/// <param name="u"></param>
	/// <param name="v"></param>
	/// <param name="counts">一共播放的张数</param>
	/// <param name="speed">播放速度, 值越大越慢</param>
	void Update(float u, float v, int counts, int speed);
	void SetUV(float x, float y, float u, float v);
};
