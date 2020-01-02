#pragma once

#include <d3dx9.h>


// name of the effect
enum Effect
{
	EFFECT_JUMP
};

// call once at init
void InitEffect();

// call once at uninit
void UninitEffect();

// play given effect
void PlayEffect(Effect type, D3DXVECTOR3 position, 
	D3DXVECTOR3 deltaPos = { 0, 0, 0 }, D3DXVECTOR3 rotation = {0, 0, 0}, 
	D3DXVECTOR3 scale = {1, 1, 1}, float playSpeed = 1.0F);

// call every draw frame
void DrawEffect();
