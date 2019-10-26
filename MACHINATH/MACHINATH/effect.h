#pragma once

// name of the effect
enum Effect
{
	EFFECT_EXPLOSION,
	EFFECT_SWORD
};

// call once at init
void InitEffect();

// call once at uninit
void UninitEffect();

// play effect
void PlayEffect(Effect effect, float posX, float posY, float posZ);

// call every draw frame
void DrawEffect();
