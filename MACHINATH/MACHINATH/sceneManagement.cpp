#include "sceneManagement.h"
#include "input.h"


// current scene
SceneType scene = SCENE_STARTSCREEN;


void SetScene(SceneType sc)
{
	scene = sc;
}

SceneType GetScene()
{
	return scene;
}

void UpdateScene()
{
	// return when in game screen
	if (scene == SCENE_GAMESCREEN) return;

	if (scene == SCENE_STARTSCREEN)
	{
		// change to game scene when button is pressed
		if (Keyboard_IsTrigger(DIK_SPACE))
		{
			scene = SCENE_GAMESCREEN;
		}
	}
	else if (scene == SCENE_RESULTSCREEN)
	{
		// go back to start screen when button is pressed
		if (Keyboard_IsTrigger(DIK_SPACE))
		{
			scene = SCENE_STARTSCREEN;
		}
	}
}
