#include "player.h"
#include "material.h"
#include "transformation.h"
#include "input.h"
#include "camera.h"
#include "playTime.h"
#include "sceneManagement.h"
#include "map.h"
#include "customMath.h"

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
	g_zRotMax = 20.0F;
	g_jumpHeight = 10.0F;
	g_jumpSpeed = 3.0F;

	g_isJumping = false;
	g_jumpCnt = 0;
	g_curRot = 0;
	g_curSlope = 0;

	// create parent
	Transform trans = Transform(D3DXVECTOR3(0.0F, 3.0F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(1, 1, 1));
	g_parent = new GameObject(trans);

	// create player
	trans.position = { 0, 0, 0 };
	g_player = new Player(trans, 0.6F, A_MESH_ROBOT, SHADER_DEFAULT, 5, 5, 5, g_parent);
	g_player->pivot.y += 3;
	g_player->PlayAnimation(1);
	g_player->SetAnimationSpeed(0.005F);

	g_camPos = trans.position;

	// create skateboard and make player the parent
	trans = Transform(D3DXVECTOR3(-0.2F, -2.5F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DXVECTOR3(1, 1, 1));
	g_skateboard = new MeshObject(trans, MESH_SKATEBOARD, SHADER_DEFAULT, g_player);
}

void UninitPlayer()
{
	// free memory
	delete g_skateboard;
	delete g_player;
}

void UpdatePlayer()
{
	if (GetScene() != SCENE_GAMESCREEN) return;

	// map events
	HandleMapEvent();

	// move player forward
	MovePlayer();

	// move left and right
	MoveSideways();

	// handle jumping
	Jump();

	// camera movement
	PlayerCamera();
}


void MovePlayer()
{
	D3DXVECTOR3 forward = g_parent->GetForward();

	// move parent
	g_parent->transform.position += forward * g_player->moveSpeed;
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
	float mapPos = (front->GetCombinedRotation().y == 90 || front->GetCombinedRotation().y == 270) ?
		front->GetCombinedPosition().x : front->GetCombinedPosition().z;

	for (int i = 0; i < front->data.event.size(); ++i)
	{
		// start event
		if (!front->data.event[i].started)
		{
			if (front->GetCombinedRotation().y == 90 &&
				mapPos - front->data.event[i].distance <= g_player->GetCombinedPosition().x)
				front->data.event[i].started = true;

			else if (front->GetCombinedRotation().y == 270 &&
				mapPos + front->data.event[i].distance >= g_player->GetCombinedPosition().x)
				front->data.event[i].started = true;

			else if (front->GetCombinedRotation().y == 0 &&
				mapPos - front->data.event[i].distance <= g_player->GetCombinedPosition().z)
				front->data.event[i].started = true;

			else if (front->GetCombinedRotation().y == 180 &&
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
	g_parent->transform.rotation.y += frameRot;
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
	// move player and rotate in z axis
	if (Keyboard_IsPress(DIK_F))
	{
		D3DXMATRIX mRot;
		D3DXVECTOR3 left(0,0,1);
		D3DXMatrixRotationY(&mRot, D3DXToRadian(g_parent->transform.rotation.y - 90));
		D3DXVec3TransformCoord(&left, &left, &mRot);

		g_parent->transform.position += left * g_player->moveSpeed;

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

		g_parent->transform.position += right * g_player->moveSpeed;

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
		g_player->transform.rotation.x += finalRot;

		if (g_jumpCnt > 180)
		{
			g_isJumping = false;
			g_jumpCnt = 0;
			g_player->transform.rotation.y = 0.0F;
			//g_player->transform.rotation.z = 0.0F;
			g_player->transform.rotation.x = 0.0F;
		}
	}
}

void PlayerCamera()
{
	// set camera position
	static float rotX = 0, rotY = 0;
	float offsetY = 10.0F;
	float offsetZ = -10;

	//if (playTime > 5.0F)
	//{
	//	rotY--;
	//	if (rotY <= -90) rotY = -90;
	//	offsetY -= 0.1F;
	//	if (offsetY < 10) offsetY = 10;
	//}

	g_camPos = Lerp(g_camPos, g_player->GetCombinedPosition(), 0.1F);

	D3DXVECTOR3 lookAt = g_camPos;
	D3DXVECTOR3 pos = g_camPos;
	D3DXVECTOR3 forward = g_parent->GetForward();

	float offsetX = offsetZ;

	offsetZ *= forward.z;
	offsetX *= forward.x;

	pos.y += offsetY;
	pos.z += offsetZ;
	pos.x += offsetX;

	SetCameraPos(lookAt, pos, rotX, rotY);
}
