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

static Sprite g_title;


void InitTitle()
{
	PlaySound(SOUND_LABEL_BGM_TITLE);

	// init title sprite
	g_title = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE), D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE) / 2 ,0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));
}

void UpdateTitle()
{
	// switch to game screen when key is pressed
	if (Keyboard_IsPress(DIK_SPACE))
	{
		StartFade(SOUND_LABEL_BGM_TITLE);
		SetScene(SCENE_GAMESCREEN);
	}
 
	//UpdateFadeSound(SOUND_LABEL_BGM_TITLE, 0.0f, 10.0f);
}

void DrawTitle()
{
	// draw if in title screen
	if (GetScene() == SCENE_TITLESCREEN)
	{
		SpriteDraw(g_title);
	}
}

void FinalizeTitle()
{

}
