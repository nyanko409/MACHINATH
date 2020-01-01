#pragma once

#include "collider.h"
#include "mesh.h"

// player class
class Player : public BoneObject
{
public:
	BoxCollider col;
	float moveSpeed;
	float sideSpeed;
	bool inEvent;

	// constructor
	Player(Transform transform, float moveSpeed, float sideSpeed, ANIMATED_MESH_NAME name, SHADER_TYPE type, float width, float height, float depth, GameObject* parent = nullptr) :
		BoneObject(transform, name, type, parent), col(BoxCollider(this, width, height, depth)),  
		moveSpeed(moveSpeed), sideSpeed(sideSpeed), inEvent(false)
	{}

	// destructor
	~Player() {}

	// draw
	void Draw() override;
};


void InitPlayer();
void UninitPlayer();
void UpdatePlayer();
Player* GetPlayer();
