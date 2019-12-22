#pragma once

#include "gameObject.h"


// AABB (axis aligned bounding box collision), inherit if needed
class BoxCollider
{
private:
	v3t_float m_size;
	v3t_float m_topLeft;

	GameObject* m_objTransform;

public:
	bool isStatic;		// set it to true if the object does not move to skip unnessacery calculations

	// constructor
	BoxCollider(GameObject* transform, float width, float height, float depth, bool isStatic = false) :
		m_objTransform(transform), isStatic(isStatic)
	{
		// calculate the size of the collider
		m_size.x = width * m_objTransform->GetCombinedScale().x;
		m_size.y = height * m_objTransform->GetCombinedScale().y;
		m_size.z = depth * m_objTransform->GetCombinedScale().z;

		// if object does not move, calculate position just once
		if(isStatic)
			m_topLeft = GetTopLeft();
	}

	// destructor
	~BoxCollider() {}

	// get width and height of this object
	v3t_float GetSize() const { return m_size; }

	// get current top left position based on object and parent position
	v3t_float GetTopLeft() const
	{
		return v3t_float((-m_size.x / 2) + m_objTransform->GetCombinedPosition().x,
			(m_size.y / 2) + m_objTransform->GetCombinedPosition().y,
			(-m_size.z / 2) + m_objTransform->GetCombinedPosition().z);
	}

	// calculate collision between two box colliders
	static bool CheckCollision(const BoxCollider& col1, const BoxCollider& col2);

	// draw the collider on screen
	static void DrawCollider(const BoxCollider& col);
};
