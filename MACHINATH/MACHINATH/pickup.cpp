#include <vector>
#include "pickup.h"
#include "common.h"
#include "material.h"
#include "mydirect3d.h"
#include "transformation.h"
#include "player.h"

static LPDIRECT3DDEVICE9 device;
static std::vector<Pickup> pickup;
static float moveSpeed = 0.1F;
static float spawnInterval = 1.0F;
static float currentTime = 0.0F;



void InitPickup()
{
	device = MyDirect3D_GetDevice();
	pickup = std::vector<Pickup>(); 
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
		pickup.push_back(Pickup(trans, MESH_COIN));
	}

	for (int i = 0; i < pickup.size(); i++)
	{
		// move pickup to -z direction
		pickup[i].transform.position.z -= moveSpeed;

		// delete unneeded pickup
		if(pickup[i].transform.position.z <= -10)
			pickup.erase(pickup.begin() + i);
	}
}

void DrawPickup()
{
	// draw every pickup in vector
	for (int i = 0; i < pickup.size(); i++)
	{
		TransformObject(pickup[i].transform.position, pickup[i].transform.rotation, pickup[i].transform.scale);
		
		for (int j = 0; j < pickup[i].mesh->numMaterial; j++)
		{
			SetMaterial(pickup[i].mesh->material);
	
			if (pickup[i].mesh->texture[j] != NULL)
				device->SetTexture(0, pickup[i].mesh->texture[j]);
	
			pickup[i].mesh->mesh->DrawSubset(j);
		}
	}
}
