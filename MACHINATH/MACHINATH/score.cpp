#include <Windows.h>
#include <string>
#include "font.h"
#include "score.h"
#include "sprite.h"
#include "texture.h"


#define FIGURE_NUM 6
#define SCORE_SPEED 3.0f

static int width, height;
static float ScoreScale = 0.5F;
static int g_score;
static Sprite scoreSprite;
static int finalscore;


void AddScore(int value)
{
	finalscore += value;
}


void InitScore()
{
	width = Texture_GetWidth(TEXTURE_INDEX_SCORE);
	height = Texture_GetHeight(TEXTURE_INDEX_SCORE);
	g_score = 0;
	finalscore = 0;

	scoreSprite = Sprite(Texture_GetTexture(TEXTURE_INDEX_SCORE), D3DXVECTOR3(400, 200, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_SCORE) / 2, Texture_GetHeight(TEXTURE_INDEX_SCORE) / 2, 0),
		0, D3DXVECTOR2(ScoreScale, ScoreScale), D3DCOLOR_RGBA(255, 255, 255, 255));
}

void UpdateScore()
{
	if (g_score < finalscore)
	{
		g_score += SCORE_SPEED;
	}
	else
	{
		g_score = finalscore;
	}
}

void DrawScore()
{
	scoreSprite.position.x = 800.0f;
	int fig = 0;
	int s = g_score;

	// draw text
	std::string str = std::to_string(g_score);
	RECT ScoreRect;
	DrawTextTo(RECT{ 200, 100, 100, 50 }, str.c_str(), str.length());

	do
	{
		//表示する位の数字を取り出す
		int n = s % 10;

		ScoreRect.left = n % 5 * 0.2f * width;//u座標
		ScoreRect.top = n / 5 * 0.5f * height;//v座標
		ScoreRect.right = ScoreRect.left + 0.2f * width;
		ScoreRect.bottom = ScoreRect.top + 0.5f * height;

		SpriteDraw(scoreSprite,true,&ScoreRect);

		//次の位を表示するため10で割る
		s /= 10;

		//表示位置を左にずらす
		scoreSprite.position.x -= 0.2*width /2;

		fig++;
	} 
	while (fig < FIGURE_NUM);
}

