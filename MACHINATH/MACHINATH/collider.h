#pragma once

#include "gameObject.h"


// AABB Collider (axis aligned bounding box)
class BoxCollider
{
private:
	v3t_float m_size;
	v3t_float m_topLeft;
	float m_xOffset, m_yOffset, m_zOffset;
	GameObject* m_objTransform;

public:
	bool isStatic;		// set it to true if the object does not move to skip unnessacery calculations
	bool isTrigger;		// disable physics if true

	// constructor
	BoxCollider(GameObject* transform, float width, float height, float depth, D3DXVECTOR3 offset = {0, 0, 0}, bool isStatic = false, bool isTrigger = false) :
		m_objTransform(transform), isStatic(isStatic), isTrigger(isTrigger), m_xOffset(offset.x), m_yOffset(offset.y), m_zOffset(offset.z)
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
		return v3t_float((-m_size.x / 2) + m_objTransform->GetCombinedPosition().x + m_xOffset,
			(m_size.y / 2) + m_objTransform->GetCombinedPosition().y + m_yOffset,
			(-m_size.z / 2) + m_objTransform->GetCombinedPosition().z + m_zOffset);
	}

	// calculate collision between two box colliders
	bool CheckCollision(const BoxCollider& other);

	// draw the collider on screen
	static void DrawCollider(const BoxCollider& col);
};
