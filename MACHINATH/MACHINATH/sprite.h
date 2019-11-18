#pragma once

#include <d3dx9.h>

class Sprite
{
public:
	LPDIRECT3DTEXTURE9 sprite;

	D3DXVECTOR3 position;
	D3DXVECTOR2 scale;
	D3DXVECTOR3 center;

	float rotZ;

	D3DXCOLOR color;

	Sprite(LPDIRECT3DTEXTURE9 sprite, D3DXVECTOR3 position = {0, 0, 0}, D3DXVECTOR3 center = {0, 0, 0}, float rotZ = 0,
		D3DXVECTOR2 scale = {1, 1},	D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255)) :
		sprite(sprite), position(position), scale(scale), rotZ(rotZ), center(center), color(color) {}

	Sprite() {}
	~Sprite() {}
};


// draw sprite
void SpriteDraw(const Sprite& sp, bool rotateAtPos = true);

// call once at start
void InitSprite();

// call every frame before drawing sprites
void SpriteStart();

// call every frame after drawing sprites
void SpriteEnd();
