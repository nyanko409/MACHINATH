#include <vector>
#include "sound.h"
#include "player.h"
#include "material.h"
#include "transformation.h"
#include "input.h"
#include "camera.h"
#include "playTime.h"
#include "sceneManagement.h"
#include "customMath.h"
#include "common.h"
#include "countdown.h"
#include "qte.h"
#include "cameraevent.h"


// globals 
static GameObject* g_parent;
static Player* g_player;
static MeshObject* g_skateboard;
static MeshObject* g_skybox;
static float g_zRotSpeed;
static float g_zRotMax;

static std::vector<EventData> g_mapEvent;
static float g_jumpCnt;

void MovePlayer();
void MoveSideways();
void Jump();
void HandleMapEvent();
void Curve(EventData& event);
void Slope(EventData& event);
void CheckMapCollision();


// override player draw
void Player::Draw()
{
#if _DEBUG
	BoxCollider::DrawCollider(col, D3DCOLOR(D3DCOLOR_RGBA(255, 0, 255, 255)));
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

	g_mapEvent = std::vector<EventData>();
	g_jumpCnt = 0;

	// create parent
	Transform trans = Transform(D3DXVECTOR3(0.0F, 1.0F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(1, 1, 1));
	g_parent = new GameObject(trans);

	// create player and set parent
	g_player = new Player(trans, 2.0F, 1.0F, 1.0F, A_MESH_ROBOT, SHADER_DEFAULT, 4, 4, 4, g_parent);
	g_player->pivot.y += 1;
	g_player->PlayAnimation(1);
	g_player->SetAnimationSpeed(0.005F);

	// create skateboard and make player the parent
	trans = Transform(D3DXVECTOR3(-0.0F, -0.5F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(1, 1, 1));
	g_skateboard = new MeshObject(trans, MESH_SKATEBOARD, SHADER_DEFAULT, g_player);

	// create skybox
	g_skybox = new MeshObject(Transform(), MESH_SKYBOX, SHADER_DEFAULT);
	g_skybox->transform.scale = { 600, 600, 600 };

	// set the camera target
	GetCamera()->target = g_player;
	GetCamera()->forwardOverride = g_parent;

	// play BGM and start countdown
	PlaySound(AUDIO_BGM_GAME);
	//StartCountdown();
	g_player->isMoving = true;
}

void UninitPlayer()
{
	// free memory
	SAFE_DELETE(g_skateboard);
	SAFE_DELETE(g_player);
	SAFE_DELETE(g_parent);
	SAFE_DELETE(g_skybox);

	g_mapEvent.clear();
}

void UpdatePlayer()
{	
	// update skybox
	g_skybox->transform.position = g_player->GetCombinedPosition();
	g_skybox->transform.rotation.y += 0.01F;

	// return if in countdown
	if (!g_player->isMoving) return;

	// update map event
	HandleMapEvent();

	// move player
	MovePlayer();

	if(g_player->isMovingSideways)
		MoveSideways();

	// jump
	if (g_player->isJumping)
		Jump();

	// check for collision
	CheckMapCollision();
}


void MovePlayer()
{
	// move parent
	g_parent->transform.position += g_parent->GetForward() * g_player->moveSpeed * getSlowmoFactor();

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
	// update active events
	for (int i = 0; i < g_mapEvent.size(); ++i)
	{
		if (!g_mapEvent[i].finished)
		{
			if (g_mapEvent[i].mapEvent == MapEvent::CURVE)
				Curve(g_mapEvent[i]);
			else if (g_mapEvent[i].mapEvent == MapEvent::SLOPE)
				Slope(g_mapEvent[i]);
			else if (g_mapEvent[i].mapEvent == MapEvent::QTE_SINGLE)
			{
				g_mapEvent[i].finished = StartQTE(QTE_DEFAULT);
				g_player->jumpHeight = g_mapEvent[i].value;
			}
			else if (g_mapEvent[i].mapEvent == MapEvent::QTE_MULTI)
			{
				g_mapEvent[i].finished = StartQTE(QTE_MULTIPRESS, g_mapEvent[i].value);
				setSlowmoLimit(g_mapEvent[i].speed);
			}
		}
		else
		{
			g_mapEvent.erase(g_mapEvent.begin() + i);
			--i;
		}
	}

	// get first event
	auto map = GetMap();
	if (map->empty()) return;

	EventData* front = nullptr;
	for (int i = 0; i < map->size(); ++i)
	{
		if (!(*map)[i]->enableDraw) break;

		for (int j = 0; j < (*map)[i]->data.event.size(); ++j)
		{
			if (!(*map)[i]->data.event[j].started &&
				!((*map)[i]->data.event[j].mapEvent == MapEvent::NONE))
			{
				front = &(*map)[i]->data.event[j];
				break;
			}
		}

		// break if event is found
		if (front != nullptr) break;
	}
	
	// check for event collision
	if (!(front == nullptr))
	{
		// queue up map event if collied with event trigger
		if (g_player->col.CheckCollision(front->trigger))
		{
			front->started = true;
			g_mapEvent.emplace_back(*front);
			AddCameraEvent(front->camEvent);
		}
	}
}

void Curve(EventData& event)
{
	float speed = event.speed * g_player->moveSpeed;

	// get rotation to add
	event.curValue += speed;
	float frameRot = event.curValue > fabsf(event.value) ? event.curValue - speed : speed;
	if (event.value < 0) frameRot *= -1;

	if (event.curValue >= fabsf(event.value))
	{
		event.finished = true;
	}

	// add rotation to player
	g_parent->transform.rotation.y += frameRot;
}

void Slope(EventData& event)
{
	float speed = event.speed * g_player->moveSpeed;

	// rotate to climb slope
	if (event.curValue < fabsf(event.value))
	{
		event.curValue += speed;

		float frameRot = event.curValue > fabsf(event.value) ?
			fabsf(event.value) - (event.curValue - speed) : speed;
		if (event.value < 0) frameRot *= -1;

		// clamp
		if (event.curValue > fabsf(event.value))
			event.curValue = fabsf(event.value);

		// add rotation to player
		g_parent->transform.rotation.x += frameRot;
	}
	else
	{
		event.finished = true;
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

		g_parent->transform.position += left * g_player->sideSpeed * getSlowmoFactor();

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

		g_parent->transform.position += right * g_player->sideSpeed * getSlowmoFactor();

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
	g_player->transform.position.y = g_player->jumpHeight * sin(D3DXToRadian(g_jumpCnt));
	g_jumpCnt += g_player->jumpSpeed * getSlowmoFactor();
	float finalRot = 360.0F / (180.0F / (g_player->jumpSpeed * getSlowmoFactor()));

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

void QueueMapEvent(EventData data)
{
	g_mapEvent.emplace_back(data);
}
