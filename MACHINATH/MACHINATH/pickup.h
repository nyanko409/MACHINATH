#pragma once

#include "gameObject.h"

// pickup class - medals, powerups etc
class Pickup : public GameObject
{
public:
	// constructor
	Pickup(Transform trans, MESH_NAME mesh) : GameObject(trans, mesh) {}
	
	// destructor
	~Pickup() {}
};


void InitPickup();
void UninitPickup();
void UpdatePickup();
void DrawPickup();
