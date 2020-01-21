#include "common.h"
#include "edge.h"
#include "texture.h"
#include "sprite.h"


static Sprite g_edgeLeft, g_edgeRight, g_edgeTop, g_edgeBottom;
static int g_fadeSpeed, g_count, g_alpha;
static int g_r, g_g, g_b;
static bool g_flip;


void InitEdge()
{
	g_edgeLeft = Sprite(Texture_GetTexture(TEXTURE_INDEX_EDGELEFT),
		{ 0,0,0 }, { 0,0,0 }, 0, { 1,1 });
	g_edgeRight = Sprite(Texture_GetTexture(TEXTURE_INDEX_EDGELEFT),
		{ SCREEN_WIDTH ,0,0 }, { 0,0,0 }, 0, { -1,1 });
	g_edgeTop = Sprite(Texture_GetTexture(TEXTURE_INDEX_EDGETOP),
		{ 0,0,0 }, { 0,0,0 }, 0, { 1,1 });
	g_edgeBottom = Sprite(Texture_GetTexture(TEXTURE_INDEX_EDGETOP),
		{ 0,SCREEN_HEIGHT,0 }, { 0,0,0 }, 0, { 1,-1 });

	g_edgeLeft.color.a = 0;
	g_edgeRight.color.a = 0;
	g_edgeTop.color.a = 0;
	g_edgeBottom.color.a = 0;
}

void UpdateEdge()
{
	if (g_count <= 0) return;

	// increase alpha by fadespeed
	g_alpha += g_flip ? -g_fadeSpeed : g_fadeSpeed;

	// clamp and flip
	if (!g_flip && g_alpha >= 255)
	{
		g_alpha = 255;
		g_flip = true;
	}
	if (g_flip && g_alpha <= 0)
	{
		g_alpha = 0;
		g_flip = false;
		--g_count;
	}

	// update sprite color
	g_edgeLeft.color = g_edgeRight.color = g_edgeTop.color = g_edgeBottom.color = 
		D3DCOLOR_RGBA(g_r, g_g, g_b, g_alpha);
}

void UninitEdge()
{

}

void DrawEdge()
{
	if (g_count <= 0) return;

	// draw edge fade
	SpriteDraw(g_edgeLeft);
	SpriteDraw(g_edgeRight);
	SpriteDraw(g_edgeTop);
	SpriteDraw(g_edgeBottom);
}

void FadeEdge(int count, int fadeSpeed, int r, int g, int b)
{
	g_r = r;
	g_g = g;
	g_b = b;

	g_alpha = 0;
	g_flip = false;
	g_fadeSpeed = fadeSpeed;
	g_count = count;
}
