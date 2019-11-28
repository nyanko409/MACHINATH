#pragma once

#include "mesh.h"

// pickup class - medals, powerups etc
class Pickup : public MeshObject, public BoxCollider
{
public:
	// constructor
	Pickup(Transform trans, MESH_NAME mesh, SHADER_TYPE type, float width, float height, float depth, GameObject* parent = nullptr) :
		MeshObject(trans, mesh, type, parent), BoxCollider(this, width, height, depth) {}
	
	// destructor
	~Pickup() {}

	// draw
	void Draw() override
	{
		MeshObject::Draw();
	}
};


void SpawnPickup(float posX, float posY, float posZ, GameObject* parent = nullptr);

std::vector<Pickup*>* GetPickup();

void InitPickup();
void UninitPickup();
void UpdatePickup();
