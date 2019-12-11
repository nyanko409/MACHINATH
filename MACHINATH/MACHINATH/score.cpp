#include <Windows.h>
#include <string>
#include "font.h"
#include "score.h"
#include "sprite.h"
#include "texture.h"

static float ScoreScale = 1.0F;
static int g_score;
static Sprite A;

void AddScore(int value)
{
	g_score += value;
}


void InitScore()
{
	g_score = 0;

	A = Sprite(Texture_GetTexture(TEXTURE_INDEX_SCORE), D3DXVECTOR3(200, 200, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_SCORE) / 2, Texture_GetHeight(TEXTURE_INDEX_SCORE) / 2, 0),
		0, D3DXVECTOR2(ScoreScale, ScoreScale), D3DCOLOR_RGBA(255, 255, 255, 255));
}

void DrawScore()
{
	// draw text
	std::string str = std::to_string(g_score);
	RECT ScoreRect;
	SpriteDraw(A);
	DrawTextTo(RECT{ 200, 100, 100, 50 }, str.c_str(), str.length());

	/*do
	{
		//�\������ʂ̐�������o��
		int n = s % 10;
		DrawSprite(x, 400, 80, 80,
			n % 5 * 0.2f,//u���W
			n / 5 * 0.2f,//v���W
			0.2f, 0.2f,
			MakeFloat4(1, 1, 1, 1));

		//���̈ʂ�\�����邽��10�Ŋ���
		s /= 10;
		//�\���ʒu����ɂ��炷
		x -= 40.0f;

		fig++;
	} while (fig < FIGURE_NUM);*/
}
