#include "enemy.h"
#include "map.h"
#include "mesh.h"


BoneObject* g_yellow;
BoneObject* g_twotone;


void InitEnemy()
{
	auto mapArray = GetMap();

	// init enemies at map
	for (Map* map : *mapArray)
	{
		if (map->data.name == MESH_MAP_GREEN_HIROBA)
		{
			SAFE_DELETE(g_yellow);

			g_yellow = new BoneObject(Transform(), A_MESH_ENEMY_YELLOW, SHADER_DEFAULT, map);
			g_yellow->transform.rotation.y = 180;
			g_yellow->transform.scale = {4, 4, 4};
			g_yellow->PlayAnimation(1);
			g_yellow->SetAnimationSpeed(0.005F);
		}
		else if (map->data.name == MESH_MAP_TWOTONE_HIROBA)
		{
			SAFE_DELETE(g_twotone);

			g_twotone = new BoneObject(Transform(), A_MESH_ENEMY_TWOTONE, SHADER_DEFAULT, map);
			g_twotone->transform.rotation.y = 180;
			g_twotone->transform.scale = { 4, 4, 4 };
			g_twotone->PlayAnimation(1);
			g_twotone->SetAnimationSpeed(0.005F);
		}
	}
}

void UninitEnemy()
{
	SAFE_DELETE(g_yellow);
	SAFE_DELETE(g_twotone);
}

void UpdateEnemy()
{
	if (g_yellow && ((Map*)g_yellow->parent)->id < GetCurrentMapId())
	{
		SAFE_DELETE(g_yellow);
		SAFE_DELETE(g_twotone);
	}
}
