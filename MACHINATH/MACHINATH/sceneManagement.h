#pragma once

enum SceneType
{
	SCENE_TITLESCREEN,
	SCENE_GAMESCREEN
};

// call once at start
void InitScene();

// set current scene
void SetScene(SceneType scene);

// get current scene
SceneType GetScene();
