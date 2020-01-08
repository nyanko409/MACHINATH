#include "player.h"
#include "material.h"
#include "transformation.h"
#include "input.h"
#include "camera.h"
#include "playTime.h"
#include "sceneManagement.h"
#include "map.h"
#include "customMath.h"
#include "common.h"

// globals 
static GameObject* g_parent;
static Player* g_player;
static MeshObject* g_skateboard;
static float g_zRotSpeed;
static float g_zRotMax;
static float g_jumpHeight;
static float g_jumpSpeed;

static D3DXVECTOR3 g_camPos;
static float g_finalYPos;
static bool g_isJumping;
static int g_jumpCnt;
static float g_curRot, g_curSlopeRot;

void MovePlayer();
void MoveSideways();
void Jump();
void PlayerCamera();
void HandleMapEvent();
void Curve(EventData& event);
void Slope(EventData& event);
void CheckMapCollision();


// override player draw
void Player::Draw()
{
	BoxCollider::DrawCollider(GetPlayer()->col, D3DCOLOR(D3DCOLOR_RGBA(0, 0, 0, 255)));

	auto device = MyDirect3D_GetDevice();
	//device->SetRenderState(D3DRS_LIGHTING, false);
	BoneObject::Draw();
	//device->SetRenderState(D3DRS_LIGHTING, true);

}


