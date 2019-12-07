#pragma once

#include <string>
#include "transform.h"
#include "transformation.h"
#include "mydirect3d.h"
#include "meshLoader.h"
#include "material.h"
#include "shader.h"


// simple vector 2 template struct
template<typename T>
struct v3t
{
	T x, y, z;

	v3t() {}
	v3t(T x, T y, T z) : x(x), y(y), z(z) {}
	~v3t() {}
};

// type definition of v3t struct for convenience
typedef v3t<int>	 v3t_int;
typedef v3t<float>	 v3t_float;


// base gameobject class
class GameObject
{
private:
	SHADER_TYPE shaderType;

public:
	LPD3DXEFFECT pShader;			// pointer to the shader
	Transform transform;			// transform data of this gameobject (position, rotation, scale)
	GameObject* parent;				// the parent this gameobject is attached to
	bool enableDraw;				// if true, draw this object

	// constructor
	GameObject() {}
	GameObject(Transform transform, SHADER_TYPE type = SHADER_DEFAULT, GameObject* parent = nullptr) : transform(transform), parent(parent) 
	{
		// assign the shader at creation
		shaderType = type;
		pShader = AssignShader(this, shaderType);

		enableDraw = true;
	}

	// destructor
	~GameObject() 
	{
		// deassign the shader
		DeassignShader(this, shaderType);
	}

	// virtual draw
	virtual void Draw() {}

	// returns the combined position of this gameobject
	D3DXVECTOR3 GetCombinedPosition()
	{
		if(parent != nullptr)
			return transform.position + parent->GetCombinedPosition();

		return transform.position;
	}

	// returns the position relative to parent of this gameobject
	D3DXVECTOR3 GetPosition()
	{
		return transform.position;
	}

	// returns the combined rotation of this gameobject
	D3DXVECTOR3 GetCombinedRotation()
	{
		if (parent != nullptr)
			return transform.rotation + parent->GetCombinedRotation();

		return transform.rotation;
	}

	// returns the rotation relative to parent of this gameobject
	D3DXVECTOR3 GetRotation()
	{
		return transform.rotation;
	}

	// returns the combined rotation of this gameobject
	D3DXVECTOR3 GetCombinedLocalRotation()
	{
		if (parent != nullptr)
			return transform.localRotation + parent->GetCombinedLocalRotation();

		return transform.localRotation;
	}

	// returns the rotation relative to parent of this gameobject
	D3DXVECTOR3 GetLocalRotation()
	{
		return transform.localRotation;
	}

	// returns the combined scale of this gameobject
	D3DXVECTOR3 GetCombinedScale()
	{
		if (parent != nullptr)
		{
			return D3DXVECTOR3(transform.scale.x * parent->GetCombinedScale().x,
								transform.scale.y * parent->GetCombinedScale().y,
								transform.scale.z * parent->GetCombinedScale().z);
		}

		return transform.scale;
	}

	// returns the scale relative to parent of this gameobject
	D3DXVECTOR3 GetScale()
	{
		return transform.scale;
	}
};


// box collider, inherit if needed
class BoxCollider
{
private:
	v3t_float size;
	v3t_float topLeft;

	GameObject* obj_transform;

public:
	// constructor
	BoxCollider(GameObject* transform, float width, float height, float depth) : obj_transform(transform)
	{
		size.x = width * obj_transform->GetCombinedScale().x;
		size.y = height * obj_transform->GetCombinedScale().y;
		size.z = depth * obj_transform->GetCombinedScale().z;
		
		topLeft.x = (-width / 2) + obj_transform->GetCombinedPosition().x;
		topLeft.y = (height / 2) + obj_transform->GetCombinedPosition().y;
		topLeft.z = (-width / 2) + obj_transform->GetCombinedPosition().z;
	}

	// destructor
	~BoxCollider() {}

	// get width and height of this object
	v3t_float GetSize() const { return size; }

	// get current top left position based on object position
	v3t_float GetTopLeft() const
	{
		D3DXMATRIX mRot;
		D3DXMatrixRotationY(&mRot, obj_transform->GetCombinedRotation().y);

		D3DXVECTOR3 finalPos = obj_transform->GetCombinedPosition();
		D3DXVec3TransformCoord(&finalPos, &finalPos, &mRot);

		return v3t_float((-size.x / 2) + finalPos.x,
						(size.y / 2) + finalPos.y,
						(-size.z / 2) + finalPos.z);

		//return v3t_float((-size.x / 2) + obj_transform->GetCombinedPosition().x, 
		//				(size.y / 2) + obj_transform->GetCombinedPosition().y,
		//				(-size.z / 2) + obj_transform->GetCombinedPosition().z);
	}


	// calculate collision between two box colliders
	static bool CheckCollision(const BoxCollider& col1, const BoxCollider& col2)
	{
		// cache necessary data
		v3t_float size1 = col1.GetSize(), size2 = col2.GetSize();
		v3t_float topLeft1 = col1.GetTopLeft(), topLeft2 = col2.GetTopLeft();

		// check collision
		if (topLeft1.x < topLeft2.x + size2.x && topLeft1.x + size1.x > topLeft2.x)
			if (topLeft1.z < topLeft2.z + size2.z && topLeft1.z + size1.z > topLeft2.z)
				if(topLeft1.y > topLeft2.y - size2.y && topLeft1.y - size1.y < topLeft2.y)
					return true;

		// no collision occured
		return false;
	}
};
