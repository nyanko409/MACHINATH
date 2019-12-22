#include "common.h"
#include "collider.h"

// color of the collider
#define COL_COLOR D3DCOLOR(D3DCOLOR_RGBA(0, 0, 0, 255))

// fvf for linestrip
#define FVF_LINE (D3DFVF_XYZ | D3DFVF_DIFFUSE)

// vertices for linestrip
struct LINE_VERTEX
{
	v3t_float pos;
	D3DCOLOR color;
};


bool BoxCollider::CheckCollision(const BoxCollider& col1, const BoxCollider& col2)
{
	// get size
	v3t_float size1 = col1.GetSize();
	v3t_float size2 = col2.GetSize();

	// get current top left position
	v3t_float topLeft1 = col1.isStatic ? col1.m_topLeft : col1.GetTopLeft();
	v3t_float topLeft2 = col2.isStatic ? col2.m_topLeft : col2.GetTopLeft();

	// check collision
	if ((topLeft1.x < topLeft2.x + size2.x && topLeft1.x + size1.x > topLeft2.x)
		&& (topLeft1.z < topLeft2.z + size2.z && topLeft1.z + size1.z > topLeft2.z)
			&& (topLeft1.y > topLeft2.y - size2.y && topLeft1.y - size1.y < topLeft2.y))
				return true;

	// no collision occured
	return false;
}


void BoxCollider::DrawCollider(const BoxCollider& col)
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
		{topLeft, COL_COLOR}, {v3t_float{topLeft.x + size.x, topLeft.y, topLeft.z}, COL_COLOR},
		{topLeft, COL_COLOR}, {v3t_float{topLeft.x, topLeft.y - size.y, topLeft.z}, COL_COLOR},
		{v3t_float{topLeft.x + size.x, topLeft.y, topLeft.z}, COL_COLOR}, {v3t_float{topLeft.x + size.x, topLeft.y - size.y, topLeft.z}, COL_COLOR},
		{v3t_float{topLeft.x, topLeft.y - size.y, topLeft.z}, COL_COLOR}, {v3t_float{topLeft.x + size.x, topLeft.y - size.y, topLeft.z}, COL_COLOR},

		// front
		{v3t_float{topLeft.x, topLeft.y, topLeft.z + size.z}, COL_COLOR}, {v3t_float{topLeft.x + size.x, topLeft.y, topLeft.z + size.z}, COL_COLOR},
		{v3t_float{topLeft.x, topLeft.y, topLeft.z + size.z}, COL_COLOR}, {v3t_float{topLeft.x, topLeft.y - size.y, topLeft.z + size.z}, COL_COLOR},
		{v3t_float{topLeft.x + size.x, topLeft.y, topLeft.z + size.z}, COL_COLOR}, {v3t_float{topLeft.x + size.x, topLeft.y - size.y, topLeft.z + size.z}, COL_COLOR},
		{v3t_float{topLeft.x, topLeft.y - size.y, topLeft.z + size.z}, COL_COLOR}, {v3t_float{topLeft.x + size.x, topLeft.y - size.y, topLeft.z + size.z}, COL_COLOR},

		// center
		{v3t_float{topLeft.x, topLeft.y, topLeft.z}, COL_COLOR}, {v3t_float{topLeft.x, topLeft.y, topLeft.z + size.z}, COL_COLOR},
		{v3t_float{topLeft.x + size.x, topLeft.y, topLeft.z}, COL_COLOR}, {v3t_float{topLeft.x + size.x, topLeft.y, topLeft.z + size.z}, COL_COLOR},
		{v3t_float{topLeft.x, topLeft.y - size.y, topLeft.z}, COL_COLOR}, {v3t_float{topLeft.x, topLeft.y - size.y, topLeft.z + size.z}, COL_COLOR},
		{v3t_float{topLeft.x + size.x, topLeft.y - size.y, topLeft.z}, COL_COLOR}, {v3t_float{topLeft.x + size.x, topLeft.y - size.y, topLeft.z + size.z}, COL_COLOR}
	};

	device->SetFVF(FVF_LINE);
	device->DrawPrimitiveUP(D3DPT_LINELIST, 12, vert, sizeof(LINE_VERTEX));

	device->SetRenderState(D3DRS_LIGHTING, true);
}
