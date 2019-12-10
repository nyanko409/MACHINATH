#include <Windows.h>
#include <string>
#include "font.h"
#include "score.h"

// score
static int g_score;


void AddScore(int value)
{
	g_score += value;
}


void InitScore()
{
	g_score = 0;
}

void DrawScore()
{
	// draw text
	std::string str = std::to_string(g_score);
	DrawTextTo(RECT{ 200, 100, 100, 50 }, str.c_str(), str.length());
}
