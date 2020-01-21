#include <vector>
#include "sound.h"
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
static float g_zRotSpeed = 0;

// set pickups
static std::vector<std::pair<int, D3DXVECTOR3>> g_spawnPos =
{
	{ 1, D3DXVECTOR3(0, 1.3f, -30)},
	{ 1, D3DXVECTOR3(0, 1.3f, 0)},
	{ 1, D3DXVECTOR3(0, 1.3f, 30)},
	{ 2, D3DXVECTOR3(0, 1.3f, 0)},
	{ 3, D3DXVECTOR3(0, 1.3f, 0)},
	{ 4, D3DXVECTOR3(0, 1.3f, 0)},
	{ 5, D3DXVECTOR3(0, 1.3f, 0)},
	{ 6, D3DXVECTOR3(0, 1.3f, 0)},
	{ 7, D3DXVECTOR3(0, 1.3f, 0)},
	{ 8, D3DXVECTOR3(0, 1.3f, 0)},
	{ 9, D3DXVECTOR3(0, 1.3f, 0)},
	{ 10, D3DXVECTOR3(0, 1.3f, 0)},
	{ 11, D3DXVECTOR3(0, 1.3f, 0)},
	{ 12, D3DXVECTOR3(0, 1.3f, 0)},
	{ 13, D3DXVECTOR3(0, 1.3f, 0)},
	{ 14, D3DXVECTOR3(0, 1.3f, 0)},
	{ 15, D3DXVECTOR3(0, 1.3f, 0)},
	{ 16, D3DXVECTOR3(0, 1.3f, 0)},
	{ 17, D3DXVECTOR3(0, 1.3f, 0)},
	{ 18, D3DXVECTOR3(0, 1.3f, 0)},
	{ 19, D3DXVECTOR3(0, 1.3f, 0)},
	{ 20, D3DXVECTOR3(0, 1.3f, 0)},
};



void Pickup::Draw()
{
#if _DEBUG
	for(auto pickup : g_pickup)
		BoxCollider::DrawCollider(pickup->col, D3DCOLOR(D3DCOLOR_RGBA(0, 255, 255, 255)));
#endif

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
		SAFE_DELETE(g_pickup[i]);

	g_pickup.clear();
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
			PlayEffect(EFFECT_GOLD, g_pickup[i]->GetCombinedPosition(), { 0, 0, 0 }, { 0, 0, 0 }, {0.2F,0.2F,0.2F});
			PlaySound(AUIDO_SE_PICKUP);
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
			--i;
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
