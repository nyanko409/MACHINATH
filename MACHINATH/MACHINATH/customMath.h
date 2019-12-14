#pragma once

#include <d3dx9.h>

// returns the distance between two points
float GetDistance(D3DXVECTOR3 t1, D3DXVECTOR3 t2, bool ignoreYAxis = false);

// if t == 0 return a, if t == 1 return b, if t == 0.5 return midpoint of a and b
D3DXVECTOR3 Lerp(D3DXVECTOR3 a, D3DXVECTOR3 b, float t);
