#pragma once

#include "collider.h"
#include "mesh.h"


class Enemy : public BoneObject
{
public:
	BoxCollider collider;

	// constructor
	Enemy(Transform trans, ANIMATED_MESH_NAME mesh, SHADER_TYPE type, float width, float height, float depth, D3DXVECTOR3 colliderOffset = {0, 0, 0}, bool isStatic = true, GameObject* parent = nullptr) :
		BoneObject(trans, mesh, type, parent), collider(BoxCollider(this, width, height, depth, colliderOffset, isStatic)) 
	{}

	// destructor
	~Enemy() {}

	// draw
	void Draw() override;
};



void InitEnemy();
void UninitEnemy();
void UpdateEnemy();
