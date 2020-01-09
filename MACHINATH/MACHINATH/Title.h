#pragma once
enum ColorFade
{
	COLOR_BLUE_IN,
	COLOR_BLUE_OUT,
	COLOR_YELLOW_IN,
	COLOR_YELLOW_OUT,
	COLOR_WHITE_IN,
	COLOR_WHITE_OUT,
	COLOR_GREEN_IN,
	COLOR_GREEN_OUT,
	COLOR_RED_IN,
	COLOR_RED_OUT
};

void InitTitleScreen();
void UpdateTitleScreen();
void DrawTitleScreen();
void FinalizeTitleScreen();