void InitPlayer()
{	
	// init
	g_zRotSpeed = 3.0F;
	g_zRotMax = 20.0F;
	g_jumpHeight = 10.0F;
	g_jumpSpeed = 3.0F;

	g_isJumping = false;
	g_jumpCnt = 0;
	g_curRot = 0;
	g_curSlopeRot = 0;

	// create parent
	Transform trans = Transform(D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(1, 1, 1));
	g_parent = new GameObject(trans);

	// create player
	g_player = new Player(trans, 2.0F, 1.0F, 1.5F, A_MESH_ROBOT, SHADER_DEFAULT, 4, 4, 4, g_parent);
	g_player->pivot.y += 1;
	g_player->PlayAnimation(0);
	g_player->SetAnimationSpeed(0.005F);

	g_camPos = trans.position;

	// create skateboard and make player the parent
	trans = Transform(D3DXVECTOR3(-0.2F, -0.5F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(1, 1, 1));
	g_skateboard = new MeshObject(trans, MESH_SKATEBOARD, SHADER_DEFAULT, g_player);

	ResetTimer();
	StartTimer();
}

void UninitPlayer()
{
	// free memory
	SAFE_DELETE(g_skateboard);
	SAFE_DELETE(g_player);
	SAFE_DELETE(g_parent);
}

void UpdatePlayer()
{	
	HandleMapEvent();
	MovePlayer();
	MoveSideways();
	Jump();
	PlayerCamera();

	CheckMapCollision();
}


void MovePlayer()
{
	// move parent
	g_parent->transform.position += g_parent->GetForward() * g_player->moveSpeed;

	// offset player y position to active map
	if (g_parent->transform.rotation.x == 0.0F)
	{
		Map* map = GetMapById(GetCurrentMapId());

		if(map != nullptr)
			g_parent->transform.position.y = Lerp(g_parent->transform.position.y, 
										map->transform.position.y + g_player->heightOffset, 0.1F);
	}
}

void HandleMapEvent()
{
	// get first event
	auto map = GetMap();
	if (!(map->size() > 0)) return;

	Map* front = nullptr;
	for (int i = 0; i < map->size(); ++i)
	{
		for (int j = 0; j < (*map)[i]->data.event.size(); ++j)
		{
			if (!(*map)[i]->data.event[j].finished &&
				!((*map)[i]->data.event[j].mapEvent == MapEvent::NONE))
			{
				front = (*map)[i];
				break;
			}
		}

		// break if front map with event is found
		if (front != nullptr) break;
	}

	// return if no event is found
	if (front == nullptr) return;
	
	// handle events
	for (int i = 0; i < front->data.event.size(); ++i)
	{
		// start event if collied with event trigger
		if (!front->data.event[i].started)
		{
			if (g_player->col.CheckCollision(front->data.event[i].trigger))
			{
				front->data.event[i].started = true;
			}
		}

		// update events
		if (front->data.event[i].started && !front->data.event[i].finished)
		{
			if (front->data.event[i].mapEvent == MapEvent::CURVE)
				Curve(front->data.event[i]);
			else if (front->data.event[i].mapEvent == MapEvent::SLOPE)
				Slope(front->data.event[i]);
		}
	}
}

void Curve(EventData& event)
{
	float speed = event.speed * g_player->moveSpeed;

	// get rotation to add
	g_curRot += speed;
	float frameRot = g_curRot > fabsf(event.value) ? g_curRot - speed : speed;
	if (event.value < 0) frameRot *= -1;

	if (g_curRot >= fabsf(event.value))
	{
		event.finished = true;
		g_curRot = 0;
	}

	// add rotation to player
	g_parent->transform.rotation.y += frameRot;
}

void Slope(EventData& event)
{
	float speed = event.speed * g_player->moveSpeed;

	// rotate to climb slope
	if (g_curSlopeRot < fabsf(event.value))
	{
		g_curSlopeRot += speed;

		float frameRot = g_curSlopeRot > fabsf(event.value) ? 
			fabsf(event.value) - (g_curSlopeRot - speed) : speed;
		if (event.value < 0) frameRot *= -1;

		// clamp
		if (g_curSlopeRot > fabsf(event.value)) 
			g_curSlopeRot = fabsf(event.value);

		// add rotation to player
		g_parent->transform.rotation.x += frameRot;
	}
	else
	{
		event.finished = true;
		g_curSlopeRot = 0;
	}
}

Player* GetPlayer()
{
	return g_player;
}

void MoveSideways()
{
	// move player and rotate in z axis
	if (Keyboard_IsPress(DIK_F))
	{
		D3DXMATRIX mRot;
		D3DXVECTOR3 left(0,0,1);
		D3DXMatrixRotationY(&mRot, D3DXToRadian(g_parent->transform.rotation.y - 90));
		D3DXVec3TransformCoord(&left, &left, &mRot);

		g_parent->transform.position += left * g_player->sideSpeed;

		g_player->transform.localRotation.z -= g_zRotSpeed;
		if (g_player->transform.localRotation.z < -g_zRotMax)
			g_player->transform.localRotation.z = -g_zRotMax;
	}
	else if (Keyboard_IsPress(DIK_G))
	{
		D3DXMATRIX mRot;
		D3DXVECTOR3 right(0, 0, 1);
		D3DXMatrixRotationY(&mRot, D3DXToRadian(g_parent->transform.rotation.y + 90));
		D3DXVec3TransformCoord(&right, &right, &mRot);

		g_parent->transform.position += right * g_player->sideSpeed;

		g_player->transform.localRotation.z += g_zRotSpeed;
		if (g_player->transform.localRotation.z > g_zRotMax)
			g_player->transform.localRotation.z = g_zRotMax;
	}

	// else rotate back to original position
	else
	{
		if (g_player->transform.localRotation.z > 0)
		{
			g_player->transform.localRotation.z -= g_zRotSpeed;
			if (g_player->transform.localRotation.z < 0)
				g_player->transform.localRotation.z = 0;
		}
		else
		{
			g_player->transform.localRotation.z += g_zRotSpeed;
			if (g_player->transform.localRotation.z > 0)
				g_player->transform.localRotation.z = 0;
		}
	}
}

void Jump()
{
	if (!g_isJumping && Keyboard_IsPress(DIK_J))
	{
		g_isJumping = true;
		g_finalYPos = g_player->transform.position.y;
	}

	//jump
	if (g_isJumping)
	{
		g_player->transform.position.y = g_finalYPos + g_jumpHeight * sin(D3DXToRadian(g_jumpCnt));
		g_jumpCnt += g_jumpSpeed;
		float finalRot = 360.0F / (180.0F / g_jumpSpeed);

		g_player->transform.rotation.y += finalRot;
		//g_player->transform.rotation.z += finalRot;
		//g_player->transform.rotation.x += finalRot;

		if (g_jumpCnt > 180)
		{
			g_isJumping = false;
			g_jumpCnt = 0;
			g_player->transform.rotation.y = 0.0F;
			//g_player->transform.rotation.z = 0.0F;
			//g_player->transform.rotation.x = 0.0F;
		}
	}
}

void PlayerCamera()
{
	static int rotY = 0;
	rotY++;
	float offsetZ = -10;
	float offsetY = 5;

	float offsetX = offsetZ;

	D3DXVECTOR3 forward = g_parent->GetForward();

	offsetZ *= forward.z;
	offsetX *= forward.x;

	g_camPos = Lerp(g_camPos, g_player->GetCombinedPosition(), 0.1F);

	auto v = SetCameraForward(g_player->GetCombinedPosition());
	SetCameraPos(g_camPos, offsetX, offsetY, offsetZ, rotY);
}

void CheckMapCollision()
{
	// get the map array
	auto map = GetMap();
	auto forward = g_parent->GetForward();

	int curMapId = GetCurrentMapId();

	// get the direction to check first
	bool nsFirst = (forward.x < 0.45F && forward.x > -0.45F) ? false : true;

	// loop for every active map and every attached collider
	for (Map* m : *map)
	{
		// check collision only for current and next map
		if (m->id < curMapId) continue;
		if (m->id > curMapId + 1) break;

		// loop for every collider attached to the map
		for (BoxCollider col : m->col)
		{
			switch (g_player->col.CheckCollision(col, nsFirst))
			{
			case 1: 
			case 2: 
				g_parent->transform.position.x = g_parent->GetPreviousPosition().x;
				break;
			case 3:	
			case 4: 
				g_parent->transform.position.z = g_parent->GetPreviousPosition().z;
				break;
			default: continue;
			}
		}
	}
}
