#pragma once

#include "gameObject.h"
#include "meshLoader.h"


// inherit this class if the object has unanimated mesh
class MeshObject : public GameObject
{
public:
	MESH_DATA* mesh;				// data of this mesh

	MeshObject(Transform transform, MESH_NAME name, SHADER_TYPE type = SHADER_DEFAULT, GameObject* parent = nullptr) :
		GameObject(transform, type, parent)
	{
		// set mesh
		mesh = GetMesh(name);
	}

	~MeshObject() {}

	// draw the object
	virtual void Draw() override
	{
		if (!enableDraw) return;

		auto device = MyDirect3D_GetDevice();

		// get world matrix
		D3DXMATRIX matWorld;

		D3DXVECTOR3 diff = m_prevRotation - transform.localRotation;
		m_prevRotation = transform.localRotation;

		matWorld = TransformObject(GetCombinedPosition(), GetCombinedScale(), diff, 
			m_matOrientation, GetCombinedOrientationMatrix(), GetCombinedRotation(), forward, up, right, pivot);


		// set world transform
		if (pShader)
		{
			pShader->SetMatrix("World", &matWorld);
			pShader->CommitChanges();
		}
		else 
			device->SetTransform(D3DTS_WORLD, &matWorld);

		// draw mesh
		for (DWORD i = 0; i < mesh->numMaterial; i++)
		{
			SetMaterial(&(mesh->pMaterial[i]));

			if (mesh->pTexture[i] != NULL)
				device->SetTexture(0, mesh->pTexture[i]);
			else
				device->SetTexture(0, NULL);

			mesh->mesh->DrawSubset(i);
		}
	}
};


// inherit this class if the object has bone animation
class BoneObject : public GameObject
{
public:
	SkinMeshFile* mesh;				// data of this mesh

	BoneObject(Transform transform, ANIMATED_MESH_NAME name, SHADER_TYPE type = SHADER_DEFAULT, GameObject* parent = nullptr) :
		GameObject(transform, type, parent)
	{
		mesh = GetAnimatedMesh(name);
	}

	~BoneObject() {}

	// draw the object
	virtual void Draw() override
	{
		if (!enableDraw) return;

		auto device = MyDirect3D_GetDevice();

		// get world matrix
		D3DXMATRIX matWorld;

		D3DXVECTOR3 diff = m_prevRotation - transform.localRotation;
		m_prevRotation = transform.localRotation;
		matWorld = TransformObject(GetCombinedPosition(), GetCombinedScale(), diff, 
			m_matOrientation, GetCombinedOrientationMatrix(), GetCombinedRotation(), forward, up, right, pivot);


		// apply world matrix
		if (pShader)
			pShader->SetMatrix("World", &matWorld);
		else
			device->SetTransform(D3DTS_WORLD, &matWorld);

		// draw	
		mesh->UpdateAnim();
		mesh->Draw(&matWorld, pShader);
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
