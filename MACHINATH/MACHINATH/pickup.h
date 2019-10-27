#pragma once

#include "gameObject.h"

// pickup class - medals, powerups etc
class Pickup : public GameObject, public BoxCollider
{
public:
	// constructor
	Pickup(Transform trans, MESH_NAME mesh) : GameObject(trans, mesh), BoxCollider(&(GameObject::transform), 5, 5, 5)
	{}
	
	// destructor
	~Pickup() {}
};


void InitPickup();
void UninitPickup();
void UpdatePickup();
void DrawPickup();
