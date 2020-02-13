#pragma once

#include "collider.h"

struct CameraEventData
{
	float yRot, yRotSpeed;
	float zOffset, zOffsetSpeed;
	float yOffset, yOffsetSpeed;
	bool started, finished;
	void(*action)();
};


// add a camera event to the queue
void AddCameraEvent(CameraEventData& event);

void InitCameraEvent();
void UninitCameraEvent();
void UpdateCameraEvent();
void SetLerpSpeed(float lerpSpeed);
