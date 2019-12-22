#include "collider.h"


bool BoxCollider::CheckCollision(const BoxCollider& col1, const BoxCollider& col2)
{
	// get size
	v3t_float size1 = col1.GetSize();
	v3t_float size2 = col2.GetSize();

	// get current top left position
	v3t_float topLeft1 = col1.isStatic ? col1.m_topLeft : col1.GetTopLeft();
	v3t_float topLeft2 = col2.isStatic ? col2.m_topLeft : col2.GetTopLeft();

	// check collision
	if (topLeft1.x < topLeft2.x + size2.x && topLeft1.x + size1.x > topLeft2.x)
		if (topLeft1.z < topLeft2.z + size2.z && topLeft1.z + size1.z > topLeft2.z)
			if (topLeft1.y > topLeft2.y - size2.y && topLeft1.y - size1.y < topLeft2.y)
				return true;

	// no collision occured
	return false;
}
