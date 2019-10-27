#pragma once

#include "gameObject.h"

// player class
class Player : public GameObject, public BoxCollider
{
public:
	float moveSpeed;

	// constructor
	Player(Transform transform, float moveSpeed, MESH_NAME name, GameObject* parent = nullptr) :
		GameObject(transform, name, parent), BoxCollider(&(GameObject::transform), 5, 5, 5),  moveSpeed(moveSpeed) 
	{}

	// destructor
	~Player() {}
};


void InitPlayer();
void UninitPlayer();
void UpdatePlayer();
void DrawPlayer();
Player* GetPlayer();
