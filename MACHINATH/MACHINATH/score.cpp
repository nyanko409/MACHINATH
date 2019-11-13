#include <Windows.h>
#include <string>
#include "font.h"
#include "score.h"


static int score;

void AddScore(int sc)
{
	score += sc;
}



void InitScore()
{
	score = 0;
}

void DrawScore()
{
	// draw text
	std::string str = std::to_string(score);
	DrawTextTo(RECT{ 200, 100, 100, 50 }, str.c_str(), str.length());
}
