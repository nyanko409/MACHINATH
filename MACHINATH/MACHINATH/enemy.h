#pragma once

#include "collider.h"
#include "mesh.h"


class Enemy : public BoneObject
{
public:
	BoxCollider collider;
	BoxCollider* eventCollider;
	bool collided;

	// constructor
	Enemy(Transform trans, ANIMATED_MESH_NAME mesh, SHADER_TYPE type,
		float width, float height, float depth, D3DXVECTOR3 colliderOffset,
		float width2, float height2, float depth2, D3DXVECTOR3 colliderOffset2,
		bool isStatic = true, GameObject* parent = nullptr) :
		BoneObject(trans, mesh, type, parent),
		collider(BoxCollider(this, width, height, depth, colliderOffset, isStatic)),
		eventCollider(new BoxCollider(this, width2, height2, depth2, colliderOffset2, isStatic)),
		collided(false)
	{}

	// destructor
	~Enemy() 
	{
		SAFE_DELETE(eventCollider);
	}

	// draw
	void Draw() override;
};



void InitEnemy();
void UninitEnemy();
void UpdateEnemy();
