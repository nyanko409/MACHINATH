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


void SpawnPickup(float posX, float posY, float posZ, GameObject* parent)
{
	Transform trans(D3DXVECTOR3(posX, posY, posZ), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 90, 0), D3DXVECTOR3(0.2F, 0.2F, 0.2F));
	pickup.emplace_back(new Pickup(trans, MESH_COIN, SHADER_DEFAULT, 3, 3, 3, parent));
}

void InitPickup()
{
	device = MyDirect3D_GetDevice();
	pickup = std::vector<Pickup*>(); 
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
			// collided, delete coin and play effect
			PlayEffect(EFFECT_JUMP, pickup[i]->transform.position);

			delete pickup[i];
			pickup.erase(pickup.begin() + i);
		}
	}
}

std::vector<Pickup*>* GetPickup()
{
	return &pickup;
}

void DeletePickup(Pickup& pu)
{

}
