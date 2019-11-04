#pragma once

#include <d3d9.h>
#include <d3dx9.h>

// name of the effect
enum Effect
{
	EFFECT_EXPLOSION,
	EFFECT_PICKUP_MEDAL,
	EFFECT_FUCK,
	EFFECT_SHIELD,
	EFFECT_BLOW
};

// call once at init
void InitEffect();

// call once at uninit
void UninitEffect();

// play effect
void PlayEffect(Effect type, float posX, float posY, float posZ, float deltaX = 0, float deltaY = 0, float deltaZ = 0, float playSpeed = 1.0F);
void PlayEffect(Effect type, D3DXVECTOR3 position, D3DXVECTOR3 delta = {0, 0, 0}, float speed = 1.0F);

// call every draw frame
void DrawEffect();
