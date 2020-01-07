#pragma once

#include <d3dx9.h>


// used for transforming sprites
D3DXMATRIX TransformSprite(D3DXVECTOR3 translate, float rotZ, D3DXVECTOR2 scale = {1.0F, 1.0F}, bool rotateAtPosition = true);

// used for transforming objects
D3DXMATRIX TransformObject(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 rot,
	D3DXMATRIX& matOrientation, D3DXMATRIX matCombinedOrientation, D3DXVECTOR3 worldRot,
	D3DXVECTOR3& forward, D3DXVECTOR3& up, D3DXVECTOR3& right, D3DXVECTOR3 pivot);
