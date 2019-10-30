#pragma once

enum SceneType
{
	SCENE_STARTSCREEN,
	SCENE_GAMESCREEN,
	SCENE_RESULTSCREEN
};

// call every frame
void UpdateScene();

// set current scene
void SetScene(SceneType scene);

// get current scene
SceneType GetScene();
