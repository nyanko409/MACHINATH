#include <string>
#include "sceneManagement.h"
#include "common.h"
#include "font.h"
#include "playTime.h"


// game time passed in seconds
float playTime = 0.0F;
bool isPlaying = false;


void UpdateTimer()
{
	// add playtime every frame
	if(isPlaying)
		playTime += TIME_PER_FRAME;
}

void StopTimer()
{
	isPlaying = false;
}

void ResetTimer()
{
	playTime = 0.0F;
}

void StartTimer()
{
	isPlaying = true;
}

void DrawTimer()
{
	// draw text
	std::string str = std::to_string(playTime);
	DrawTextTo(RECT{ 10, 10, 100, 50 }, str.c_str(), str.length() - 4);
}
