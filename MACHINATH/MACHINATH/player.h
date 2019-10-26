#pragma once

#include "gameObject.h"

class Player : public GameObject
{
public:
	float moveSpeed;

	// constructor
	Player(Transform transform, float moveSpeed, MESH_NAME name, GameObject* parent = nullptr) :
		GameObject(transform, name, parent), moveSpeed(moveSpeed) {}

	// destructor
	~Player() {}
};


void InitPlayer();
void UninitPlayer();
void UpdatePlayer();
void DrawPlayer();
