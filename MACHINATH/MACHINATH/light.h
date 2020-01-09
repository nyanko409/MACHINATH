#pragma once

#include <d3dx9.h>

// add a new directional light to the scene
void AddDirectionalLight(int index, D3DXVECTOR3 direction, D3DXCOLOR diffuse);

// add a new spot light to the scene
void AddSpotLight(int index, D3DXVECTOR3 position, float range, D3DXCOLOR diffuse);

// add a new point light to the scene
void AddPointLight(int index, D3DXVECTOR3 position, float range, D3DXCOLOR diffuse);

/*  move existing light in the scene
	aboslute = move the light to given position
	!aboslute = move the light by given value      */
void MoveLight(int index, float x, float y, float z, bool absolute);

/*  rotate existing light in the scene
	aboslute = rotate the light to given rotation
	!aboslute = rotate the light by given value      */
void RotateLight(int index, float x, float y, float z, bool absolute);

// activate or deactivate light
void ActivateLight(int index, bool enable);
