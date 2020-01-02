#include <Windows.h>
#include <string>
#include "sceneManagement.h"
#include "font.h"
#include "Result.h"
#include "sprite.h"
#include "texture.h"
#include "input.h"
#include "common.h"
#include "sound.h"

static Sprite g_resultscore;


void InitResult()
{

	// init result sprite
	g_resultscore = Sprite(Texture_GetTexture(TEXTURE_INDEX_SCORE), D3DXVECTOR3(400, 200, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_SCORE) / 2, Texture_GetHeight(TEXTURE_INDEX_SCORE) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));
}

void UpdateResult()
{
	// switch to game screen when key is pressed
	//if (Keyboard_IsPress(DIK_SPACE))
	//{
	//	SetScene(SCENE_TITLESCREEN);
	//}

	//UpdateFadeSound(SOUND_LABEL_BGM_TITLE, 0.0f, 10.0f);
}

void DrawResult()
{
	
}

void FinalizeResult()
{

}
