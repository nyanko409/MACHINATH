#include "common.h"
#include "collider.h"


// fvf for linestrip
#define FVF_LINE (D3DFVF_XYZ | D3DFVF_DIFFUSE)

// vertices for linestrip
struct LINE_VERTEX
{
	v3t_float pos;
	D3DCOLOR color;
};


int BoxCollider::CheckCollision(const BoxCollider& other, const bool nsFirst) const
{
	// get size
	v3t_float size1 = GetSize();
	v3t_float size2 = other.GetSize();

	// get current top left position
	v3t_float topLeft1 = isStatic ? m_topLeft : GetTopLeft();
	v3t_float topLeft2 = other.isStatic ? other.m_topLeft : other.GetTopLeft();

	// check collision
	if ((topLeft1.x < topLeft2.x + size2.x && topLeft1.x + size1.x > topLeft2.x)
		&& (topLeft1.z < topLeft2.z + size2.z && topLeft1.z + size1.z > topLeft2.z)
		&& (topLeft1.y > topLeft2.y - size2.y && topLeft1.y - size1.y < topLeft2.y))
	{
		if (!nsFirst)
		{
			// collision on right side
			if (topLeft1.x + size1.x > topLeft2.x && topLeft1.x < topLeft2.x)
				return 1;

			// collision on left side
			if (topLeft1.x < topLeft2.x + size2.x && topLeft1.x + size1.x > topLeft2.x + size2.x)
				return 2;

			// collision on top side
			if (topLeft1.z + size1.z > topLeft2.z && topLeft1.z < topLeft2.z)
				return 3;
			// collision on bottom side
			if (topLeft1.z < topLeft2.z + size2.z && topLeft1.z + size1.z > topLeft2.z + size2.z)
				return 4;
		}
		else
		{
			// collision on top side
			if (topLeft1.z + size1.z > topLeft2.z && topLeft1.z < topLeft2.z)
				return 3;
			// collision on bottom side
			if (topLeft1.z < topLeft2.z + size2.z && topLeft1.z + size1.z > topLeft2.z + size2.z)
				return 4;

			// collision on right side
			if (topLeft1.x + size1.x > topLeft2.x && topLeft1.x < topLeft2.x)
				return 1;

			// collision on left side
			if (topLeft1.x < topLeft2.x + size2.x && topLeft1.x + size1.x > topLeft2.x + size2.x)
				return 2;
		}
	}

	// no collision occured
	return 0;
}


void BoxCollider::DrawCollider(const BoxCollider& col, const D3DCOLOR& color)
{
	// init
	auto device = MyDirect3D_GetDevice();

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	device->SetTransform(D3DTS_WORLD, &matIdentity);
	device->SetTexture(0, NULL);
	device->SetRenderState(D3DRS_LIGHTING, false);

	// get size and current top left position
	v3t_float size = col.GetSize();
	v3t_float topLeft = col.isStatic ? col.m_topLeft : col.GetTopLeft();

	LINE_VERTEX vert[] =
	{
		// back
		{topLeft, color}, {v3t_float{topLeft.x + size.x, topLeft.y, topLeft.z}, color},
		{topLeft, color}, {v3t_float{topLeft.x, topLeft.y - size.y, topLeft.z}, color},
		{v3t_float{topLeft.x + size.x, topLeft.y, topLeft.z}, color}, {v3t_float{topLeft.x + size.x, topLeft.y - size.y, topLeft.z}, color},
		{v3t_float{topLeft.x, topLeft.y - size.y, topLeft.z}, color}, {v3t_float{topLeft.x + size.x, topLeft.y - size.y, topLeft.z}, color},

		// front
		{v3t_float{topLeft.x, topLeft.y, topLeft.z + size.z}, color}, {v3t_float{topLeft.x + size.x, topLeft.y, topLeft.z + size.z}, color},
		{v3t_float{topLeft.x, topLeft.y, topLeft.z + size.z}, color}, {v3t_float{topLeft.x, topLeft.y - size.y, topLeft.z + size.z}, color},
		{v3t_float{topLeft.x + size.x, topLeft.y, topLeft.z + size.z}, color}, {v3t_float{topLeft.x + size.x, topLeft.y - size.y, topLeft.z + size.z}, color},
		{v3t_float{topLeft.x, topLeft.y - size.y, topLeft.z + size.z}, color}, {v3t_float{topLeft.x + size.x, topLeft.y - size.y, topLeft.z + size.z}, color},

		// center
		{v3t_float{topLeft.x, topLeft.y, topLeft.z}, color}, {v3t_float{topLeft.x, topLeft.y, topLeft.z + size.z}, color},
		{v3t_float{topLeft.x + size.x, topLeft.y, topLeft.z}, color}, {v3t_float{topLeft.x + size.x, topLeft.y, topLeft.z + size.z}, color},
		{v3t_float{topLeft.x, topLeft.y - size.y, topLeft.z}, color}, {v3t_float{topLeft.x, topLeft.y - size.y, topLeft.z + size.z}, color},
		{v3t_float{topLeft.x + size.x, topLeft.y - size.y, topLeft.z}, color}, {v3t_float{topLeft.x + size.x, topLeft.y - size.y, topLeft.z + size.z}, color}
	};

	device->SetFVF(FVF_LINE);
	device->DrawPrimitiveUP(D3DPT_LINELIST, 12, vert, sizeof(LINE_VERTEX));

	device->SetRenderState(D3DRS_LIGHTING, true);
}
