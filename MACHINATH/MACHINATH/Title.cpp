#include <Windows.h>
#include <string>
#include "sceneManagement.h"
#include "font.h"
#include "Title.h"
#include "sprite.h"
#include "texture.h"
#include "input.h"
#include "common.h"
#include "sound.h"

#define COLOR_CHANGESPEED 2

static Sprite g_titleback;
static Sprite g_titleeye;
static Sprite g_titleforward;
static Sprite g_titleforward2;
static Sprite g_titlefadescreen;
static int fadecnt;
static int titleforwardcnt;
static bool nextsceneGO;
static int eyeRed, eyeGreen, eyeBlue;

ColorFade g_color;


void InitTitleScreen()
{
	//init flag 
	nextsceneGO = false;
	
	//PlaySound(SOUND_LABEL_BGM_TITLE);
	//SetVolume(SOUND_LABEL_BGM_TITLE , 0);
	//StartFade(SOUND_LABEL_BGM_TITLE);

	//init eye color
	g_color = COLOR_BLUE_IN;

	//init cnt
	fadecnt = 255;
	titleforwardcnt = 60;
	eyeRed = eyeGreen = eyeBlue = 0;

	// init title sprite
	g_titleback = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE_BACK), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE_BACK) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE_BACK) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_titleeye = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE_EYE), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE_EYE) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE_EYE) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_titleforward = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE_FORWARD), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE_FORWARD) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE_FORWARD) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_titleforward2 = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE_FORWARD2), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE_FORWARD2) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE_FORWARD2) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_titlefadescreen = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE_BACK), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE_BACK) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE_BACK) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));
}

void UpdateTitleScreen()
{
	if (!nextsceneGO)
	{
		// set sound volume
		//UpdateFadeSound(SOUND_LABEL_BGM_TITLE, 1, 5);

		// decrease fadecnt
		if (fadecnt > 0)
			fadecnt--;
	}
	else
	{
		//UpdateFadeSound(SOUND_LABEL_BGM_TITLE, 0, 3);

		//increase fadecnt
		if (fadecnt < 255)
			fadecnt++;
	}

	// control titleforward
	if (titleforwardcnt > 0)
	{
		titleforwardcnt--;
	}
	else
	{
		titleforwardcnt = 60;
	}

	// change color of eye
	switch (g_color)
	{
	case COLOR_BLUE_IN:
		eyeBlue++;
		if (eyeBlue == 255)
			g_color = COLOR_BLUE_OUT;
		break;
	
	case COLOR_BLUE_OUT:
		eyeBlue--;
		if (eyeBlue == 0)
			g_color = COLOR_YELLOW_IN;
		break;

	case COLOR_YELLOW_IN:
		eyeGreen++; eyeRed++;
		if (eyeGreen == 255)
			g_color = COLOR_YELLOW_OUT;
		break;

	case COLOR_YELLOW_OUT:
		eyeRed--; eyeGreen--;
		if (eyeRed == 0)
			g_color = COLOR_WHITE_IN;
		break;

	case COLOR_WHITE_IN:
		eyeRed++; eyeGreen++; eyeBlue++;
		if (eyeRed == 255)
			g_color = COLOR_WHITE_OUT;
		break;
		
	case COLOR_WHITE_OUT:
		eyeRed--; eyeGreen--; eyeBlue--;
		if (eyeRed == 0)
			g_color = COLOR_GREEN_IN;
		break;

	case COLOR_GREEN_IN:
		eyeGreen++;
		if (eyeGreen == 255)
			g_color = COLOR_GREEN_OUT;
		break;

	case COLOR_GREEN_OUT:
		eyeGreen--;
		if (eyeGreen == 0)
			g_color = COLOR_RED_IN;
		break;

	case COLOR_RED_IN:
		eyeRed++;
		if (eyeRed == 255)
			g_color = COLOR_RED_OUT;
		break;

	case COLOR_RED_OUT:
		eyeRed--;
		if (eyeRed == 0)
			g_color = COLOR_BLUE_IN;
		break;
	
	default: break;
	}

	// start fade when key is pressed
	if (!nextsceneGO && Keyboard_IsTrigger(DIK_SPACE))
	{
		//StartFade(SOUND_LABEL_BGM_TITLE);
		nextsceneGO = true;
	}

	// switch to next scene after fading
	if (nextsceneGO && fadecnt >= 255)
	{
		SetScene(SCENE_GAMESCREEN);
		nextsceneGO = false;
	}
}

void DrawTitleScreen()
{
	// draw title screen
	SpriteDraw(g_titleback);
	SpriteDraw(g_titleeye);

	if (titleforwardcnt > 30)
	{
		SpriteDraw(g_titleforward);
	}
	else
	{
		SpriteDraw(g_titleforward2);
	}

	SpriteDraw(g_titlefadescreen); 

	g_titleforward.color = g_titleforward2.color = D3DCOLOR_RGBA(rand() % 61 + 1, rand() % 122 + 122, 60, 255);
	g_titleeye.color = D3DCOLOR_RGBA(eyeRed, eyeGreen,eyeBlue, 255);
	g_titlefadescreen.color = D3DCOLOR_RGBA(255, 255, 255, fadecnt);
}

void FinalizeTitleScreen()
{

}
