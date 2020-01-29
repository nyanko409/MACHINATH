#include "enemy.h"
#include "map.h"
#include "mesh.h"


BoneObject* g_yellow;


void InitEnemy()
{
	auto mapArray = GetMap();

	// yellow
	for (Map* map : *mapArray)
	{
		if (map->data.name == MESH_MAP_GREEN_HIROBA)
		{
			SAFE_DELETE(g_yellow);

			g_yellow = new BoneObject(Transform(), A_MESH_ENEMY_YELLOW, SHADER_DEFAULT, map);
			g_yellow->transform.rotation.y = 180;
			g_yellow->transform.scale = {4, 4, 4};
			g_yellow->PlayAnimation(1);
			g_yellow->SetAnimationSpeed(0.001F);
		}
	}
}

void UninitEnemy()
{
	SAFE_DELETE(g_yellow);
}

void UpdateEnemy()
{
	if (g_yellow && ((Map*)g_yellow->parent)->id < GetCurrentMapId())
	{
		SAFE_DELETE(g_yellow);
	}
}
