#pragma once

#include "mesh.h"


class Boss : public MeshObject
{
public:
	Boss(Transform transform, MESH_NAME name, SHADER_TYPE type, GameObject* parent = nullptr) :
		MeshObject(transform, name, type, parent)
	{}

	~Boss() {}

	void Draw() override;

private:

};


void InitBoss();
void UninitBoss();
void UpdateBoss();
