#include <vector>
#include <Effekseer.h>
#include <EffekseerRendererDX9.h>
#include <EffekseerSoundXAudio2.h>
#include "mydirect3d.h"
#include "effect.h"
#include "camera.h"


#if _DEBUG
#pragma comment(lib,"Effekseer.Debug.lib")
#pragma comment(lib,"EffekseerRendererDX9.Debug.lib")
#pragma comment(lib,"EffekseerSoundXAudio2.Debug.lib")
#else
#pragma comment(lib,"Effekseer.Release.lib")
#pragma comment(lib,"EffekseerRendererDX9.Release.lib")
#pragma comment(lib,"EffekseerSoundXAudio2.Release.lib")
#endif

#define MAX_EFFECTS 10

// path to the effect
EFK_CHAR* path[5]
{
	(EFK_CHAR*)L"asset/effect/blow.efk",
	(EFK_CHAR*)L"asset/effect/pickup_medal.efk",
	(EFK_CHAR*)L"asset/effect/fuck.efk",
	(EFK_CHAR*)L"asset/effect/shield.efk",
	(EFK_CHAR*)L"asset/effect/eff.efk"
};


LPDIRECT3DDEVICE9 device = NULL;
Camera* camera = NULL;

EffekseerRenderer::Renderer* renderer = NULL;
Effekseer::Manager* manager = NULL;
std::vector<Effekseer::Handle> handle;

Effekseer::Effect* effect[MAX_EFFECTS] = { };
Effekseer::Vector3D g_delta[MAX_EFFECTS] = { };
float g_playSpeed[MAX_EFFECTS] = { };


void InitEffect()
{
	// get device and camera
	device = MyDirect3D_GetDevice();
	camera = GetCamera();

	// init effect and handle
	for (int i = 0; i < MAX_EFFECTS; i++)
	{
		effect[i] = NULL;
		handle.push_back(Effekseer::Handle(-1));
	}

	// init effect interface
	renderer = EffekseerRendererDX9::Renderer::Create(device, 5000);
	manager = Effekseer::Manager::Create(5000);

	// �`����@���w�肵�܂��B�Ǝ��Ɋg�����邱�Ƃ��ł��܂��B
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());

	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());

	// ���W�n�̎w��
	manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// set projection
	renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(camera->fov, camera->aspect, camera->nearClip, camera->farClip));
}

void PlayEffect(Effect type, D3DXVECTOR3 position, D3DXVECTOR3 delta, float speed)
{
	PlayEffect(type, position.x, position.y, position.z, delta.x, delta.y, delta.z, speed);
}

void PlayEffect(Effect type, float posX, float posY, float posZ, float deltaX, float deltaY, float deltaZ, float playSpeed)
{
	// �G�t�F�N�g�̓Ǎ�
	for (int i = 0; i < MAX_EFFECTS; i++)
	{
		if (!effect[i])
		{
			effect[i] = Effekseer::Effect::Create(manager, path[type]);
			g_delta[i] = Effekseer::Vector3D(deltaX, deltaY, deltaZ);
			g_playSpeed[i] = playSpeed;

			Effekseer::Handle _handle = manager->Play(effect[i], posX, posY, posZ);
			handle[i] = _handle;
			
			return;
		}
	}
}

void DrawEffect()
{
	// update camera matrix
	renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(Effekseer::Vector3D(camera->position.x, camera->position.y, camera->position.z),
			Effekseer::Vector3D(camera->forward.x + camera->position.x, camera->forward.y + camera->position.y, camera->forward.z + camera->position.z),
			Effekseer::Vector3D(0, 1, 0)));

	// update effects
	for (int i = 0; i < MAX_EFFECTS; i++)
	{
		// set handle and effect to null after draw
		if (effect[i] && !manager->Exists(handle[i]))
		{
			effect[i]->UnloadResources();
			effect[i]->Release();

			handle[i] = -1;
			effect[i] = NULL;
		}
		
		// update drawing effects
		if (handle[i] != -1)
		{
			manager->AddLocation(handle[i], g_delta[i]);
			manager->SetSpeed(handle[i], g_playSpeed[i]);
		}
	}

	manager->Update();
	
	// draw effect
	renderer->BeginRendering();
	manager->Draw();
	renderer->EndRendering();
}

void UninitEffect()
{
	for (int i = 0; i < MAX_EFFECTS; i++)
	{
		ES_SAFE_RELEASE(effect[i]);
	}

	manager->Destroy();
	renderer->Destroy();
}
