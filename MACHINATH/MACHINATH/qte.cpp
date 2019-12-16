#include <vector>
#include "input.h"
#include "font.h"
#include "texture.h"
#include "sprite.h"
#include "score.h"
#include "playTime.h"
#include "qte.h"

// performance threshold
#define PERFECT 0.1F
#define GREAT	0.2F
#define BAD		0.3F

// globals
static Sprite g_inner, g_outer;
static float g_outerScale = 1.0F;
static float g_innerScale = 0.4F;
static float g_multiDelay = 0.3F;
static int g_multiCount = 5;

static std::vector<Sprite> g_outerMultiSprite;
static QTE g_activeQTE;
static bool g_active;
static int g_curCount;
static float g_curTime;

void qteDefault();
void qteMultiPress();



void StartQTE(QTE type)
{
	// return if qte is already active
	if (g_active) return;

	// activate qte
	g_activeQTE = type;
	g_curTime = playTime;
	g_curCount = g_multiCount;
	g_inner.scale.x = g_innerScale;
	g_inner.scale.y = g_innerScale;

	if (g_activeQTE == QTE_DEFAULT)
	{
		g_outer.color.a = 0;
		g_outer.scale.x = g_outerScale;
		g_outer.scale.y = g_outerScale;
	}

	g_active = true;
}




void InitQTE()
{
	// initialize
	g_active = false;

	g_inner = Sprite(Texture_GetTexture(TEXTURE_INDEX_QTE_INNER), D3DXVECTOR3(500, 500, 0), 
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_QTE_INNER) / 2, Texture_GetHeight(TEXTURE_INDEX_QTE_INNER) / 2, 0),
		0, D3DXVECTOR2(g_innerScale, g_innerScale), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_outer = Sprite(Texture_GetTexture(TEXTURE_INDEX_QTE_OUTER), D3DXVECTOR3(500, 500, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_QTE_OUTER) / 2, Texture_GetHeight(TEXTURE_INDEX_QTE_OUTER) / 2, 0),
		0, D3DXVECTOR2(g_outerScale, g_outerScale), D3DCOLOR_RGBA(255, 255, 255, 255));
}

void UninitQTE()
{

}

void UpdateQTE()
{
	// start qte when its not active for test
	if (!g_active)
		StartQTE(QTE_DEFAULT);

	// update active qte
	if (g_active)
	{
		if(g_activeQTE == QTE_DEFAULT)
			qteDefault();

		if (g_activeQTE == QTE_MULTIPRESS)
			qteMultiPress();
	}
}

void DrawQTE()
{
	// draw qte when active
	if (g_active)
	{
		SpriteDraw(g_inner);

		if(g_activeQTE == QTE_DEFAULT)
			SpriteDraw(g_outer);

		if (g_activeQTE == QTE_MULTIPRESS)
		{
			for (int i = 0; i < g_outerMultiSprite.size(); i++)
			{
				SpriteDraw(g_outerMultiSprite[i]);
			}
		}
	}
}



void qteDefault()
{
	// reduce scale, rotate and increase alpha
	g_outer.scale -= {0.02F, 0.02F};
	g_inner.rotZ += 10;
	g_outer.rotZ += 10;

	if (g_outer.color.a < 1.0F)
		g_outer.color.a += 0.05F;


	// get distance between inner and outer circle
	float dist = g_outer.scale.x - g_inner.scale.x;

	// check for user input
	if (Keyboard_IsTrigger(DIK_L))
	{
		// check for performance and add it to score
		dist = fabs(dist);
		int score = 0;

		if (dist < PERFECT)
			score += 100;
		else if (dist < GREAT)
			score += 50;
		else if (dist < BAD)
			score += 10;

		// add to score and finish qte
		AddScore(score);
		g_active = false;
	}

	// if distance < -bad, player didnt click
	if (dist < -BAD)
	{
		g_active = false;
	}
}

void qteMultiPress()
{
	// spawn outer sprite every multi delay
	if (playTime > g_curTime + g_multiDelay)
	{
		g_curTime = playTime;
		g_curCount--;

		if (g_curCount < 0)
		{
			g_active = false;
			return;
		}

		g_outerMultiSprite.push_back(Sprite(Texture_GetTexture(TEXTURE_INDEX_QTE_OUTER), D3DXVECTOR3(500, 500, 0),
			D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_QTE_OUTER) / 2, Texture_GetHeight(TEXTURE_INDEX_QTE_OUTER) / 2, 0),
			0, D3DXVECTOR2(g_outerScale, g_outerScale), D3DXCOLOR(1.0F, 1.0F, 1.0F, 0.0F)));
	}

	if (g_outerMultiSprite.size() > 0)
	{
		// loop for every outer sprite
		for (int i = 0; i < g_outerMultiSprite.size(); i++)
		{
			// rotate, scale and increase alpha
			if(g_outerMultiSprite[i].color.a < 1.0F)
				g_outerMultiSprite[i].color.a += 0.05F;

			g_outerMultiSprite[i].scale -= {0.01F, 0.01F};
			g_outerMultiSprite[i].rotZ -= 10;
		}

		// check for user input
		if (Keyboard_IsTrigger(DIK_L))
		{
			// add score
			AddScore(10);
		}

		// erase if scale <= 0
		if (g_outerMultiSprite[0].scale.x <= 0.0F)
			g_outerMultiSprite.erase(g_outerMultiSprite.begin());
	}
}
