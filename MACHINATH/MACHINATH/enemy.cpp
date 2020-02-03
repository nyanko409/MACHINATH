#include "enemy.h"
#include "map.h"
#include "mesh.h"


BoneObject* g_yellow;
BoneObject* g_blue;
BoneObject* g_red;
BoneObject* g_twotone;


void InitEnemy()
{
	auto mapArray = GetMap();

	// init enemies at map
	for (Map* map : *mapArray)
	{
		if (!g_yellow && map->data.name == MESH_MAP_GREEN_HIROBA)
		{
			g_yellow = new BoneObject(Transform(), A_MESH_ENEMY_YELLOW, SHADER_DEFAULT, map);
			g_yellow->transform.rotation.y = 180;
			g_yellow->transform.scale = {4, 4, 4};
			g_yellow->enableDraw = false;
			g_yellow->PlayAnimation(1);
			g_yellow->SetAnimationSpeed(0);
		}
		else if (!g_blue && map->data.name == MESH_MAP_BLUE_HIROBA)
		{
			g_blue = new BoneObject(Transform(), A_MESH_ENEMY_BLUE, SHADER_DEFAULT, map);
			g_blue->transform.rotation.y = 180;
			g_blue->transform.scale = { 4, 4, 4 };
			g_blue->enableDraw = false;
			g_blue->PlayAnimation(1);
			g_blue->SetAnimationSpeed(0);
		}
		else if (!g_red && map->data.name == MESH_MAP_RED_HIROBA)
		{
			g_red = new BoneObject(Transform(), A_MESH_ENEMY_RED, SHADER_DEFAULT, map);
			g_red->transform.rotation.y = 180;
			g_red->transform.scale = { 4, 4, 4 };
			g_red->enableDraw = false;
			g_red->PlayAnimation(1);
			g_red->SetAnimationSpeed(0.004);
		}
		else if (!g_twotone && map->data.name == MESH_MAP_TWOTONE_HIROBA)
		{
			g_twotone = new BoneObject(Transform(), A_MESH_ENEMY_TWOTONE, SHADER_DEFAULT, map);
			g_twotone->transform.rotation.y = 180;
			g_twotone->transform.scale = { 4, 4, 4 };
			g_twotone->enableDraw = false;
			g_twotone->PlayAnimation(1);
			g_twotone->SetAnimationSpeed(0.004);
		}
	}
}

void UninitEnemy()
{
	SAFE_DELETE(g_yellow);
	SAFE_DELETE(g_blue);
	SAFE_DELETE(g_red);
	SAFE_DELETE(g_twotone);
}

void UpdateEnemy()
{
	// draw enemy when map is near
	if (g_yellow && ((Map*)g_yellow->parent->enableDraw))
		g_yellow->enableDraw = true;
	if (g_blue && ((Map*)g_blue->parent->enableDraw))
		g_blue->enableDraw = true;
	if (g_red && ((Map*)g_red->parent->enableDraw))
		g_red->enableDraw = true;
	if (g_twotone && ((Map*)g_twotone->parent->enableDraw))
		g_twotone->enableDraw = true;

	// delete enemies when not needed anymore
	if (g_yellow && ((Map*)g_yellow->parent)->id < GetCurrentMapId())
		SAFE_DELETE(g_yellow);
	if (g_blue && ((Map*)g_blue->parent)->id < GetCurrentMapId())
		SAFE_DELETE(g_blue);
	if (g_red && ((Map*)g_red->parent)->id < GetCurrentMapId())
		SAFE_DELETE(g_red);
	if (g_twotone && ((Map*)g_twotone->parent)->id < GetCurrentMapId())
		SAFE_DELETE(g_twotone);
}
