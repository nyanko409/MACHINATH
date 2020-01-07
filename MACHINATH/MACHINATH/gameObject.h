#pragma once

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
	D3DXMATRIX m_matOrientation;	// orienration matrix for local axis rotation
	D3DXVECTOR3 m_prevRotation;		// previous frame rotation of this object
	D3DXVECTOR3 m_prevPosition;		// previous frame position of this object
	D3DXVECTOR3 forward, up, right;	// forward, up and right vector

public:
	LPD3DXEFFECT pShader;			// pointer to the shader
	Transform transform;			// transform data of this gameobject (position, rotation, scale)
	D3DXVECTOR3 pivot;				// pivot point for rotation, default is 0
	GameObject* parent;				// the parent this gameobject is attached to
	bool enableDraw;				// if true, draw this object

	// constructor
	GameObject() {}
	GameObject(Transform transform, SHADER_TYPE type = SHADER_DEFAULT, GameObject* parent = nullptr) : 
		transform(transform), parent(parent)
	{
		// assign the shader at creation
		shaderType = type;
		pShader = AssignShader(this, shaderType);

		D3DXMatrixIdentity(&m_matOrientation);
		pivot = D3DXVECTOR3(0, 0, 0);
		forward = D3DXVECTOR3(0, 0, 1);
		up = D3DXVECTOR3(0, 1, 0);
		right = D3DXVECTOR3(1, 0, 0);
		enableDraw = true;
	}

	// destructor
	~GameObject() 
	{
		// deassign the shader
		DeassignShader(this, shaderType);
	}

	// virtual draw
	virtual void Draw() 
	{
		m_prevPosition = transform.position;
	}

	// returns the final forward vector of both local and world rotation
	D3DXVECTOR3 GetForward()
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

	// returns the combined orientation matrix
	D3DXMATRIX GetCombinedOrientationMatrix()
	{
		if (parent != nullptr)
			return m_matOrientation * parent->GetCombinedOrientationMatrix();

		return m_matOrientation;
	}

	// returns the combined orientation matrix
	D3DXMATRIX GetOrientationMatrix()
	{
		return m_matOrientation;
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

	// returns the scale relative to parent of this gameobject
	D3DXVECTOR3 GetPreviousPosition()
	{
		return m_prevPosition;
	}
};
