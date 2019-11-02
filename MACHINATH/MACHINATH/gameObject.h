#pragma once

#include "transform.h"
#include "transformation.h"
#include "mydirect3d.h"
#include "meshLoader.h"
#include "material.h"
#include <string>


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
public:
	Transform transform;			// transform data of this gameobject (position, rotation, scale)
	GameObject* parent;				// the parent this gameobject is attached to

	// constructor
	GameObject() {}
	GameObject(Transform transform, GameObject* parent = nullptr) : transform(transform), parent(parent) {}

	// destructor
	~GameObject() {}

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

	// returns the world space rotation of this gameobject
	D3DXVECTOR3 GetWorldRotation()
	{
		if (parent != nullptr)
			return transform.rotation + parent->GetWorldRotation();

		return transform.rotation;
	}

	// returns the local space rotation relative to parent of this gameobject
	D3DXVECTOR3 GetLocalRotation()
	{
		return transform.rotation;
	}

	// returns the world space rotation of this gameobject
	D3DXVECTOR3 GetWorldScale()
	{
		if (parent != nullptr)
			return transform.scale + parent->GetWorldScale();

		return transform.scale;
	}

	// returns the local space rotation relative to parent of this gameobject
	D3DXVECTOR3 GetLocalScale()
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

	Transform* obj_transform;

public:
	// constructor
	BoxCollider(Transform* transform, float width, float height, float depth) : obj_transform(transform)
	{
		size.x = width * obj_transform->scale.x;
		size.y = height * obj_transform->scale.y;
		size.z = depth * obj_transform->scale.z;
		
		topLeft.x = (-width / 2) + obj_transform->position.x;
		topLeft.y = (height / 2) + obj_transform->position.y;
		topLeft.z = (-width / 2) + obj_transform->position.z;
	}

	// destructor
	~BoxCollider() {}

	// get width and height of this object
	v3t_float GetSize() const { return size; }

	// get current top left position based on object position
	v3t_float GetTopLeft() const
	{
		return v3t_float((-size.x / 2) + obj_transform->position.x, 
						(size.y / 2) + obj_transform->position.y,
						(-size.z / 2) + obj_transform->position.z);
	}


	// calculate collision between two box colliders
	static bool CheckCollision(const BoxCollider& col1, const BoxCollider& col2)
	{
		// cache necessary data
		v3t_float size1 = col1.GetSize(), size2 = col2.GetSize();
		v3t_float topLeft1 = col1.GetTopLeft(), topLeft2 = col2.GetTopLeft();

		// check collision
		if (topLeft1.x < topLeft2.x + size2.x && topLeft1.x + size1.x > topLeft2.x)
		{
			if (topLeft1.z < topLeft2.z + size2.z && topLeft1.z + size1.z > topLeft2.z)
			{
				// collision occured
				return true;
			}
		}

		// no collision occured
		return false;
	}
};

