#include "fade.h"
#include "common.h"
#include "texture.h"
#include "sprite.h"


enum FadeState
{
	FadeIn, FadeOut, FadeNone
};

static Sprite g_fade;
static FadeState g_state;
static SceneType g_fadeScene;


void InitFade()
{ 
	g_fade = Sprite(Texture_GetTexture(TEXTURE_INDEX_BLACK), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_BLACK) / 2, Texture_GetHeight(TEXTURE_INDEX_BLACK) / 2, 0),
		0, D3DXVECTOR2(200, 200), D3DCOLOR_RGBA(255, 255, 255, 0));

	g_state = FadeNone;
}

void UninitFade()
{

}

void UpdateFade()
{
	if (g_state == FadeNone) return;

	if (g_state == FadeIn)
	{
		// fade in
		g_fade.color.a += 0.01F;
		if (g_fade.color.a >= 1)
		{
			g_fade.color.a = 1;
			g_state = FadeOut;

			// switch to next scene
			SetScene(g_fadeScene);
		}
	}
	else
	{
		// fade out
		g_fade.color.a -= 0.01F;
		if (g_fade.color.a <= 0)
		{
			g_fade.color.a = 0;
			g_state = FadeNone;
		}
	}
}

void DrawFade()
{
	if (g_state == FadeNone) return;

	// draw the fade sprite
	SpriteDraw(g_fade);
}

bool StartFadeToScene(SceneType scene)
{
	// return if already fading
	if (g_state != FadeNone) return false;

	// init fade
	g_fadeScene = scene;
	g_state = FadeIn;
	g_fade.color.a = 0;
	return true;
}
