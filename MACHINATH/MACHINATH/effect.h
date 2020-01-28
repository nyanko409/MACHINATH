#pragma once

#include "mydirect3d.h"
#include <Effekseer.h>
#include <EffekseerRendererDX9.h>


// name of the effect
enum Effect
{
	EFFECT_JUMP,
	EFFECT_GOLD,
	EFFECT_LANDING,
	EFFECT_QTESUCCESS,
	EFFECT_BOOST,
	EFFECT_AIR
};

// call once at init
void InitEffect();

// call once at uninit
void UninitEffect();

// play given effect and returns the handle of the effect
int PlayEffect(Effect type, D3DXVECTOR3 position, D3DXVECTOR3 rotation = {0, 0, 0}, 
		D3DXVECTOR3 scale = {1, 1, 1}, float playSpeed = 1.0F);

// update the effect of given handle
void UpdateEffect(int handle, D3DXVECTOR3 position, D3DXVECTOR3 rotation);

// call every frame
void DrawEffect();

// stop the effect of given handle
void StopEffect(int handle);

// stop all effect
void StopAllEffect();
