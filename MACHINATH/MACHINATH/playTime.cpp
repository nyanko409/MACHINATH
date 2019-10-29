#include "common.h"
#include "playTime.h"

// game time passed in seconds
float playTime = 0.0F;
bool isPlaying = false;


void UpdateTimer()
{
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
