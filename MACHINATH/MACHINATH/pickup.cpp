#include <vector>
#include "pickup.h"
#include "common.h"
#include "customMath.h"
#include "material.h"
#include "mydirect3d.h"
#include "transformation.h"
#include "player.h"
#include "effect.h"
#include "map.h"


// globals
static LPDIRECT3DDEVICE9 device;
static std::vector<Pickup*> pickup;
static std::vector<std::pair<int, D3DXVECTOR3>> spawnPos;

static float rotSpeed = 0;
static float poolDistance = 0;




void InitPickup()
{
	// init
	rotSpeed = 10;
	poolDistance = 100;
	device = MyDirect3D_GetDevice();
	pickup = std::vector<Pickup*>();  
	
	// init spawn points
	spawnPos = std::vector<std::pair<int, D3DXVECTOR3>>();
	spawnPos = 
	{
		{0, D3DXVECTOR3(0, 1, 10)}, {0, D3DXVECTOR3(5, 1, 10)}, {1, D3DXVECTOR3(5, 1, 10)},
		{1, D3DXVECTOR3(5, 1, 7)}, {2, D3DXVECTOR3(5, 1, 10)}, {3, D3DXVECTOR3(0, 1, 0)}
	};
}

void UninitPickup()
{

}

void UpdatePickup()
{
	// loop for every pickup
	for (int i = 0; i < pickup.size(); i++)
	{
		// rotate pickup
		pickup[i]->transform.localRotation.y += rotSpeed;

		// check for collision with player
		if (BoxCollider::CheckCollision(*pickup[i], *GetPlayer()))
		{
			// collided, play effect and delete pickup
			//PlayEffect(EFFECT_JUMP, pickup[i]->transform.position);
			delete pickup[i];
			pickup.erase(pickup.begin() + i);
		}
	}
}


void ActivatePickup(int mapId)
{
	// activate all pickups with the given mapId
	for (int i = 0; i < spawnPos.size(); ++i)
	{
		if (spawnPos[i].first > mapId) return;
		if (spawnPos[i].first == mapId)
		{
			SpawnPickup(spawnPos[i].first, spawnPos[i].second, GetMapById(mapId));
		}
	}
}

void CleanPickup(int mapId)
{
	// delete all pickups with the given mapId
	for (int i = 0; i < pickup.size(); ++i)
	{
		if (pickup[i]->mapId > mapId) return;
		if (pickup[i]->mapId == mapId)
		{
			delete pickup[i];
			pickup.erase(pickup.begin() + i);
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
	pickup.emplace_back(new Pickup(mapId, trans, MESH_COIN, SHADER_DEFAULT, 3, 3, 3, parent));
}

std::vector<Pickup*>* GetPickup()
{
	return &pickup;
}
