#include "boss.h"
#include "player.h"
#include "effect.h"
#include "map.h"


void Boss::Draw()
{
	if (!enableDraw) return;

#ifdef _DEBUG
	BoxCollider::DrawCollider(collider, D3DCOLOR(D3DCOLOR_RGBA(255, 0, 0, 255)));
#endif

	// draw both faces for boss
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
	// return if boss is already dead
	if (!g_boss) return;

	// draw boss if map is active
	if (!g_boss->enableDraw && g_boss->parent->enableDraw)
		g_boss->enableDraw = true;

	// if collided with player, delete object and play effects
	if (g_boss->collider.CheckCollision(GetPlayer()->col))
	{
		PlayEffect(EFFECT_EXPLOSION_RED, g_boss->GetCombinedPosition(), { 0, 0, 0 }, { 10, 10, 10 }, 1.0F);
		SAFE_DELETE(g_boss);
	}
}
