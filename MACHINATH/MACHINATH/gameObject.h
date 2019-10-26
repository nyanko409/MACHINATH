#pragma once

#include "transform.h"
#include "mydirect3d.h"
#include "meshLoader.h"
#include <string>

class GameObject
{
public:
	Transform transform;			// transform data of this gameobject (position, rotation, scale)
	GameObject* parent;				// the parent this gameobject is attached to

	MESH_DATA* mesh;				// data of this mesh

	// constructor
	GameObject() {}

	GameObject(Transform transform, MESH_NAME name, GameObject* parent = nullptr) : transform(transform), parent(parent) 
	{
		// load mesh
		mesh = GetMesh(name);
	}

	// destructor
	~GameObject() 
	{
		if(parent) free(parent);
		//if (mesh) free(mesh);
	}

	// returns the world space position of this gameobject
	D3DXVECTOR3 GetWorldPosition()
	{
		if(parent != nullptr)
			return transform.position + parent->GetWorldPosition();

		return transform.position;
	}

	// returns the local space position relative to parent of this gameobject
	D3DXVECTOR3 GetLocalPosition()
	{
		return transform.position;
	}
};
