#include <vector>
#include "pickup.h"
#include "common.h"
#include "material.h"
#include "mydirect3d.h"
#include "transformation.h"
#include "player.h"


// variable declaration
static LPDIRECT3DDEVICE9 device;
static std::vector<Pickup*> pickup;
static float moveSpeed = 0.5F;
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

		Transform trans(D3DXVECTOR3(0, 1, 80), D3DXVECTOR3(0, 90, 0), D3DXVECTOR3(0.2F, 0.2F, 0.2F));
		pickup.push_back(new Pickup(trans, MESH_COIN));
	}

	// loop for every pickup
	for (int i = 0; i < pickup.size(); i++)
	{
		// move pickup to -z direction
		pickup[i]->transform.position.z -= moveSpeed;

		// delete unneeded pickup
		if(pickup[i]->transform.position.z <= -10)
			pickup.erase(pickup.begin() + i);

		// only check for collision if needed
		if (pickup[i]->transform.position.z < player->transform.position.z + player->GetSize().z)
		{
			// check for collision with player
			if (BoxCollider::CheckCollision(*pickup[i], *player))
			{
				// collided, delete coin
				pickup.erase(pickup.begin() + i);
			}
		}
	}
}

void DrawPickup()
{
	// draw every pickup in vector
	device->SetTexture(0, NULL);

	for (int i = 0; i < pickup.size(); i++)
	{
		TransformObject(pickup[i]->transform.position, pickup[i]->transform.rotation, pickup[i]->transform.scale);
		
		for (int j = 0; j < pickup[i]->mesh->numMaterial; j++)
		{
			SetMaterial(pickup[i]->mesh->material);
	
			if (pickup[i]->mesh->texture[j] != NULL)
				device->SetTexture(0, pickup[i]->mesh->texture[j]);
	
			pickup[i]->mesh->mesh->DrawSubset(j);
		}
	}
}
