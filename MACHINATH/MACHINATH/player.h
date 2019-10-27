#pragma once

#include "gameObject.h"

// player class
class Player : public GameObject, public BoxCollider
{
public:
	float moveSpeed;

	// constructor
	Player(Transform transform, float moveSpeed, MESH_NAME name, float width, float height, float depth, GameObject* parent = nullptr) :
		GameObject(transform, name, parent), BoxCollider(&(GameObject::transform), width, height, depth),  moveSpeed(moveSpeed) 
	{}

	// destructor
	~Player() {}
};


void InitPlayer();
void UninitPlayer();
void UpdatePlayer();
void DrawPlayer();
Player* GetPlayer();
