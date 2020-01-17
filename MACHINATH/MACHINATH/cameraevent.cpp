#include <d3dx9.h>
#include "player.h"
#include "camera.h"
#include "cameraevent.h"
#include "customMath.h"


static D3DXVECTOR3 g_camPos;

void InitCameraPosition(D3DXVECTOR3 position)
{
	g_camPos = position;
}

void UpdateCameraEvent(CameraEvent event, GameObject* follow)
{
	static int rotY = 0;
	rotY++;
	float offsetZ = -10;
	float offsetY = 5;

	float offsetX = offsetZ;

	D3DXVECTOR3 forward = follow->GetForward();

	offsetZ *= forward.z;
	offsetX *= forward.x;

	g_camPos = Lerp(g_camPos, follow->GetCombinedPosition(), 0.1F);

	auto v = SetCameraForward(follow->GetCombinedPosition());
	SetCameraPos(g_camPos, offsetX, offsetY, offsetZ, rotY);
}
