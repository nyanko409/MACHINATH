#include <d3dx9.h>
#include "sound.h"
#include "countdown.h"
#include "texture.h"
#include "common.h"
#include "cameraevent.h"
#include "gate.h"
#include "sprite.h"
#include "player.h"


enum CountState
{
	GetReady, CountOne, CountTwo, CountThree, CountGo, CountDone
};


static Sprite g_count1, g_count2, g_count3, g_countGo, g_getReady;
static CountState state;


void StartCountdown()
{
	PlaySound(AUDIO_SE_GETREADY);
	state = GetReady;

	CameraEventData ced = { 360, 0.5F, 0, 0, -7, -0.1F };
	AddCameraEvent(ced);

	g_count1.color.a = g_count2.color.a = g_count3.color.a = 
		g_countGo.color.a =  g_getReady.color.a = 1;
	g_count1.scale.x = g_count2.scale.x = g_count3.scale.x =
		g_countGo.scale.x = g_getReady.scale.x =
	g_count1.scale.y = g_count2.scale.y = g_count3.scale.y =
		g_countGo.scale.y = g_getReady.scale.y = 2;

	g_getReady.scale.x = g_getReady.scale.y = 5;
}


void InitCountdown()
{
	state = CountDone;

	g_count1 = Sprite(Texture_GetTexture(TEXTURE_INDEX_COUNT1), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_COUNT1) / 2, Texture_GetHeight(TEXTURE_INDEX_COUNT1) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(0, 255, 0, 255));

	g_count2 = Sprite(Texture_GetTexture(TEXTURE_INDEX_COUNT2), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_COUNT2) / 2, Texture_GetHeight(TEXTURE_INDEX_COUNT2) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(0, 255, 0, 255));

	g_count3 = Sprite(Texture_GetTexture(TEXTURE_INDEX_COUNT3), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_COUNT3) / 2, Texture_GetHeight(TEXTURE_INDEX_COUNT3) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(0, 255, 0, 255));

	g_countGo = Sprite(Texture_GetTexture(TEXTURE_INDEX_COUNTGO), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_COUNTGO) / 2, Texture_GetHeight(TEXTURE_INDEX_COUNTGO) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(0, 255, 0, 255));

	g_getReady = Sprite(Texture_GetTexture(TEXTURE_INDEX_COUNTGETREADY), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_COUNTGETREADY) / 2, Texture_GetHeight(TEXTURE_INDEX_COUNTGETREADY) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(0, 255, 0, 255));
}

void UpdateCountdown()
{
	switch (state)
	{
	case GetReady:
		g_getReady.color.a -= 0.003F;
		if (g_getReady.color.a <= -1)
		{
			OpenGate();
			g_getReady.color.a = 0;
			state = CountThree;
			PlaySound(AUDIO_SE_THREE);
		}
		break;
	case CountThree:
		g_count3.color.a -= 0.01F;
		g_count3.scale.x += 0.01F;
		g_count3.scale.y += 0.01F;
		if (g_count3.color.a <= 0)
		{
			g_count3.color.a = 0;
			state = CountTwo;
			PlaySound(AUDIO_SE_TWO);
		}
		break;
	case CountTwo:
		g_count2.color.a -= 0.01F;
		g_count2.scale.x += 0.01F;
		g_count2.scale.y += 0.01F;
		if (g_count2.color.a <= 0)
		{
			g_count2.color.a = 0;
			state = CountOne;
			PlaySound(AUDIO_SE_ONE);
		}
		break;
	case CountOne:
		g_count1.color.a -= 0.01F;
		g_count1.scale.x += 0.01F;
		g_count1.scale.y += 0.01F;
		if (g_count1.color.a <= 0)
		{
			CameraEventData ced = { 0, 0, 0, 0, 7, 0.1F };
			AddCameraEvent(ced);

			g_count1.color.a = 0;
			state = CountGo;
			PlaySound(AUDIO_SE_GO);
			GetPlayer()->isMoving = true;
			SetLerpSpeed(0.1F);
		}
		break;
	case CountGo:
		g_countGo.color.a -= 0.01F;
		g_countGo.scale.x += 0.01F;
		g_countGo.scale.y += 0.01F;
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
	case GetReady:
		SpriteDraw(g_getReady);
		break;
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
