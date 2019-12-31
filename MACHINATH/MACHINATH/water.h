#pragma once

#include "mesh.h"


class Water : public MeshObject
{
public:

	// constructor
	Water(Transform trans, MESH_NAME mesh, SHADER_TYPE type, GameObject* parent = nullptr) :
		MeshObject(trans, mesh, type, parent) {}

	// destructor
	~Water() {}

	// draw
	void Draw() override;
};
