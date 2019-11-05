#pragma once

#include <d3dx9.h>


void InitShader();
void UninitShader();

void EffectStart();
void EffectEnd();

LPD3DXEFFECT GetEffect();
