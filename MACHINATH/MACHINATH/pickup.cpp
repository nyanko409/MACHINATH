#include <vector>
#include "common.h"
#include "pickup.h"
#include "customMath.h"
#include "mydirect3d.h"
#include "player.h"
#include "effect.h"
#include "map.h"
#include "score.h"


// globals
static std::vector<Pickup*> g_pickup;
static std::vector<std::pair<int, D3DXVECTOR3>> g_spawnPos =
{
	{1, D3DXVECTOR3(0, 1, 10)}, {1, D3DXVECTOR3(0, 1, 0)}, {2, D3DXVECTOR3(0, 1, 0)},
	{3, D3DXVECTOR3(0, 1, 0)}, {4, D3DXVECTOR3(0, 1, 0)}, {5, D3DXVECTOR3(0, 1, 0)}
};;
static float g_zRotSpeed = 0;


void Pickup::Draw()
{
	MeshObject::Draw();
}

void InitPickup()
{
	// init
	g_zRotSpeed = 10;
	g_pickup = std::vector<Pickup*>();
}

void UninitPickup()
{
	// free memory
	for (int i = 0; i < g_pickup.size(); ++i)
	{
		if (g_pickup[i] != nullptr)
			SAFE_DELETE(g_pickup[i]);
	}
}

void UpdatePickup()
{
	// loop for every pickup
	for (int i = 0; i < g_pickup.size(); i++)
	{
		// rotate pickup
		g_pickup[i]->transform.localRotation.y += g_zRotSpeed;

		// check for collision with player
		if (g_pickup[i]->col.CheckCollision(GetPlayer()->col))
		{
			// collided, play effect and delete pickup
			PlayEffect(EFFECT_JUMP, g_pickup[i]->GetCombinedPosition(), { 0, 0, 0 }, { 0, 0, 0 }, {5, 5, 5});
			SAFE_DELETE(g_pickup[i]);
			AddScore(100);
			g_pickup.erase(g_pickup.begin() + i);
		}
	}
}


void ActivatePickup(int mapId)
{
	// activate all pickups with the given mapId
	for (int i = 0; i < g_spawnPos.size(); ++i)
	{
		if (g_spawnPos[i].first > mapId) return;
		if (g_spawnPos[i].first == mapId)
		{
			SpawnPickup(g_spawnPos[i].first, g_spawnPos[i].second, GetMapById(mapId));
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
			i--;
		}
	}
}

void SpawnPickup(int mapId, D3DXVECTOR3 position, GameObject* parent)
{
	SpawnPickup(mapId, position.x, position.y, position.z, parent);
}

void SpawnPickup(int mapId, float posX, float posY, float posZ, GameObject* parent)
{
	Transform trans(D3DXVECTOR3(posX, posY, posZ), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 90, 0), D3DXVECTOR3(0.2F, 0.2F, 0.2F));
	g_pickup.emplace_back(new Pickup(mapId, trans, MESH_COIN, SHADER_DEFAULT, 12, 12, 12, true, parent));
}

std::vector<Pickup*>* GetPickup()
{
	return &g_pickup;
}
