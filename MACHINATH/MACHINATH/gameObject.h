#pragma once

#include <vector>
#include <algorithm>
#include "transform.h"
#include "shader.h"


// simple vector 3 template struct
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

protected:
	D3DXMATRIX m_matOrientation;		// orienration matrix for local axis rotation
	D3DXVECTOR3 m_prevRotation;			// previous frame rotation of this object
	D3DXVECTOR3 m_prevPosition;			// previous frame position of this object
	D3DXVECTOR3 forward, up, right;		// forward, up and right vector

public:
	Transform transform;				// transform data of this gameobject (position, rotation, scale)
	LPD3DXEFFECT pShader;				// pointer to the shader
	D3DXVECTOR3 pivot;					// pivot point for rotation, default is 0
	GameObject* parent;					// the parent this gameobject is attached to
	std::vector<GameObject*> child;		// list of children of this gameobject
	bool enableDraw;					// if true, draw this object


	// constructor
	GameObject() {}
	GameObject(Transform transform, SHADER_TYPE type = SHADER_DEFAULT, GameObject* parent = nullptr) : 
		transform(transform), shaderType(type), pShader(AssignShader(this, shaderType)),
		child(std::vector<GameObject*>()), enableDraw(true), pivot(D3DXVECTOR3(0, 0, 0)),
		forward(D3DXVECTOR3(0, 0, 1)), up(D3DXVECTOR3(0, 1, 0)), right(D3DXVECTOR3(1, 0, 0)),
		m_matOrientation(*D3DXMatrixIdentity(&D3DXMATRIX{}))
	{
		// set parent
		SetParent(parent);
	}

	// destructor
	virtual ~GameObject() 
	{
		// deassign the shader
		DeassignShader(this, shaderType);

		// delete the references of children to this gameobject
		for (auto c : child)
		{
			c->parent = nullptr;
		}

		// remove this object from parent child list
		if (parent)
			parent->child.erase(std::remove(parent->child.begin(), parent->child.end(), this));
	}

	// virtual draw
	virtual void Draw() 
	{
		m_prevRotation = transform.localRotation;
		m_prevPosition = transform.position;
	}

	// set the parent of this gameobject
	void SetParent(GameObject* parent)
	{
		if (!parent) return;

		this->parent = parent;
		parent->child.emplace_back(this);
	}

	// removes the parent of this gameobject
	void RemoveParent()
	{

	}

	// returns the final forward vector of both local and world rotation
	D3DXVECTOR3 GetForward() const
	{
		// create y rotation matrix
		D3DXMATRIX rotX, rotY, rotZ;
		D3DXMatrixRotationX(&rotX, D3DXToRadian(transform.rotation.x));
		D3DXMatrixRotationY(&rotY, D3DXToRadian(transform.rotation.y));
		D3DXMatrixRotationZ(&rotZ, D3DXToRadian(transform.rotation.z));

		D3DXVECTOR3 newForward = forward;
		D3DXVec3TransformCoord(&newForward, &newForward, &rotX);
		D3DXVec3TransformCoord(&newForward, &newForward, &rotY);
		D3DXVec3TransformCoord(&newForward, &newForward, &rotZ);

		return newForward;
	}

	// returns the combined position of this gameobject
	D3DXVECTOR3 GetCombinedPosition() const
	{
		if(parent != nullptr)
			return transform.position + parent->GetCombinedPosition();

		return transform.position;
	}

	// returns the position relative to parent of this gameobject
	D3DXVECTOR3 GetPosition() const
	{
		return transform.position;
	}

	// returns the combined rotation of this gameobject
	D3DXVECTOR3 GetCombinedRotation() const
	{
		if (parent != nullptr)
			return transform.rotation + parent->GetCombinedRotation();

		return transform.rotation;
	}

	// returns the combined orientation matrix
	D3DXMATRIX GetCombinedOrientationMatrix() const
	{
		if (parent != nullptr)
			return m_matOrientation * parent->GetCombinedOrientationMatrix();

		return m_matOrientation;
	}

	// returns the combined orientation matrix
	D3DXMATRIX GetOrientationMatrix() const
	{
		return m_matOrientation;
	}

	// returns the rotation relative to parent of this gameobject
	D3DXVECTOR3 GetRotation() const
	{
		return transform.rotation;
	}

	// returns the combined rotation of this gameobject
	D3DXVECTOR3 GetCombinedLocalRotation() const
	{
		if (parent != nullptr)
			return transform.localRotation + parent->GetCombinedLocalRotation();

		return transform.localRotation;
	}

	// returns the rotation relative to parent of this gameobject
	D3DXVECTOR3 GetLocalRotation() const
	{
		return transform.localRotation;
	}

	// returns the combined scale of this gameobject
	D3DXVECTOR3 GetCombinedScale() const
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
	D3DXVECTOR3 GetScale() const 
	{
		return transform.scale;
	}

	// returns the scale relative to parent of this gameobject
	D3DXVECTOR3 GetPreviousPosition() const 
	{
		return m_prevPosition;
	}
};
