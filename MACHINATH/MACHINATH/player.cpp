#include <vector>
#include "sound.h"
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
#include "countdown.h"
#include "cameraevent.h"


// globals 
static GameObject* g_parent;
static Player* g_player;
static MeshObject* g_skateboard;
static MeshObject* g_skybox;
static float g_zRotSpeed;
static float g_zRotMax;
static float g_jumpHeight;
static float g_jumpSpeed;

static std::vector<CameraEventData> g_camEvent;
static float g_finalYPos;
static int g_jumpCnt;
static float g_curRot, g_curSlopeRot;

void MovePlayer();
void MoveSideways();
void Jump();
void HandleMapEvent();
void HandleCameraEvent();
void Curve(EventData& event);
void Slope(EventData& event);
void CheckMapCollision();


// override player draw
void Player::Draw()
{
#if _DEBUG
	BoxCollider::DrawCollider(GetPlayer()->col, D3DCOLOR(D3DCOLOR_RGBA(255, 0, 255, 255)));
#endif

	BoneObject::Draw();
}


void InitPlayer()
{	
	// init
	ResetTimer();
	StartTimer();
	g_zRotSpeed = 2.0F;
	g_zRotMax = 20.0F;
	g_jumpHeight = 10.0F;
	g_jumpSpeed = 3.0F;

	g_camEvent = std::vector<CameraEventData>();
	g_jumpCnt = 0;
	g_curRot = 0;
	g_curSlopeRot = 0;

	// create parent
	Transform trans = Transform(D3DXVECTOR3(0.0F, 1.0F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(1, 1, 1));
	g_parent = new GameObject(trans);

	// create player and set parent
	g_player = new Player(trans, 2.0F, 1.0F, 1.0F, A_MESH_ROBOT, SHADER_DEFAULT, 4, 4, 4, g_parent);
	g_player->pivot.y += 1;
	g_player->PlayAnimation(0);
	g_player->SetAnimationSpeed(0.005F);

	// create skateboard and make player the parent
	trans = Transform(D3DXVECTOR3(-0.2F, -0.5F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(1, 1, 1));
	g_skateboard = new MeshObject(trans, MESH_SKATEBOARD, SHADER_DEFAULT, g_player);

	// create skybox and set parent
	g_skybox = new MeshObject(Transform(), MESH_SKYBOX, SHADER_DEFAULT, g_parent);
	g_skybox->transform.scale = { 400, 400, 400 };

	// play BGM and start countdown
	PlaySound(AUDIO_BGM_GAME);
	InitCameraPosition({ 0, 70, -40 });
	SetLerpSpeed(0.01F);
	StartCountdown();
	//g_player->isMoving = true;
}

void UninitPlayer()
{
	// free memory
	SAFE_DELETE(g_skateboard);
	SAFE_DELETE(g_player);
	SAFE_DELETE(g_parent);
	SAFE_DELETE(g_skybox);

	g_camEvent.clear();
}

void UpdatePlayer()
{	
	// handle camera
	UpdateCameraPosition(g_player, g_parent->GetForward());
	HandleCameraEvent();

	// return if in countdown
	if (!g_player->isMoving) return;

	// update map and player
	HandleMapEvent();
	MovePlayer();
	MoveSideways();

	if (g_player->isJumping)
		Jump();

	CheckMapCollision();
}


void MovePlayer()
{
	// move parent
	g_parent->transform.position += g_parent->GetForward() * g_player->moveSpeed;

	// lerp player y position to active map height
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
		// start map event if collied with event trigger
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

void HandleCameraEvent()
{
	// get first camera event
	auto map = GetMap();
	if (!(map->size() > 0)) return;

	CameraEvent* event = nullptr;
	for (int i = 0; i < map->size(); ++i)
	{
		if (!(*map)[i]->camEvent.data.started)
		{
			event = &(*map)[i]->camEvent;
			break;
		}
	}

	// check for collision with player
	if (event != nullptr && g_player->col.CheckCollision(event->trigger) != 0)
	{
		event->data.started = true;
		g_camEvent.emplace_back(event->data);
	}
	
	// execute queued up camera event
	if (g_camEvent.size() > 0)
	{
		UpdateCameraEvent(g_camEvent.front());

		// delete finished events
		if (g_camEvent.front().finished)
			g_camEvent.erase(g_camEvent.begin());
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
	// move player up
	g_player->transform.position.y = g_finalYPos + g_jumpHeight * sin(D3DXToRadian(g_jumpCnt));
	g_jumpCnt += g_jumpSpeed;
	float finalRot = 360.0F / (180.0F / g_jumpSpeed);

	// rotate player
	g_player->transform.rotation.y += finalRot;
	g_player->transform.rotation.z += finalRot;
	//g_player->transform.rotation.x += finalRot;

	// player reached ground
	if (g_jumpCnt > 180)
	{
		g_player->isJumping = false;
		g_jumpCnt = 0;
		g_player->transform.rotation.y = 0.0F;
		g_player->transform.rotation.z = 0.0F;
		//g_player->transform.rotation.x = 0.0F;
	}
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
