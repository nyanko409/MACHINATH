#include "customMath.h"

float GetDistance(D3DXVECTOR3 t1, D3DXVECTOR3 t2, bool ignoreYAxis)
{
	D3DXVECTOR3 dist = t1 - t2;

	if(!ignoreYAxis)
		return sqrtf(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
	else
		return sqrtf(dist.x * dist.x + dist.z * dist.z);
}
