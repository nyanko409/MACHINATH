#pragma once

#include <d3d9.h>
#include <d3dx9.h>

D3DXMATRIX TransformObject(D3DXVECTOR3 translate, D3DXVECTOR3 rotate, D3DXVECTOR3 scale, bool rotateAtPosition = true);
