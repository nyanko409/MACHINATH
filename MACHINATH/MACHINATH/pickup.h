#pragma once

#include "collider.h"
#include "mesh.h"

// pickup class - medals, powerups etc
class Pickup : public MeshObject, public BoxCollider
{
public:
	int mapId;

	// constructor
	Pickup(int mapId, Transform trans, MESH_NAME mesh, SHADER_TYPE type, float width, float height, float depth, GameObject* parent = nullptr) :
		mapId(mapId), MeshObject(trans, mesh, type, parent), BoxCollider(this, width, height, depth) {}
	
	// destructor
	~Pickup() {}

	// draw
	void Draw() override
	{
		MeshObject::Draw();
	}

	
};


// spawn pickup at position
void SpawnPickup(int mapId, D3DXVECTOR3 position, GameObject* parent = nullptr);
void SpawnPickup(int mapId, float posX, float posY, float posZ, GameObject* parent = nullptr);

// delete every pickup with the given mapId
void CleanPickup(int mapId);

// activate every pickup with the given mapId
void ActivatePickup(int mapId);

// returns a vector of every pickup
std::vector<Pickup*>* GetPickup();

void InitPickup();
void UninitPickup();
void UpdatePickup();
