#pragma once

#include "collider.h"

struct CameraEventData
{
	int yRot, yRotSpeed;
	float zOffset, zOffsetSpeed;
	float yOffset, yOffsetSpeed;
	bool started, finished;
};

struct CameraEvent
{
	BoxCollider trigger;
	CameraEventData data;
};


void InitCameraPosition(D3DXVECTOR3 position);
void UpdateCameraEvent(CameraEventData& event);
void UpdateCameraPosition(GameObject* follow, const D3DXVECTOR3& forward);
void SetLerpSpeed(float lerpSpeed);
