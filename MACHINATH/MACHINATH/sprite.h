#pragma once

#include <d3dx9.h>

class Sprite
{
public:
	LPDIRECT3DTEXTURE9 sprite;
	D3DXVECTOR3 position;
	D3DXVECTOR3 center;
	D3DCOLOR color;

	Sprite(LPDIRECT3DTEXTURE9 sprite, D3DXVECTOR3 position = {0, 0, 0}, D3DXVECTOR3 center = {0, 0, 0}, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255)) :
		sprite(sprite), position(position), center(center), color(color) {}

	Sprite() {}
	~Sprite() {}
};


// draw sprite
void SpriteDraw(const Sprite& sp);

// call once at start
void InitSprite();

// call every frame before drawing sprites
void SpriteStart();

// call every frame after drawing sprites
void SpriteEnd();
