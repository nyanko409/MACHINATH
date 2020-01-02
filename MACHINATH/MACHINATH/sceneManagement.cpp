#include "sceneManagement.h"
#include "input.h"


// current scene
SceneType g_scene;


void SetScene(SceneType sc)
{
	g_scene = sc;
}
SceneType GetScene()
{
	return g_scene;
}

void InitScene()
{
	g_scene = SCENE_TITLESCREEN;
}
