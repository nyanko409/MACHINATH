#pragma once

#include <d3dx9.h>

class GameObject;

// shader types
enum SHADER_TYPE
{
	SHADER_WATER,
	SHADER_FOG,

	SHADER_DEFAULT
};

// assigns the given object to shader
LPD3DXEFFECT AssignShader(GameObject* obj, SHADER_TYPE type);

// deassigns the given shader
void DeassignShader(GameObject* obj, SHADER_TYPE type);


#include "gameObject.h"

// call once at init
void InitShader();

// call once at uninit
void UninitShader();

// call every frame to draw
void DrawObjects();
