#pragma once

#include "gameObject.h"


// inherit this class if the object has unanimated mesh
class MeshObject : public GameObject
{
public:
	MESH_DATA* mesh;				// data of this mesh


	MeshObject(Transform transform, MESH_NAME name, GameObject* parent = nullptr) :
		GameObject(transform, parent)
	{
		// set mesh
		mesh = GetMesh(name);
	}

	~MeshObject() {}

	// draw the object
	void Draw(bool UseWorldPos, bool rotateAtPosition)
	{
		auto device = MyDirect3D_GetDevice();

		// apply world matrix
		if (UseWorldPos)
			TransformObject(GetWorldPosition(), GetWorldRotation(), GetWorldScale(), rotateAtPosition);
		else
			TransformObject(GetLocalPosition(), GetLocalRotation(), GetLocalScale(), rotateAtPosition);

		// draw
		for (DWORD i = 0; i < mesh->numMaterial; i++)
		{
			SetMaterial(&(mesh->material[i]));

			if (mesh->texture[i] != NULL)
				device->SetTexture(0, mesh->texture[i]);

			mesh->mesh->DrawSubset(i);
		}
	}
};


// inherit this class if the object has bone animation
//class BoneObject : public GameObject
//{
//public:
//	MESH_DATA* mesh;				// data of this mesh
//
//
//	BoneObject(Transform transform, MESH_NAME name, GameObject* parent = nullptr) :
//		GameObject(transform, parent)
//	{
//		file = new SkinMeshFile();
//		file->Load("asset/mesh/robotA.x");
//		//file->SetAnimationSpeed(0, 0.01F);
//	}
//
//	~BoneObject() {}
//
//	// draw the object
//	void Draw(bool UseWorldPos, bool rotateAtPosition)
//	{
//		auto device = MyDirect3D_GetDevice();
//
//		// apply world matrix
//		if (UseWorldPos)
//			TransformObject(GetWorldPosition(), GetWorldRotation(), GetWorldScale(), rotateAtPosition);
//		else
//			TransformObject(GetLocalPosition(), GetLocalRotation(), GetLocalScale(), rotateAtPosition);
//
//		// draw
//		for (DWORD i = 0; i < mesh->numMaterial; i++)
//		{
//			SetMaterial(&(mesh->material[i]));
//
//			if (mesh->texture[i] != NULL)
//				device->SetTexture(0, mesh->texture[i]);
//
//			mesh->mesh->DrawSubset(i);
//		}
//	}
//};
