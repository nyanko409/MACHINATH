#include <vector>
#include "sound.h"
#include "input.h"
#include "font.h"
#include "texture.h"
#include "sprite.h"
#include "score.h"
#include "playTime.h"
#include "qte.h"
#include "common.h"
#include "player.h"

// performance threshold
#define PERFECT 0.1F
#define GREAT	0.2F
#define BAD		0.3F

// globals
static Sprite g_inner, g_outer, g_press, g_black;
static float g_outerScale = 1.0F;
static float g_innerScale = 0.4F;
static float g_multiDelay = 0.1F;
static int g_multiCount = 20;

static std::vector<Sprite> g_outerMultiSprite;
static QTE g_activeQTE;
static bool g_active;
static int g_curCount;
static float g_curTime;
static float g_alpha;
static float g_slowmoFactor = 1;


void qteDefault();
void qteMultiPress();
void finishQTE();


bool StartQTE(QTE type)
{
	// return if qte is already active
	if (g_active) return false;

	// activate qte
	PlaySound(AUDIO_SE_SLOWMO_START, 1.0F);
	g_activeQTE = type;
	g_curTime = playTime;
	g_curCount = g_multiCount;
	g_inner.scale.x = g_innerScale;
	g_inner.scale.y = g_innerScale;
	g_inner.color.a = 0;
	g_press.color.a = 0;

	if (g_activeQTE == QTE_DEFAULT)
	{
		g_outer.scale.x = g_outerScale;
		g_outer.scale.y = g_outerScale;
		g_outer.color.a = 0;
	}

	g_alpha = 0;
	g_slowmoFactor = 1;
	g_active = true;
	return true;
}

bool IsQTEActive()
{
	return g_active;
}



void InitQTE()
{
	// initialize
	g_active = false;

	g_inner = Sprite(Texture_GetTexture(TEXTURE_INDEX_QTE_INNER), D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT-200, 0), 
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_QTE_INNER) / 2, Texture_GetHeight(TEXTURE_INDEX_QTE_INNER) / 2, 0),
		0, D3DXVECTOR2(g_innerScale, g_innerScale), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_outer = Sprite(Texture_GetTexture(TEXTURE_INDEX_QTE_OUTER), D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT-200, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_QTE_OUTER) / 2, Texture_GetHeight(TEXTURE_INDEX_QTE_OUTER) / 2, 0),
		0, D3DXVECTOR2(g_outerScale, g_outerScale), D3DCOLOR_RGBA(255, 255, 255, 255));

	g_press = Sprite(Texture_GetTexture(TEXTURE_INDEX_QTE_AFTER), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 200, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_QTE_INNER) / 2, Texture_GetHeight(TEXTURE_INDEX_QTE_INNER) / 2, 0),
		0, D3DXVECTOR2(g_innerScale, g_innerScale), D3DCOLOR_RGBA(255, 255, 255, 255));
}

void UninitQTE()
{

}

void UpdateQTE()
{
	SetPlaybackSpeed(AUDIO_BGM_GAME, 0.9 + 0.1 * g_slowmoFactor);

	if (!g_active)
	{
		// decrease slowmo factor
		g_slowmoFactor += 0.04F;
		if(g_slowmoFactor > 1)
			g_slowmoFactor = 1;
	}

	if (g_active)
	{
		// increase slowmo factor
		g_slowmoFactor -= 0.04F;
		if (g_slowmoFactor < 0.2F)
			g_slowmoFactor = 0.2F;

		// increase alpha
		g_alpha += 0.05;
		if (g_alpha > 1) g_alpha = 1;

		// update sprite alpha
		g_inner.color.a = g_alpha;
		g_outer.color.a = g_alpha;
		g_press.color.a = g_alpha;

		// update active qte
		switch (g_activeQTE)
		{
		case QTE_DEFAULT:
			qteDefault();
			break;

		case QTE_MULTIPRESS:
			qteMultiPress();
			break;

		default:
			break;
		}
	}
	else if(g_alpha > 0)
	{
		// decrease alpha
		g_alpha -= 0.05;
		if (g_alpha < 0)
			g_alpha = 0;

		// update sprite alpha
		g_inner.color.a = g_alpha;
		g_outer.color.a = g_alpha;
		g_press.color.a = g_alpha;
	}
}

void DrawQTE()
{
	// draw qte when active or not faded out
	if (g_active || g_alpha > 0)
	{
		SpriteDraw(g_inner);

		if (Keyboard_IsPress(DIK_SPACE))
			SpriteDraw(g_press);

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
	g_outer.scale -= {0.015F, 0.015F};
	//g_inner.rotZ += 10;
	g_outer.rotZ += 10;

	if (g_outer.color.a < 1.0F)
		g_outer.color.a += 0.05F;


	// get distance between inner and outer circle
	float dist = g_outer.scale.x - g_inner.scale.x;

	// check for user input
	if (Keyboard_IsTrigger(DIK_SPACE))
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

		// add score, set jump flag and finish qte
		AddScore(score);
		finishQTE();
	}

	// if distance < -bad, player didnt click
	if (dist < -BAD)
	{
		finishQTE();
	}
}

void qteMultiPress()
{
	// spawn outer sprite every multi delay
	if (playTime > g_curTime + g_multiDelay)
	{
		g_curTime = playTime;
		g_curCount--;

		// disable qte after its done
		if(g_curCount > 0)
		{
			g_outerMultiSprite.push_back(Sprite(Texture_GetTexture(TEXTURE_INDEX_QTE_OUTER), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 200, 0.1),
				D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_QTE_OUTER) / 2, Texture_GetHeight(TEXTURE_INDEX_QTE_OUTER) / 2, 0),
				0, D3DXVECTOR2(g_outerScale, g_outerScale), D3DXCOLOR(1.0F, 1.0F, 1.0F, 0.0F)));
		}
		else if (g_outerMultiSprite.size() == 0)
		{
			finishQTE();
			g_outerMultiSprite.clear();
			return;
		}
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

		// add score after user input
		if (Keyboard_IsTrigger(DIK_SPACE))
		{
			AddScore(10);
		}

		// erase if scale <= inner scale
		if (g_outerMultiSprite[0].scale.x <= g_innerScale)
			g_outerMultiSprite.erase(g_outerMultiSprite.begin());
	}
}


void finishQTE()
{
	// jump if default qte
	if (g_activeQTE == QTE_DEFAULT)
	{
		GetPlayer()->jumpSpeed = 2.9F;
		GetPlayer()->isJumping = true;
	}

	PlaySound(AUDIO_SE_SLOWMO_END, 1.0F);
	g_active = false;
}

float getSlowmoFactor()
{
	return g_slowmoFactor;
}
