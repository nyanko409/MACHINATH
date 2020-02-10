#pragma once

#include "collider.h"
#include "mesh.h"
#include "map.h"


// pickup class - medals, powerups etc
class Pickup : public MeshObject
{
public:
	BoxCollider col;
	int mapId;

	// constructor
	Pickup(int mapId, Transform trans, MESH_NAME mesh, SHADER_TYPE type, float width, float height, float depth, bool isStatic = true, GameObject* parent = nullptr) :
		mapId(mapId), MeshObject(trans, mesh, type, parent), col(BoxCollider(this, width, height, depth, {0,0,0}, isStatic)) {}
	
	// destructor
	~Pickup() {}

	// draw
	void Draw() override;
};


// spawn pickup at position
void SpawnPickup(D3DXVECTOR3 position, Map* parent = nullptr);
void SpawnPickup(float posX, float posY, float posZ, Map* parent = nullptr);

// delete every pickup with the given mapId
void CleanPickup(int mapId);

// returns a vector of every pickup
std::vector<Pickup*>* GetPickup();

void InitPickup();
void UninitPickup();
void UpdatePickup();
