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
#define GREAT 0.2F
#define BAD 0.3F

// globals
static bool active;
static QTE activeQTE;
static Sprite inner, outer;
static std::vector<Sprite> outer_multi;
static float outerScale = 1.0F;
static float innerScale = 0.4F;
static float multi_delay = 0.3F;
static int multiCount = 5;

static int curCount;
static float curTime;

void qteDefault();
void qteMultiPress();



void StartQTE(QTE type)
{
	// return if qte is currently active
	if (active) return;

	// init qte
	activeQTE = type;
	curTime = playTime;
	curCount = multiCount;
	inner.scale.x = innerScale;
	inner.scale.y = innerScale;

	if (activeQTE == QTE_DEFAULT)
	{
		outer.color.a = 0;
		outer.scale.x = outerScale;
		outer.scale.y = outerScale;
	}

	active = true;
}




void InitQTE()
{
	// initialize
	active = false;

	inner = Sprite(Texture_GetTexture(TEXTURE_INDEX_QTE_INNER), D3DXVECTOR3(500, 500, 0), 
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_QTE_INNER) / 2, Texture_GetHeight(TEXTURE_INDEX_QTE_INNER) / 2, 0),
		0, D3DXVECTOR2(innerScale, innerScale), D3DCOLOR_RGBA(255, 255, 255, 255));

	outer = Sprite(Texture_GetTexture(TEXTURE_INDEX_QTE_OUTER), D3DXVECTOR3(500, 500, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_QTE_OUTER) / 2, Texture_GetHeight(TEXTURE_INDEX_QTE_OUTER) / 2, 0),
		0, D3DXVECTOR2(outerScale, outerScale), D3DCOLOR_RGBA(255, 255, 255, 255));
}

void UninitQTE()
{

}

void UpdateQTE()
{
	// start qte when its not active for test
	if (!active)
		StartQTE(QTE_DEFAULT);

	// update active qte
	if (active)
	{
		if(activeQTE == QTE_DEFAULT)
			qteDefault();

		if (activeQTE == QTE_MULTIPRESS)
			qteMultiPress();
	}
}

void DrawQTE()
{
	// draw qte when active
	if (active)
	{
		SpriteDraw(inner);

		if(activeQTE == QTE_DEFAULT)
			SpriteDraw(outer);

		if (activeQTE == QTE_MULTIPRESS)
		{
			for (int i = 0; i < outer_multi.size(); i++)
			{
				SpriteDraw(outer_multi[i]);
			}
		}
	}
}



void qteDefault()
{
	// reduce scale, rotate and increase alpha
	outer.scale -= {0.02F, 0.02F};
	outer.rotZ += 10;

	if (outer.color.a < 1.0F)
		outer.color.a += 0.05F;


	// get distance between inner and outer circle
	float dist = outer.scale.x - inner.scale.x;

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
		active = false;
	}

	// if distance < -bad, player didnt click
	if (dist < -BAD)
	{
		active = false;
	}
}

void qteMultiPress()
{
	// spawn outer sprite every multi delay
	if (playTime > curTime + multi_delay)
	{
		curTime = playTime;
		curCount--;

		if (curCount < 0)
		{
			active = false;
			return;
		}

		outer_multi.push_back(Sprite(Texture_GetTexture(TEXTURE_INDEX_QTE_OUTER), D3DXVECTOR3(500, 500, 0),
			D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_QTE_OUTER) / 2, Texture_GetHeight(TEXTURE_INDEX_QTE_OUTER) / 2, 0),
			0, D3DXVECTOR2(outerScale, outerScale), D3DXCOLOR(1.0F, 1.0F, 1.0F, 0.0F)));
	}

	if (outer_multi.size() > 0)
	{
		// loop for every outer sprite
		for (int i = 0; i < outer_multi.size(); i++)
		{
			// rotate, scale and increase alpha
			if(outer_multi[i].color.a < 1.0F)
				outer_multi[i].color.a += 0.05F;

			outer_multi[i].scale -= {0.01F, 0.01F};
			outer_multi[i].rotZ -= 10;
		}

		// check for user input
		if (Keyboard_IsTrigger(DIK_L))
		{
			// add score
			AddScore(10);
		}

		// erase if scale <= 0
		if (outer_multi[0].scale.x <= 0.0F)
			outer_multi.erase(outer_multi.begin());
	}
}
