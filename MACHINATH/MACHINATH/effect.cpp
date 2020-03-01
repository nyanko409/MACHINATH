#include <vector>
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


// struct of effect data
struct EffectData
{
	Effekseer::Handle handle;
	Effekseer::Effect* effect;
	Effekseer::Vector3D position;
	Effekseer::Vector3D rotation;
	Effekseer::Vector3D scale;
	float playSpeed;
};

// path to the effect
EFK_CHAR* path[]
{
	(EFK_CHAR*)L"asset/shader/effekseer/jump.efk",
	(EFK_CHAR*)L"asset/shader/effekseer/gold.efk",
	(EFK_CHAR*)L"asset/shader/effekseer/landing.efk",
	(EFK_CHAR*)L"asset/shader/effekseer/qte_success.efk",
	(EFK_CHAR*)L"asset/shader/effekseer/boost.efk",
	(EFK_CHAR*)L"asset/shader/effekseer/air.efk",
	(EFK_CHAR*)L"asset/shader/effekseer/explosion_red.efk",
	(EFK_CHAR*)L"asset/shader/effekseer/explosion_twotone.efk",
	(EFK_CHAR*)L"asset/shader/effekseer/explosion_blue.efk",
	(EFK_CHAR*)L"asset/shader/effekseer/explosion_yellow.efk"
};

EffekseerRenderer::Renderer* renderer = NULL;
Effekseer::Manager* manager = NULL;

std::vector<EffectData> g_effect;


void InitEffect()
{
	// get device
	auto pDevice = MyDirect3D_GetDevice();

	// init effect interface
	renderer = EffekseerRendererDX9::Renderer::Create(pDevice, 5000);
	manager = Effekseer::Manager::Create(5000);

	// •`‰æ•û–@‚ðŽw’è‚µ‚Ü‚·B“ÆŽ©‚ÉŠg’£‚·‚é‚±‚Æ‚à‚Å‚«‚Ü‚·B
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());

	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());

	// À•WŒn‚ÌŽw’è
	manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

int PlayEffect(Effect type, D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale, float playSpeed)
{
	Effekseer::Effect* effect = Effekseer::Effect::Create(manager, path[type]);
	Effekseer::Handle handle = manager->Play(effect, position.x, position.y, position.z);

	g_effect.emplace_back(
		EffectData{ handle, effect, Effekseer::Vector3D(position.x, position.y, position.z),
		Effekseer::Vector3D(rotation.x, rotation.y, rotation.z),
		Effekseer::Vector3D(scale.x, scale.y, scale.z), playSpeed });

	return handle;
}

void UpdateEffect(int handle, D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale)
{
	// update the effect of given handle
	for (int i = 0; i < g_effect.size(); ++i)
	{
		if (g_effect[i].handle == handle)
		{
			g_effect[i].position = Effekseer::Vector3D(position.x, position.y, position.z);
			g_effect[i].rotation = Effekseer::Vector3D(rotation.x, rotation.y, rotation.z);
			g_effect[i].scale = Effekseer::Vector3D(scale.x, scale.y, scale.z);
			return;
		}
	}
}

void StopAllEffect()
{
	// release all effects
	manager->StopAllEffects();
	for (auto effect : g_effect)
	{
		if (effect.effect)
		{
			effect.effect->UnloadResources();
			effect.effect->Release();
			effect.effect = NULL;
		}
	}
	
	g_effect.clear();
}

void StopEffect(int& handle)
{
	manager->StopEffect(handle);
	for (int i = 0; i < g_effect.size(); ++i)
	{
		if (g_effect[i].handle == handle)
		{
			g_effect[i].effect->UnloadResources();
			g_effect[i].effect->Release();
			g_effect[i].effect = NULL;

			g_effect.erase(g_effect.begin() + i);
			handle = -1;
			return;
		}
	}
}

void DrawEffect()
{
	// update camera matrix
	auto pCamera = GetCamera();

	renderer->SetProjectionMatrix(Effekseer::Matrix44().PerspectiveFovLH(
		pCamera->fov, pCamera->aspect, pCamera->nearClip, pCamera->farClip));

	renderer->SetCameraMatrix(
		Effekseer::Matrix44().LookAtLH(
			Effekseer::Vector3D(pCamera->position.x, pCamera->position.y, pCamera->position.z),
			Effekseer::Vector3D(pCamera->forward.x + pCamera->position.x, 
				pCamera->forward.y + pCamera->position.y, pCamera->forward.z + pCamera->position.z),
			Effekseer::Vector3D(pCamera->up.x, pCamera->up.y, pCamera->up.z)));


	// update effects
	for (int i = 0; i < g_effect.size(); ++i)
	{
		// remove effect from vector if finished playing the effect
		if (g_effect[i].effect && !manager->Exists(g_effect[i].handle))
		{
			g_effect[i].effect->UnloadResources();
			g_effect[i].effect->Release();
			g_effect[i].effect = NULL;

			g_effect.erase(g_effect.begin() + i);
			--i;
			continue;
		}

		// update active effects
		manager->SetLocation(g_effect[i].handle, g_effect[i].position);
		manager->SetSpeed(g_effect[i].handle, g_effect[i].playSpeed);
		manager->SetScale(g_effect[i].handle, 
			g_effect[i].scale.X, g_effect[i].scale.Y, g_effect[i].scale.Z);
		manager->SetRotation(g_effect[i].handle, 
			D3DXToRadian(g_effect[i].rotation.X), 
			D3DXToRadian(g_effect[i].rotation.Y),
			D3DXToRadian(g_effect[i].rotation.Z));
	}

	manager->Update();
	
	// draw effect
	renderer->BeginRendering();
	manager->Draw();
	renderer->EndRendering();
}

void UninitEffect()
{
	// free unreleased effect from memory
	for (auto data : g_effect)
	{
		ES_SAFE_RELEASE(data.effect);
	}

	manager->Destroy();
	renderer->Destroy();
}
