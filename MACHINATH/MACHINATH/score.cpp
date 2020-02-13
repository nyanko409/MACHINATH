#include <Windows.h>
#include <string>
#include "font.h"
#include "score.h"
#include "sprite.h"
#include "texture.h"
#include "sceneManagement.h"
#include "playTime.h"


#define RANKING_COUNT 3
#define FIGURE_NUM 6
#define SCORE_SPEED 10.0f

static int g_score;
static Sprite g_scoreSprite;
static Sprite g_placement[3];
static int g_finalScore;
static int g_ranking[RANKING_COUNT];
static bool dispRanking;


void AddScore(int value)
{
	g_finalScore += value;
}

void ShowRanking()
{
	dispRanking = true;

	// load score from text data
	for (int i = 0; i < RANKING_COUNT; ++i)
	{
		g_ranking[i] = (i + 1) * 123456;
	}
}


void InitScore()
{
	g_score = 0;
	g_finalScore = 0;
	dispRanking = false;
	ShowRanking();

	g_scoreSprite = Sprite(Texture_GetTexture(TEXTURE_INDEX_SCORE), D3DXVECTOR3(400, 200, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_SCORE) / 2, Texture_GetHeight(TEXTURE_INDEX_SCORE) / 2, 0),
		0, D3DXVECTOR2(0.5f, 0.5f), D3DCOLOR_RGBA(255,255,255, 255));

	g_placement[0] = Sprite(Texture_GetTexture(TEXTURE_INDEX_FIRST), D3DXVECTOR3(400, 200, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_FIRST) / 2, Texture_GetHeight(TEXTURE_INDEX_FIRST) / 2, 0),
		0, D3DXVECTOR2(0.5f, 0.5f), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_placement[1] = Sprite(Texture_GetTexture(TEXTURE_INDEX_SECOND), D3DXVECTOR3(400, 200, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_SECOND) / 2, Texture_GetHeight(TEXTURE_INDEX_SECOND) / 2, 0),
		0, D3DXVECTOR2(0.5f, 0.5f), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_placement[2] = Sprite(Texture_GetTexture(TEXTURE_INDEX_THIRD), D3DXVECTOR3(400, 200, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_THIRD) / 2, Texture_GetHeight(TEXTURE_INDEX_THIRD) / 2, 0),
		0, D3DXVECTOR2(0.5f, 0.5f), D3DCOLOR_RGBA(255, 255, 255, 255));
}

void UpdateScore()
{
	if (g_score < g_finalScore)
	{
		g_score += SCORE_SPEED;
	}
	else
	{
		g_score = g_finalScore;
	}
}

void DrawScore()
{
	int width = Texture_GetWidth(TEXTURE_INDEX_SCORE);
	int height = Texture_GetHeight(TEXTURE_INDEX_SCORE);

	if (true)
	{
		for (int i = 0; i < RANKING_COUNT; ++i)
		{
			RECT ScoreRect;
			g_scoreSprite.position.x = 1000.0f;
			g_scoreSprite.position.y = 200 + (180 * (i + 1));

			g_placement[i].position.x = g_scoreSprite.position.x - 600;
			g_placement[i].position.y = g_scoreSprite.position.y - 100;

			int fig = 0;
			int s = g_ranking[i];

			do
			{
				//表示する位の数字を取り出す
				int n = s % 10;

				ScoreRect.left = n % 5 * 0.2f * width;	//u座標
				ScoreRect.top = n / 5 * 0.5f * height;	//v座標
				ScoreRect.right = ScoreRect.left + 0.2f * width;
				ScoreRect.bottom = ScoreRect.top + 0.5f * height;

				//g_scoreSprite.color = D3DCOLOR_RGBA(rand() % 61 + 1, rand()%122 + 122, 60, 255);
				g_scoreSprite.color = D3DCOLOR_RGBA(0, 255, 0, 255);

				SpriteDraw(g_placement[i], true);
				SpriteDraw(g_scoreSprite, true, &ScoreRect);

				//次の位を表示するため10で割る
				s *= 0.1F;

				//表示位置を左にずらす
				g_scoreSprite.position.x -= 0.2F * width * 0.3F;

				fig++;
			} while (fig < FIGURE_NUM);
		}
	}

	else
	{
		RECT ScoreRect;
		g_scoreSprite.position.x = 600.0f;
		int fig = 0;
		int s = g_score;

		do
		{
			//表示する位の数字を取り出す
			int n = s % 10;

			ScoreRect.left = n % 5 * 0.2f * width;	//u座標
			ScoreRect.top = n / 5 * 0.5f * height;	//v座標
			ScoreRect.right = ScoreRect.left + 0.2f * width;
			ScoreRect.bottom = ScoreRect.top + 0.5f * height;

			//g_scoreSprite.color = D3DCOLOR_RGBA(rand() % 61 + 1, rand()%122 + 122, 60, 255);
			g_scoreSprite.color = D3DCOLOR_RGBA(0, 255, 0, 255);

			SpriteDraw(g_scoreSprite, true, &ScoreRect);

			//次の位を表示するため10で割る
			s *= 0.1F;

			//表示位置を左にずらす
			g_scoreSprite.position.x -= 0.2F * width * 0.3F;

			fig++;
		} while (fig < FIGURE_NUM);
	}
}
