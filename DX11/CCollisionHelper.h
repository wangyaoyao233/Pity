#pragma once
class CCollisionHelper
{
public:
	//a,b 为中心点位置, 即m_Position
	//size_a, size_b 为各个边长
	static bool IsCollision(D3DXVECTOR3 a, D3DXVECTOR3 size_a, D3DXVECTOR3 b, D3DXVECTOR3 size_b);
};
