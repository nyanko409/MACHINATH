#include "boss.h"
#include "map.h"


void Boss::Draw()
{
	auto pDevice = MyDirect3D_GetDevice();
	
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	MeshObject::Draw();
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}



Boss* g_boss;


void InitBoss()
{
	Map* map = GetMap()->back();
	Transform trans;

	if(map->exit == Direction::NORTH)
		trans = Transform({ 0 ,50, -200 });
	else if (map->exit == Direction::SOUTH)
		trans = Transform({ 0 ,50, 200 });
	else if(map->exit == Direction::WEST)
		trans = Transform({ 200 ,50, 0 });
	else
		trans = Transform({ -200 ,50, 0 });

	g_boss = new Boss(trans, MESH_BOSS, SHADER_DEFAULT, map);
	g_boss->enableDraw = false;
}

void UninitBoss()
{
	SAFE_DELETE(g_boss);
}

void UpdateBoss()
{
	// draw boss if map is active
	if (g_boss->parent->enableDraw)
		g_boss->enableDraw = true;
}
