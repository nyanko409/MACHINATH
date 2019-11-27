#pragma once

#include <d3dx9.h>
#include "common.h"

class Transform
{
public:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 localRotation;
	D3DXVECTOR3 scale;

	// constructor
	Transform(D3DXVECTOR3 position = { 0, 0, 0 }, D3DXVECTOR3 rotation = { 0, 0, 0 }, D3DXVECTOR3 localRot = {0, 0, 0}, D3DXVECTOR3 scale = { 1, 1, 1 }) :
		position(position), rotation(rotation), localRotation(localRot), scale(scale) {}

	// destructor
	~Transform() {}
};
