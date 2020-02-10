#include <vector>
#include "sound.h"
#include "common.h"
#include "pickup.h"
#include "customMath.h"
#include "mydirect3d.h"
#include "player.h"
#include "effect.h"
#include "score.h"


// globals
static std::vector<Pickup*> g_pickup;
static float g_zRotSpeed = 0;

void SpawnPickupAtRandom(Map* map);



void Pickup::Draw()
{
#if _DEBUG
	if(enableDraw)
		BoxCollider::DrawCollider(col, D3DCOLOR(D3DCOLOR_RGBA(0, 255, 255, 255)));
#endif

	MeshObject::Draw();
}

void InitPickup()
{
	// init
	g_zRotSpeed = 10;
	g_pickup = std::vector<Pickup*>();

	// spawn pickups at random
	for (Map* map : *GetMap())
	{
		SpawnPickupAtRandom(map);
	}
}

void UninitPickup()
{
	// free memory
	for (int i = 0; i < g_pickup.size(); ++i)
		SAFE_DELETE(g_pickup[i]);

	g_pickup.clear();
}

void UpdatePickup()
{
	// loop for every pickup
	for (int i = 0; i < g_pickup.size(); i++)
	{
		if (!g_pickup[i]->enableDraw) break;

		// rotate pickup
		g_pickup[i]->transform.localRotation.y += g_zRotSpeed;

		// check for collision with player
		if (GetCurrentMapId() == g_pickup[i]->mapId && g_pickup[i]->col.CheckCollision(GetPlayer()->col))
		{
			// collided, play effect and delete pickup
			PlayEffect(EFFECT_GOLD, g_pickup[i]->GetCombinedPosition(), { 0, 0, 0 }, {0.2F,0.2F,0.2F});
			PlaySound(AUIDO_SE_PICKUP);
			SAFE_DELETE(g_pickup[i]);
			AddScore(100);
			g_pickup.erase(g_pickup.begin() + i);
		}
	}
}


void CleanPickup(int mapId)
{
	// delete all pickups with the given mapId
	for (int i = 0; i < g_pickup.size(); ++i)
	{
		if (g_pickup[i]->mapId > mapId) return;
		if (g_pickup[i]->mapId == mapId)
		{
			delete g_pickup[i];
			g_pickup.erase(g_pickup.begin() + i);
			--i;
		}
	}
}

void SpawnPickupAtRandom(Map* map)
{
	if (map->data.name == MESH_NAME::MESH_MAP_GREEN_STRAIGHT)
	{
		if (map->exit == Direction::NORTH || map->exit == Direction::SOUTH)
		{
			float r = (rand() % 61) - 30;

			SpawnPickup({ r,2,-10 }, map);
			SpawnPickup({ r,2,0 }, map);
			SpawnPickup({ r,2,10 }, map);
			return;
		}
		if (map->exit == Direction::EAST || map->exit == Direction::WEST)
		{
			float r = (rand() % 61) - 30;

			SpawnPickup({ -10,2,r }, map);
			SpawnPickup({ 0,2,r }, map);
			SpawnPickup({ 10,2,r }, map);
			return;
		}
	}
}

void SpawnPickup(D3DXVECTOR3 position, Map* parent)
{
	SpawnPickup(position.x, position.y, position.z, parent);
}

void SpawnPickup(float posX, float posY, float posZ, Map* parent)
{
	Transform trans(D3DXVECTOR3(posX, posY, posZ), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 90, 0), D3DXVECTOR3(0.2F, 0.2F, 0.2F));
	g_pickup.emplace_back(new Pickup(parent->id, trans, MESH_COIN, SHADER_DEFAULT, 12, 12, 12, true, parent));
	g_pickup.back()->enableDraw = parent->enableDraw;
}

std::vector<Pickup*>* GetPickup()
{
	return &g_pickup;
}
