#include <d3dx9.h>
#include "player.h"
#include "camera.h"
#include "cameraevent.h"
#include "customMath.h"


static std::vector<CameraEventData> g_camEvent;
static D3DXVECTOR3 g_camPos;
static float rotY = 0;
static float offsetZ;
static float offsetY;
static float g_lerpSpeed;

static bool init;
static int finalRotY;
static float finalOffsetZ, finalOffsetY;

void UpdateCameraEventData(CameraEventData& event);



void AddCameraEvent(CameraEventData& event)
{
	g_camEvent.emplace_back(event);
}

void UpdateCameraEvent()
{
	// execute queued up camera event
	if (!g_camEvent.empty())
	{
		UpdateCameraEventData(g_camEvent.front());

		// delete finished events
		if (g_camEvent.front().finished)
			g_camEvent.erase(g_camEvent.begin());
	}

	// update the camera position
	// get target and forward vector
	auto target = GetCamera()->target;
	auto forward = GetCamera()->forwardOverride->GetForward();

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


void InitCameraEvent()
{
	g_camEvent = std::vector<CameraEventData>();
	g_camPos = GetCamera()->position;
	offsetZ = -10;
	offsetY = 10;
	rotY = 0;
	g_lerpSpeed = 0.01F;
	init = false;
}

void UninitCameraEvent()
{
	g_camEvent.clear();
}

void UpdateCameraEventData(CameraEventData& event)
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
	if ( ((event.yRot >= 0 && rotY >= finalRotY) || (event.yRot <= 0 && rotY <= finalRotY)) &&
		 ((event.zOffset >= 0 && offsetZ >= finalOffsetZ) || (event.zOffset <= 0 && offsetZ <= finalOffsetZ)) &&
		 ((event.yOffset >= 0 && offsetY >= finalOffsetY) || (event.yOffset <= 0 && offsetY <= finalOffsetY)) )
	{
		event.finished = true;
		init = false;
	}
}

void SetLerpSpeed(float lerpSpeed)
{
	g_lerpSpeed = lerpSpeed;
}
