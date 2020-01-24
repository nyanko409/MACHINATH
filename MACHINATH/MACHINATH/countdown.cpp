#include <d3dx9.h>
#include "countdown.h"
#include "texture.h"
#include "common.h"
#include "sprite.h"


enum CountState
{
	CountOne, CountTwo, CountThree, CountGo, CountDone
};


static Sprite g_count1, g_count2, g_count3, g_countGo;
static CountState state;


void StartCountdown()
{
	state = CountThree;
	g_count1.color.a = g_count2.color.a = g_count3.color.a = g_countGo.color.a = 1;
}


void InitCountdown()
{
	state = CountDone;

	g_count1 = Sprite(Texture_GetTexture(TEXTURE_INDEX_COUNT1), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_COUNT1) / 2, Texture_GetHeight(TEXTURE_INDEX_COUNT1) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_count2 = Sprite(Texture_GetTexture(TEXTURE_INDEX_COUNT2), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_COUNT2) / 2, Texture_GetHeight(TEXTURE_INDEX_COUNT2) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_count3 = Sprite(Texture_GetTexture(TEXTURE_INDEX_COUNT3), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_COUNT3) / 2, Texture_GetHeight(TEXTURE_INDEX_COUNT3) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_countGo = Sprite(Texture_GetTexture(TEXTURE_INDEX_COUNTGO), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_COUNTGO) / 2, Texture_GetHeight(TEXTURE_INDEX_COUNTGO) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));
}

void UpdateCountdown()
{
	switch (state)
	{
	case CountThree:
		g_count3.color.a -= 0.01F;
		g_count3.position.x += 5;
		if (g_count3.color.a <= 0)
		{
			g_count3.color.a = 0;
			state = CountTwo;
		}
		break;
	case CountTwo:
		g_count2.color.a -= 0.01F;
		if (g_count2.color.a <= 0)
		{
			g_count2.color.a = 0;
			state = CountOne;
		}
		break;
	case CountOne:
		g_count1.color.a -= 0.01F;
		if (g_count1.color.a <= 0)
		{
			g_count1.color.a = 0;
			state = CountGo;
		}
		break;
	case CountGo:
		g_countGo.color.a -= 0.01F;
		if (g_countGo.color.a <= 0)
		{
			g_countGo.color.a = 0;
			state = CountDone;
		}
		break;
	default:
		break;
	}
}


void DrawCountdown()
{
	switch (state)
	{
	case CountThree:
		SpriteDraw(g_count3);
		break;
	case CountTwo:
		SpriteDraw(g_count2);
		break;
	case CountOne:
		SpriteDraw(g_count1);
		break;
	case CountGo:
		SpriteDraw(g_countGo);
		break;
	default:
		break;
	}
}

void UninitCountdown()
{

}
