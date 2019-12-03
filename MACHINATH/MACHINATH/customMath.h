#pragma once

#include <d3dx9.h>

// returns the distance between two points
float GetDistance(D3DXVECTOR3 t1, D3DXVECTOR3 t2, bool ignoreYAxis = false);
