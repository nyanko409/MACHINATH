#include "sceneManagement.h"
#include <Windows.h>
#include <string>
#include "font.h"
#include "Title.h"
#include "sprite.h"
#include "texture.h"
#include "input.h"
#include "common.h"


static int width, height;
static float TitleScale = 1.0F;
static Sprite titleSprite;

void InitTitle()
{
	width = Texture_GetWidth(TEXTURE_INDEX_TITLE);
	height = Texture_GetHeight(TEXTURE_INDEX_TITLE);

	titleSprite = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE), D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE) / 2, 0),
		0, D3DXVECTOR2(TitleScale, TitleScale), D3DCOLOR_RGBA(255, 255, 255, 255));
}

void UpdateTitle()
{
	if (Keyboard_IsPress(DIK_SPACE))
	{
		SetScene(SCENE_TITLESCREEN);
	}
}

void DrawTitle()
{
	SpriteDraw(titleSprite);
}

void FinalizeTitle()
{

}