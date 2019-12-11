#include <Windows.h>
#include <string>
#include "font.h"
#include "score.h"
#include "sprite.h"
#include "texture.h"

static float ScoreScale = 1.0F;
static int score;
static Sprite A;

void AddScore(int sc)
{
	score += sc;
}



void InitScore()
{
	score = 0;

	A = Sprite(Texture_GetTexture(TEXTURE_INDEX_SCORE), D3DXVECTOR3(200, 200, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_SCORE) / 2, Texture_GetHeight(TEXTURE_INDEX_SCORE) / 2, 0),
		0, D3DXVECTOR2(ScoreScale, ScoreScale), D3DCOLOR_RGBA(255, 255, 255, 255));
}

void DrawScore()
{
	// draw text
	std::string str = std::to_string(score);
	RECT ScoreRect;
	SpriteDraw(A);
	DrawTextTo(RECT{ 200, 100, 100, 50 }, str.c_str(), str.length());

	/*do
	{
		//表示する位の数字を取り出す
		int n = s % 10;
		DrawSprite(x, 400, 80, 80,
			n % 5 * 0.2f,//u座標
			n / 5 * 0.2f,//v座標
			0.2f, 0.2f,
			MakeFloat4(1, 1, 1, 1));

		//次の位を表示するため10で割る
		s /= 10;
		//表示位置を左にずらす
		x -= 40.0f;

		fig++;
	} while (fig < FIGURE_NUM);*/
}
