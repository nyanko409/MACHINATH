#include <d3dx9.h>
#include "player.h"
#include "camera.h"
#include "cameraevent.h"
#include "customMath.h"


static D3DXVECTOR3 g_camPos;
static int rotY = 0;
static float offsetZ;
static float offsetY;
static float g_lerpSpeed;

static bool init;
static int finalRotY;
static float finalOffsetZ, finalOffsetY;


void InitCameraPosition(D3DXVECTOR3 position)
{
	g_camPos = position;
	offsetZ = -10;
	offsetY = 5;
	rotY = 0;
	g_lerpSpeed = 0.1F;
}

void UpdateCameraEvent(CameraEventData& event)
{ 
	if (!init)
	{
		finalRotY = rotY + event.yRot;
		finalOffsetZ = offsetZ + event.zOffset;
		finalOffsetY = offsetY + event.yOffset;
		init = true;
	}

	// update camera variables
	rotY += event.yRotSpeed;
	offsetZ += event.zOffsetSpeed;
	offsetY += event.yOffsetSpeed;

	// clamp
	if(event.yRot > 0) rotY = rotY > finalRotY ? finalRotY : rotY;
	else if(event.yRot < 0) rotY = rotY < finalRotY ? finalRotY : rotY;

	if (event.zOffset > 0) offsetZ = offsetZ > finalOffsetZ ? finalOffsetZ : offsetZ;
	else if (event.zOffset < 0) offsetZ = offsetZ < finalOffsetZ ? finalOffsetZ : offsetZ;

	if (event.yOffset > 0) offsetY = offsetY > finalOffsetY ? finalOffsetY : offsetY;
	else if (event.yOffset < 0) offsetY = offsetY < finalOffsetY ? finalOffsetY : offsetY;

	// check if event is finished
	if ((event.yRot > 0 && rotY >= finalRotY) || (event.yRot < 0 && rotY <= finalRotY) &&
		(event.zOffset > 0 && offsetZ >= finalOffsetZ) || (event.zOffset < 0 && offsetZ <= finalOffsetZ) &&
		(event.yOffset > 0 && offsetY >= finalOffsetY) || (event.yOffset < 0 && offsetY <= finalOffsetY))
	{
		event.finished = true;
		init = false;
	}
}

void UpdateCameraPosition()
{
	// get target and forward vector
	auto target = GetCamera()->target;
	auto forward = target->GetForward();

	// adjust z and x offset
	float offZ = offsetZ;
	float offX = offZ;
	offZ *= forward.z;
	offX *= forward.x;

	// lerp the camera to target
	g_camPos = Lerp(g_camPos, target->GetCombinedPosition(), g_lerpSpeed);

	// set camera forward direction and position
	SetCameraForward(target->GetCombinedPosition());
	SetCameraPos(g_camPos, offX, offsetY, offZ, rotY);
}

void SetLerpSpeed(float lerpSpeed)
{
	g_lerpSpeed = lerpSpeed;
}
