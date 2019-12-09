#include "player.h"
#include "material.h"
#include "transformation.h"
#include "input.h"
#include "camera.h"
#include "playTime.h"
#include "sceneManagement.h"
#include "map.h"

// globals
#define JumpHeight	10.0F
#define JumpSpeed	3.0F

static GameObject* g_parent;
static Player* g_player;
static MeshObject* g_skateboard;
static float g_zRotSpeed;
static float g_zRotMax;

static float g_finalYPos;
static bool g_jumping;
static int g_jumpCnt;
static float g_curRot, g_curSlope;

void MovePlayer();
void MoveSideways();
void Jump();
void PlayerCamera();
void HandleMapEvent();
void Curve(EventData& event);
void Slope(EventData& event);


// override player draw
void Player::Draw()
{
	BoneObject::Draw();
}


void InitPlayer()
{	
	// init
	g_zRotSpeed = 3.0F;
	g_zRotMax = 10.0F;

	g_jumping = false;
	g_jumpCnt = 0;
	g_curRot = 0;
	g_curSlope = 0;

	// parent object for posing etc
	g_parent = new GameObject(Transform());

	// create player
	Transform trans = Transform(D3DXVECTOR3(0.0F, 2.5F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, -90.0F, 0.0F), D3DXVECTOR3(1, 1, 1));
	g_player = new Player(trans, 0.3F, A_MESH_ROBOT, SHADER_DEFAULT, 5, 5, 5, g_parent);
	g_player->pivot.y += 3;
	g_player->PlayAnimation(0);
	g_player->SetAnimationSpeed(0.004F);

	// create skateboard and make player the parent
	trans = Transform(D3DXVECTOR3(-0.2F, -2.5F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(1, 1, 1));
	g_skateboard = new MeshObject(trans, MESH_SKATEBOARD, SHADER_DEFAULT, g_player);
}

void UninitPlayer()
{
	// free memory
	delete g_skateboard;
	delete g_player;
	delete g_parent;
}

void UpdatePlayer()
{
	//g_player->transform.localRotation.x++;
	if (GetScene() != SCENE_GAMESCREEN) return;

	// clamp rotation between 0-360
	if (g_player->transform.localRotation.x >= 360.0F)
		g_player->transform.localRotation.x -= 360.0F;
	if (g_player->transform.localRotation.y >= 360.0F)
		g_player->transform.localRotation.y -= 360.0F;
	if (g_player->transform.localRotation.z >= 360.0F)
		g_player->transform.localRotation.z -= 360.0F;

	// map events
	HandleMapEvent();

	// move player forward
	MovePlayer();

	// move left and right
	MoveSideways();

	// handle jumping
	Jump();

	// camera movement
	//PlayerCamera();
}


void MovePlayer()
{
	g_parent->transform.position += g_parent->GetForward() * g_player->moveSpeed;
}

void HandleMapEvent()
{
	// get first event
	auto map = GetMap();
	if (!(map->size() > 0)) return;

	Map* front = nullptr;
	for (int i = 0; i < map->size(); i++)
	{
		if (!((*map)[i]->data.event.front().mapEvent == MapEvent::NONE))
		{
			front = (*map)[i];
			break;
		}
	}
	if (front == nullptr) return;

	// handle events
	// check x or z distance based on local map rotation
	float mapPos = (front->GetLocalRotation().y == 90 || front->GetLocalRotation().y == 270) ?
		front->GetCombinedPosition().x : front->GetCombinedPosition().z;

	for (int i = 0; i < front->data.event.size(); ++i)
	{
		// start event
		if (!front->data.event[i].started)
		{
			if (front->GetLocalRotation().y == 90 &&
				mapPos - front->data.event[i].distance <= g_player->GetCombinedPosition().x)
				front->data.event[i].started = true;

			else if (front->GetLocalRotation().y == 270 &&
				mapPos + front->data.event[i].distance >= g_player->GetCombinedPosition().x)
				front->data.event[i].started = true;

			else if (front->GetLocalRotation().y == 0 &&
				mapPos - front->data.event[i].distance <= g_player->GetCombinedPosition().z)
				front->data.event[i].started = true;

			else if (front->GetLocalRotation().y == 180 &&
				mapPos + front->data.event[i].distance >= g_player->GetCombinedPosition().z)
				front->data.event[i].started = true;
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
	// get rotation to add
	g_curRot += event.speed;
	float frameRot = g_curRot > fabsf(event.value) ? g_curRot - event.speed : event.speed;
	if (event.value < 0) frameRot *= -1;

	if (g_curRot >= fabsf(event.value))
	{
		event.finished = true;
		g_curRot = 0;
	}

	// add rotation to player
	g_parent->transform.localRotation.y += frameRot;

	// adjust x and z rotation ratio
	auto forw = g_parent->GetForward();
	g_parent->transform.localRotation.x *= fabsf(forw.x);
	g_parent->transform.localRotation.z *= fabsf(forw.z);
}

void Slope(EventData& event)
{

}

Player* GetPlayer()
{
	return g_player;
}

void MoveSideways()
{
	// move player and rotate parent in z axis
	if (Keyboard_IsPress(DIK_F))
	{
		D3DXMATRIX mRot;
		D3DXVECTOR3 left(0,0,1);
		D3DXMatrixRotationY(&mRot, D3DXToRadian(g_parent->transform.localRotation.y - 90));
		D3DXVec3TransformCoord(&left, &left, &mRot);

		g_parent->transform.position += left * g_player->moveSpeed;
		g_parent->transform.localRotation += g_parent->GetForward() * g_zRotSpeed;
	}
	else if (Keyboard_IsPress(DIK_G))
	{
		D3DXMATRIX mRot;
		D3DXVECTOR3 right(0, 0, 1);
		D3DXMatrixRotationY(&mRot, D3DXToRadian(g_parent->transform.localRotation.y + 90));
		D3DXVec3TransformCoord(&right, &right, &mRot);

		g_parent->transform.position += right * g_player->moveSpeed;
		g_parent->transform.localRotation -= g_parent->GetForward() * g_zRotSpeed;
	}

	// else rotate back to original position
	else
	{
		//if (g_parent->transform.localRotation.z > 0)
		//{
		//	g_parent->transform.localRotation.z -= g_zRotSpeed;
		//	if (g_parent->transform.localRotation.z < 0)
		//		g_parent->transform.localRotation.z = 0;
		//}
		//else
		//{
		//	g_parent->transform.localRotation.z += g_zRotSpeed;
		//	if (g_parent->transform.localRotation.z > 0)
		//		g_parent->transform.localRotation.z = 0;
		//}
	}

	// clip rotation
	//if (g_parent->transform.localRotation.z > g_zRotMax)
	//	g_parent->transform.localRotation.z = g_zRotMax;
	//if (g_parent->transform.localRotation.z < -g_zRotMax)
	//	g_parent->transform.localRotation.z = -g_zRotMax;
}

void Jump()
{
	if (!g_jumping && Keyboard_IsPress(DIK_J))
	{
		g_jumping = true;
		g_finalYPos = g_player->transform.position.y;
	}

	//jump
	if (g_jumping)
	{
		g_player->transform.position.y = g_finalYPos + JumpHeight * sin(D3DXToRadian(g_jumpCnt));
		g_jumpCnt += JumpSpeed;
		float finalRot = 360.0F / (180.0F / JumpSpeed);

		g_player->transform.localRotation.y += finalRot;
		g_player->transform.localRotation.z += finalRot;
		g_player->transform.localRotation.x += finalRot;

		if (g_jumpCnt > 180)
		{
			g_jumping = false;
			g_jumpCnt = 0;
			g_player->transform.localRotation.y = -90.0F;
			g_player->transform.localRotation.z = 0.0F;
			g_player->transform.localRotation.x = 0.0F;
		}
	}
}

void PlayerCamera()
{
	// set camera position
	static float rotX = 0, rotY = 0;
	static float offsetY = 10.0F;

	//if (playTime > 5.0F)
	//{
	//	rotY--;
	//	if (rotY <= -45) rotY = -45;
	//	offsetY -= 0.1F;
	//	if (offsetY < 10) offsetY = 10;
	//}

	SetCameraPos(D3DXVECTOR3(0, g_player->transform.position.y, g_player->transform.position.z), D3DXVECTOR3(0, offsetY, -10), 0, 0);
}
