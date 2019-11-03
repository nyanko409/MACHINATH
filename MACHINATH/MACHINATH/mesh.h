#pragma once

#include "gameObject.h"
#include "meshLoader.h"


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
class BoneObject : public GameObject
{
public:
	SkinMeshFile* mesh;				// data of this mesh


	BoneObject(Transform transform, ANIMATED_MESH_NAME name, GameObject* parent = nullptr) :
		GameObject(transform, parent)
	{
		mesh = GetAnimatedMesh(name);
	}

	~BoneObject() {}

	// draw the object
	void Draw(bool UseWorldPos, bool rotateAtPosition)
	{
		// apply world matrix
		D3DXMATRIX world;
		if (UseWorldPos)
			world = TransformObject(GetWorldPosition(), GetWorldRotation(), GetWorldScale(), rotateAtPosition);
		else
			world = TransformObject(GetLocalPosition(), GetLocalRotation(), GetLocalScale(), rotateAtPosition);

		// draw	
		mesh->UpdateAnim();
		mesh->Draw(&world);
	}

	// set animation speed
	void SetAnimationSpeed(const float speed)
	{
		mesh->SetAnimationSpeed(0, speed);
	}

	// play specified animation track
	void PlayAnimation(const UINT track)
	{
		ID3DXAnimationSet* tr = 0;
		mesh->GetAnimator()->GetAnimationSet(track, &tr);
		mesh->GetAnimator()->SetTrackAnimationSet(0, tr);
		mesh->GetAnimator()->ResetTime();
	}
};
