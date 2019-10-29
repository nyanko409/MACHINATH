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
void PlayEffect(Effect effect, float posX, float posY, float posZ);
void PlayEffect(Effect effect, D3DXVECTOR3 position);

// call every draw frame
void DrawEffect();
