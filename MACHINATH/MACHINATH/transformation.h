#pragma once

#include <d3d9.h>
#include <d3dx9.h>

D3DXMATRIX TransformSprite(D3DXVECTOR3 translate, float rotZ, D3DXVECTOR2 scale = {1.0F, 1.0F}, bool rotateAtPosition = true);

D3DXMATRIX TransformObject(D3DXVECTOR3 translate, D3DXVECTOR3 rotate, D3DXVECTOR3 scale = {1.0F, 1.0F, 1.0F}, bool rotateAtPosition = true);
