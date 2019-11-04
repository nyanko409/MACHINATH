#include "sprite.h"
#include "mydirect3d.h"


// sprite device
static LPD3DXSPRITE sprite;



void SpriteDraw(const Sprite& sp)
{
	sprite->Draw(sp.sprite, NULL, &sp.center, &sp.position, sp.color);
}

void InitSprite()
{
	auto device = MyDirect3D_GetDevice();
	D3DXCreateSprite(device, &sprite);
}

void SpriteStart()
{
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void SpriteEnd()
{
	sprite->End();
}
