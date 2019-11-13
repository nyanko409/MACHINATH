#include "input.h"
#include "font.h"
#include "texture.h"
#include "sprite.h"
#include "score.h"
#include "qte.h"

// performance threshold
#define PERFECT 0.1F
#define GREAT 0.2F
#define BAD 0.3F

// globals
static bool active;
static Sprite inner, outer;
static float outerScale = 1.0F;
static float innerScale = 0.4F;



void StartQTE()
{
	// set scale and active
	outer.scale.x = outerScale;
	outer.scale.y = outerScale;
	inner.scale.x = innerScale;
	inner.scale.y = innerScale;

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
	if (!active)
		StartQTE();

	if (active)
	{
		// reduce scale
		outer.scale -= {0.01F, 0.01F};
		outer.rotZ += 10;

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
}

void DrawQTE()
{
	// draw qte when active
	if (active)
	{
		SpriteDraw(inner);
		SpriteDraw(outer);
	}
}
