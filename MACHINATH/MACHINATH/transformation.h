#pragma once

#include <d3d9.h>
#include <d3dx9.h>

D3DXMATRIX TransformSprite(D3DXVECTOR3 translate, float rotZ, D3DXVECTOR2 scale = {1.0F, 1.0F}, bool rotateAtPosition = true);

D3DXMATRIX TransformObject(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 rot, D3DXVECTOR3 localRot, D3DXVECTOR3 pivot);
