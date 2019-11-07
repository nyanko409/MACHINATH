#pragma once

class GameObject;

// shader types
enum SHADER_TYPE
{
	SHADER_SIMPLE,


	SHADER_DEFAULT
};

// assigns the given object to shader
void AssignShader(GameObject* obj, SHADER_TYPE type);

// deassigns the given shader
void DeassignShader(GameObject* obj, SHADER_TYPE type);


#include <d3dx9.h>
#include "gameObject.h"



// call once at init
void InitShader();

// call once at uninit
void UninitShader();

// call every frame to draw
void DrawObjects();
