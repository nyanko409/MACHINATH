#include "sceneManagement.h"
#include "input.h"


// current scene
SceneType g_scene = SCENE_STARTSCREEN;


void SetScene(SceneType sc)
{
	g_scene = sc;
}
SceneType GetScene()
{
	return g_scene;
}

void UpdateScene()
{
	// return when in game screen
	if (g_scene == SCENE_GAMESCREEN) return;

	if (g_scene == SCENE_STARTSCREEN)
	{
		// change to game scene when button is pressed
		if (Keyboard_IsTrigger(DIK_SPACE))
		{
			g_scene = SCENE_GAMESCREEN;
			return;
		}
	}
	else if (g_scene == SCENE_RESULTSCREEN)
	{
		// go back to start screen when button is pressed
		if (Keyboard_IsTrigger(DIK_SPACE))
		{
			g_scene = SCENE_STARTSCREEN;
			return;
		}
	}
}
