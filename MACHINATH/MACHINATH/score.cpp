#include <Windows.h>
#include <iostream>
#include <fstream>
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
static int g_finalScore;

static Sprite g_scoreSprite;
static Sprite g_placement[3];

static int g_ranking[RANKING_COUNT];
static bool dispRanking;

void LoadRankingFromFile(char* path);
void SaveRankingToFile(char* path);
void UpdateRanking();



void AddScore(int value)
{
	g_finalScore += value;
}

void ShowRanking()
{
	dispRanking = true;

	LoadRankingFromFile((char*)"asset/data/ranking.txt");
	UpdateRanking();
	SaveRankingToFile((char*)"asset/data/ranking.txt");
}


void InitScore()
{
	g_score = 0;
	g_finalScore = 0;
	dispRanking = false;

	g_scoreSprite = Sprite(Texture_GetTexture(TEXTURE_INDEX_SCORE), D3DXVECTOR3(400, 200, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_SCORE) / 2, Texture_GetHeight(TEXTURE_INDEX_SCORE) / 2, 0),
		0, D3DXVECTOR2(0.5f, 0.5f), D3DCOLOR_RGBA(255,255,255, 255));

	g_placement[0] = Sprite(Texture_GetTexture(TEXTURE_INDEX_FIRST), D3DXVECTOR3(400, 200, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_FIRST) / 2, Texture_GetHeight(TEXTURE_INDEX_FIRST) / 2, 0),
		0, D3DXVECTOR2(0.5f, 0.5f), D3DCOLOR_RGBA(212, 175, 55, 255));

	g_placement[1] = Sprite(Texture_GetTexture(TEXTURE_INDEX_SECOND), D3DXVECTOR3(400, 200, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_SECOND) / 2, Texture_GetHeight(TEXTURE_INDEX_SECOND) / 2, 0),
		0, D3DXVECTOR2(0.5f, 0.5f), D3DCOLOR_RGBA(192, 192, 192, 255));

	g_placement[2] = Sprite(Texture_GetTexture(TEXTURE_INDEX_THIRD), D3DXVECTOR3(400, 200, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_THIRD) / 2, Texture_GetHeight(TEXTURE_INDEX_THIRD) / 2, 0),
		0, D3DXVECTOR2(0.5f, 0.5f), D3DCOLOR_RGBA(205, 127, 50, 255));
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

	if (dispRanking)
	{
		for (int i = 0; i < RANKING_COUNT; ++i)
		{
			RECT ScoreRect;
			g_scoreSprite.position.x = 900.0f;
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


void UpdateRanking()
{
	int temp[3];
	temp[0] = g_ranking[0];
	temp[1] = g_ranking[1];
	temp[2] = g_ranking[2];

	if (g_score > g_ranking[0])
	{
		g_ranking[0] = g_score;
		g_ranking[1] = temp[0];
		g_ranking[2] = temp[1];
	}
	else if (g_score > g_ranking[1])
	{
		g_ranking[1] = g_score;
		g_ranking[2] = temp[1];
	}
	else if (g_score > g_ranking[2])
	{
		g_ranking[2] = g_score;
	}
}

void SaveRankingToFile(char* path)
{
	std::ofstream of(path);

	for (int i = 0; i < RANKING_COUNT; ++i)
	{
		of << g_ranking[i] << "\n";
	}

	of.close();
}

void LoadRankingFromFile(char* path)
{
	std::ifstream in(path);
	
	if (!in.is_open())
	{
		for (int i = 0; i < RANKING_COUNT; ++i)
		{
			g_ranking[i] = 0;
		}

		return;
	}

	int id = 0;
	char c;

	while (true)
	{
		try
		{
			// read next byte
			in.read(&c, 1);

			// break if end of file is reached
			if (in.eof())
			{
				in.close();
				break;
			}

			// if c == #, loop till next line to skip comment
			if (c == 35)
			{
				do
				{
					in.read(&c, 1);
					if (in.eof())
					{
						in.close();
						break;
					}
				}
				while (!(c == 10));
			}

			// continue if its not a number
			if (!(c >= 48 && c <= 57)) continue;

			// get all the digits
			int ci = c - '0';
			while (true)
			{
				in.read(&c, 1);
				if (c == 10 || c == 32 || in.eof()) break;
				if (c < 48 || c > 57) throw std::runtime_error("Failed to parse ranking.txt!");

				// shift the number to left and append next number to front
				ci *= 10;
				ci += c - '0';
			}

			g_ranking[id] = ci;
			++id;
		}
		catch (std::runtime_error& e)
		{
			// display error box and exit program
			ShowCursor(true);
			MessageBox(NULL,
				e.what(),
				"Error!",
				MB_ICONEXCLAMATION | MB_OK);

			std::exit(1);
		}
	}
}
