#include <Windows.h>
#include <string>
#include "sceneManagement.h"
#include "font.h"
#include "Title.h"
#include "fade.h"
#include "sprite.h"
#include "texture.h"
#include "input.h"
#include "common.h"
#include "sound.h"

#define COLOR_CHANGESPEED 3
#define ANIMATION_SPAN 5
#define ANIMATION_PERIOD 200

static Sprite g_titleeye;
static Sprite g_titleforward;
static Sprite g_titleforward2;
static Sprite g_titleAnime1;
static Sprite g_titleAnime2;
static Sprite g_titleAnime3;
static Sprite g_titleSuna1;
static Sprite g_titleSuna2;

static int titleforwardcnt;
static int g_Animecnt;
static int eyeRed, eyeGreen, eyeBlue;

ColorFade g_color;

void InitTitleScreen()
{
	//init  
	g_color = COLOR_BLUE_IN;
	titleforwardcnt = 60;
	g_Animecnt = ANIMATION_PERIOD;
	eyeRed = eyeGreen = eyeBlue = 0;

	// init title sprites(MainSprites)
	g_titleeye = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE_EYE), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE_EYE) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE_EYE) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_titleforward = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE_FORWARD), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE_FORWARD) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE_FORWARD) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_titleforward2 = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE_FORWARD2), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE_FORWARD2) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE_FORWARD2) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));

	// init title sprites(Sprites for Animation)
	g_titleAnime1 = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE_ANIME1), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE_ANIME1) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE_ANIME1) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_titleAnime2 = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE_ANIME2), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE_ANIME2) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE_ANIME2) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_titleAnime3 = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE_ANIME3), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE_ANIME3) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE_ANIME3) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_titleSuna1 = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE_SUNA1), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE_SUNA1) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE_SUNA1) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_titleSuna2 = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE_SUNA2), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE_SUNA2) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE_SUNA2) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));



	// play title bgm
	PlaySound(AUDIO_BGM_TITLE);
}

void UpdateTitleScreen()
{
	// control titleforward
	if (titleforwardcnt > 0)
	{
		titleforwardcnt--;
	}
	else
	{
		titleforwardcnt = 60;
	}
	// control span of animation
	if (g_Animecnt > 0)
	{
		g_Animecnt--;
		if (g_Animecnt == ANIMATION_SPAN * 5)
		{
			PlaySound(AUDIO_SE_TITLE_NOISE);
		}
	}
	else
	{
		StopSound(AUDIO_SE_TITLE_NOISE);
		g_Animecnt = rand()% ANIMATION_PERIOD +100;
	}

	// change color of eye
	switch (g_color)
	{
	case COLOR_BLUE_IN:
		eyeBlue += COLOR_CHANGESPEED;
		if (eyeBlue == 255)
			g_color = COLOR_BLUE_OUT;
		break;
	
	case COLOR_BLUE_OUT:
		eyeBlue-= COLOR_CHANGESPEED;
		if (eyeBlue == 0)
			g_color = COLOR_YELLOW_IN;
		break;

	case COLOR_YELLOW_IN:
		eyeGreen += COLOR_CHANGESPEED;  eyeRed += COLOR_CHANGESPEED;
		if (eyeGreen == 255)
			g_color = COLOR_YELLOW_OUT;
		break;

	case COLOR_YELLOW_OUT:
		eyeRed-= COLOR_CHANGESPEED; eyeGreen-= COLOR_CHANGESPEED;
		if (eyeRed == 0)
			g_color = COLOR_WHITE_IN;
		break;

	case COLOR_WHITE_IN:
		eyeRed+= COLOR_CHANGESPEED; eyeGreen+= COLOR_CHANGESPEED; eyeBlue+= COLOR_CHANGESPEED;
		if (eyeRed == 255)
			g_color = COLOR_WHITE_OUT;
		break;
		
	case COLOR_WHITE_OUT:
		eyeRed-= COLOR_CHANGESPEED; eyeGreen-= COLOR_CHANGESPEED; eyeBlue-= COLOR_CHANGESPEED;
		if (eyeRed == 0)
			g_color = COLOR_GREEN_IN;
		break;

	case COLOR_GREEN_IN:
		eyeGreen+= COLOR_CHANGESPEED;
		if (eyeGreen == 255)
			g_color = COLOR_GREEN_OUT;
		break;

	case COLOR_GREEN_OUT:
		eyeGreen-= COLOR_CHANGESPEED;
		if (eyeGreen == 0)
			g_color = COLOR_RED_IN;
		break;

	case COLOR_RED_IN:
		eyeRed+= COLOR_CHANGESPEED;
		if (eyeRed == 255)
			g_color = COLOR_RED_OUT;
		break;

	case COLOR_RED_OUT:
		eyeRed-= COLOR_CHANGESPEED;
		if (eyeRed == 0)
			g_color = COLOR_BLUE_IN;
		break;
	
	default: break;
	}

	// fade to game screen
	if (Keyboard_IsTrigger(DIK_SPACE) && StartFadeToScene(SCENE_GAMESCREEN))
	{
		StopSound(AUDIO_SE_TITLE_NOISE);
		PlaySound(AUDIO_SE_TITLE_PUSHBUTTON , 0.2f);
		StartFade(AUDIO_BGM_TITLE, 0, 3.0F);

	}
}

void DrawTitleScreen()
{
	// draw title screen
	SpriteDraw(g_titleeye);


	//animation
	if (g_Animecnt > 0 && g_Animecnt < ANIMATION_SPAN)
	{
		//SpriteDraw(g_titleSuna1);
		
		SpriteDraw(g_titleAnime1);
	}
	else if (g_Animecnt > ANIMATION_SPAN && g_Animecnt < ANIMATION_SPAN*2)
	{
		//SpriteDraw(g_titleSuna2);

		SpriteDraw(g_titleAnime2);
	}
	else if (g_Animecnt > ANIMATION_SPAN*2 && g_Animecnt < ANIMATION_SPAN*3)
	{
		SpriteDraw(g_titleAnime3);
	}
	else if (g_Animecnt > ANIMATION_SPAN * 3 && g_Animecnt < ANIMATION_SPAN * 4)
	{
		SpriteDraw(g_titleAnime2);
	}
	else if (g_Animecnt > ANIMATION_SPAN * 4 && g_Animecnt < ANIMATION_SPAN * 5)
	{
		SpriteDraw(g_titleAnime1);
	}
	else
	{

		if (titleforwardcnt > 30)
		{
			SpriteDraw(g_titleforward);
		}
		else
		{
			SpriteDraw(g_titleforward2);
		}
	}

	//control color of sprites
	g_titleAnime1.color = g_titleAnime2.color = g_titleAnime3.color=
	g_titleforward.color =  g_titleforward2.color = D3DCOLOR_RGBA(rand() % 61 + 1, rand() % 122 + 122, 60, 255);
	g_titleeye.color = D3DCOLOR_RGBA(eyeRed, eyeGreen,eyeBlue, 255);
	
	g_titleSuna1.color= g_titleSuna2.color = D3DCOLOR_RGBA(255,255,255,50);
}

void FinalizeTitleScreen()
{

}
