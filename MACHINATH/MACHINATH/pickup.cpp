#include <vector>
#include "pickup.h"
#include "common.h"
#include "material.h"
#include "mydirect3d.h"
#include "transformation.h"
#include "player.h"
#include "effect.h"


// variable declaration
static LPDIRECT3DDEVICE9 device;
static std::vector<Pickup*> pickup;

static float rotSpeed = 10.0F;
static float spawnInterval = 1.0F;
static float currentTime = 0.0F;

static Player* player;



void InitPickup()
{
	device = MyDirect3D_GetDevice();
	pickup = std::vector<Pickup*>(); 
	player = GetPlayer();
}

void UninitPickup()
{

}

void UpdatePickup()
{
	// increase currentime
	currentTime += TIME_PER_FRAME;
	
	// spawn pickup
	if (currentTime >= spawnInterval)
	{
		currentTime -= spawnInterval;

		int pos = (rand() % 21) - 10.0F;
		Transform trans(D3DXVECTOR3(pos, 1, 80), D3DXVECTOR3(0, 90, 0), D3DXVECTOR3(0.2F, 0.2F, 0.2F));
		pickup.push_back(new Pickup(trans, MESH_COIN, SHADER_DEFAULT, 3, 3, 3));
	}

	// loop for every pickup
	for (int i = 0; i < pickup.size(); i++)
	{
		// move pickup to -z direction
		pickup[i]->transform.position.z -= player->moveSpeed;

		// delete pickup out of bounds
		if (pickup[i]->transform.position.z <= -10.0F)
		{
			delete pickup[i];
			pickup.erase(pickup.begin() + i);
		}

		// rotate pickup
		pickup[i]->transform.rotation.y += rotSpeed;

		// check for collision with player
		if (BoxCollider::CheckCollision(*pickup[i], *player))
		{
			// collided, delete coin and play effect
			PlayEffect(EFFECT_PICKUP_MEDAL, pickup[i]->transform.position);

			delete pickup[i];
			pickup.erase(pickup.begin() + i);
		}
	}
}
