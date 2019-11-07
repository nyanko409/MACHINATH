#include <vector>
#include <string>
#include <algorithm>
#include "shader.h"
#include "mydirect3d.h"


// shader struct
struct Effect_Data
{
	std::string path;
	LPD3DXEFFECT effect;
	D3DXHANDLE technique;
};

// stores every shader and pointer to a list of gameobject to apply the shader to
static std::vector<std::pair<Effect_Data, std::vector<GameObject*>>> effect
{
	std::pair<Effect_Data,std::vector<GameObject*>>(Effect_Data{"asset/shader/custom/vertex.fx"}, std::vector<GameObject*>()),

	std::pair<Effect_Data,std::vector<GameObject*>>(Effect_Data{""}, std::vector<GameObject*>())
};


void InitShader()
{
	auto pDevice = MyDirect3D_GetDevice();
	LPD3DXBUFFER errorlog;
	
	// load shader into memory
	for (int i = 0; i < effect.size() - 1; i++)
	{
		D3DXCreateEffectFromFile(pDevice, effect[i].first.path.c_str(), 0, 0, 
			D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, 0, &effect[i].first.effect, &errorlog);

		effect[i].first.effect->FindNextValidTechnique(NULL, &effect[i].first.technique);
	}
}

void DrawObjects()
{
	auto pDevice = MyDirect3D_GetDevice();

	// get view and projection matrix
	D3DXMATRIX matView, matProjection;
	pDevice->GetTransform(D3DTS_VIEW, &matView);
	pDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

	// draw for every shader except for default shader
	for (int i = 0; i < effect.size() - 1; i++)
	{
		// begin pass
		effect[i].first.effect->Begin(NULL, NULL);
		effect[i].first.effect->BeginPass(0);

		// set view and projection matrix once
		effect[i].first.effect->SetMatrix("View", &matView);
		effect[i].first.effect->SetMatrix("Projection", &matProjection);

		// draw every assigned gameobject 
		for (int j = 0; j < effect[i].second.size(); j++)
		{
			effect[i].second[j]->Draw(true, true, effect[i].first.effect);
		}

		// end pass
		effect[i].first.effect->EndPass();
		effect[i].first.effect->End();

		// draw rest with default shader
		for (int j = 0; j < effect.back().second.size(); j++)
		{
			effect.back().second[j]->Draw(true, true);
		}
	}
}

void AssignShader(GameObject* obj, SHADER_TYPE type)
{
	effect[type].second.push_back(obj);
}

void DeassignShader(GameObject* obj, SHADER_TYPE type)
{
	effect[type].second.erase(std::remove(effect[type].second.begin(), effect[type].second.end(), obj), effect[type].second.end());
}

void UninitShader()
{
	// free memory
	//if(effect)
	//	effect->Release();
}
