#include <Windows.h>
#include <string>
#include "sceneManagement.h"
#include "font.h"
#include "Title.h"
#include "sprite.h"
#include "texture.h"
#include "input.h"
#include "common.h"


static Sprite titleSprite;


void InitTitle()
{
	// init title sprite
	titleSprite = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE), D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE) / 2, 0),
		0, D3DXVECTOR2(1.92f/2.0f, 1.08f/2.0f), D3DCOLOR_RGBA(255, 255, 255, 255));
}

void UpdateTitle()
{
	// switch to game screen when key is pressed
	if (Keyboard_IsPress(DIK_SPACE))
	{
		SetScene(SCENE_GAMESCREEN);
	}
}

void DrawTitle()
{
	// draw if in title screen
	if (GetScene() == SCENE_TITLESCREEN)
	{
		SpriteDraw(titleSprite);
	}
}

void FinalizeTitle()
{

}
