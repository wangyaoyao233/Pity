#include "main.h"
#include "CCollisionHelper.h"

bool CCollisionHelper::IsCollision(D3DXVECTOR3 a, D3DXVECTOR3 size_a, D3DXVECTOR3 b, D3DXVECTOR3 size_b)
{
	bool x = fabsf(a.x - b.x) <= (size_a.x + size_b.x) * 0.5f;
	bool y = fabsf(a.y - b.y) <= (size_a.y + size_b.y) * 0.5f;
	bool z = fabsf(a.z - b.z) <= (size_a.z + size_b.z) * 0.5f;

	if (x && y && z)
	{
		return true;
	}
	return false;
}