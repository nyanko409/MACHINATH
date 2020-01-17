#pragma once

struct CameraEvent
{
	int yRot, yRotSpeed;
	float zOffset, zOffsetSpeed;
	float yOffset, yOffsetSpeed;
};

void InitCameraPosition(D3DXVECTOR3 position);
void UpdateCameraEvent(CameraEvent event, GameObject* follow);
