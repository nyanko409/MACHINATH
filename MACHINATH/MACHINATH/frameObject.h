#pragma once

#include <d3dx9.h>
#include <string>
#include "mydirect3d.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

LPD3DXFRAME topFrame = NULL;
LPD3DXANIMATIONCONTROLLER animController = NULL;
D3DXMATRIX* finalMatrices = NULL;
int maxFrames;

struct CUSTOM_FRAME : public D3DXFRAME
{
	D3DXMATRIX combinedTransformationMatrix;
};

struct CUSTOM_MESHCONTAINER : public D3DXMESHCONTAINER
{
	D3DXMATRIX** ppFrameMatrices;
	LPD3DXMESH pFinalMesh;
	LPDIRECT3DTEXTURE9 *pTexture;
};


class MeshAllocation : public ID3DXAllocateHierarchy
{
	STDMETHOD(CreateFrame)(LPCSTR name, LPD3DXFRAME* ppNewFrame)
	{
		// create custom frame
		CUSTOM_FRAME* pFrame = new CUSTOM_FRAME;
		*ppNewFrame = pFrame;
		ZeroMemory(pFrame, sizeof(CUSTOM_FRAME));

		// copy the name
		if (name)
		{
			UINT len = strlen(name) + 1;
			pFrame->Name = new char[len];
			memcpy(pFrame->Name, name, len);
		}

		return S_OK;
	}

	STDMETHOD(CreateMeshContainer) (LPCSTR name, const D3DXMESHDATA* pMeshData, const D3DXMATERIAL* pMaterial,
		const D3DXEFFECTINSTANCE* pEffectInstance, DWORD numMaterials, const DWORD* pAdjacency,
		LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER* ppNewMeshContainer)
	{
		// get device
		auto device = MyDirect3D_GetDevice();

		// init custom mesh container
		struct CUSTOM_MESHCONTAINER *pMeshContainer = new CUSTOM_MESHCONTAINER;
		*ppNewMeshContainer = pMeshContainer;
		ZeroMemory(pMeshContainer, sizeof(CUSTOM_MESHCONTAINER));

		// check if it is a normal mesh
		if (pMeshData->Type != D3DXMESHTYPE_MESH)
		{
			DestroyMeshContainer(pMeshContainer);
			return E_FAIL;
		}

		// copy the name
		if (name)
		{
			UINT len = strlen(name) + 1;
			pMeshContainer->Name = new char[len];
			memcpy(pMeshContainer->Name, name, len);
		}

		// copy the mesh data
		pMeshContainer->MeshData.Type = pMeshData->Type;
		pMeshContainer->MeshData.pMesh = pMeshData->pMesh;
		pMeshContainer->MeshData.pMesh->AddRef();

		// copy the materials
		pMeshContainer->pMaterials = new D3DXMATERIAL[numMaterials];
		for (DWORD i = 0; i < numMaterials; i++)
		{
			pMeshContainer->pMaterials[i] = pMaterial[i];
			pMeshContainer->pMaterials[i].MatD3D.Ambient = pMaterial[i].MatD3D.Diffuse;
		}

		pMeshContainer->pEffects = NULL;
		pMeshContainer->NumMaterials = max(1, numMaterials);

		// copy the adjacency data
		pMeshContainer->pAdjacency = new DWORD[pMeshData->pMesh->GetNumFaces() * 3];
		memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * pMeshData->pMesh->GetNumFaces() * 3);

		// copy skin info if available
		if (pSkinInfo)
		{
			pMeshContainer->pSkinInfo = pSkinInfo;
			pSkinInfo->AddRef();

			pMeshContainer->ppFrameMatrices = new D3DXMATRIX*[pSkinInfo->GetNumBones()];
			for (DWORD i = 0; i < pSkinInfo->GetNumBones(); i++)
			{
				pMeshContainer->ppFrameMatrices[i] = NULL;
			}
		}

		// create a duplicate of the mesh
		pMeshContainer->MeshData.pMesh->CloneMesh(D3DXMESH_MANAGED, NULL, device, &pMeshContainer->pFinalMesh);

		// load the textures
		pMeshContainer->pTexture = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
		std::string path;
		for (DWORD i = 0; i < numMaterials; i++)
		{
			pMeshContainer->pTexture[i] = 0;
			if (pMaterial[i].pTextureFilename)
			{
				if (pMeshContainer->pMaterials[i].pTextureFilename)
				{
					path = "asset/texture/";
					path += pMeshContainer->pMaterials[i].pTextureFilename;
				}
				else
					path = "";

				D3DXCreateTextureFromFileA(device, path.c_str(), &pMeshContainer->pTexture[i]);
			}
		}

		if (pMeshContainer->pSkinInfo)
			maxFrames = max(maxFrames, (int)pMeshContainer->pSkinInfo->GetNumBones());

		return S_OK;
	}

	STDMETHOD(DestroyFrame)(LPD3DXFRAME pFrameToFree)
	{
		// free the name and the frame
		SAFE_DELETE_ARRAY(pFrameToFree->Name);
		SAFE_DELETE(pFrameToFree);

		return S_OK;
	}

	STDMETHOD(DestroyMeshContainer)(LPD3DXMESHCONTAINER pMeshContainerToFree)
	{
		CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pMeshContainerToFree;

		// free memory
		SAFE_DELETE_ARRAY(pMeshContainer->Name);
		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
		SAFE_RELEASE(pMeshContainer->pFinalMesh);
		SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
		SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
		SAFE_RELEASE(pMeshContainer->pSkinInfo);
		SAFE_DELETE_ARRAY(pMeshContainer->ppFrameMatrices);
		for (UINT i = 0; i < pMeshContainer->NumMaterials; i++)
			SAFE_RELEASE(pMeshContainer->pTexture[i]);
		SAFE_DELETE(pMeshContainer);

		return S_OK;
	}
};

void Link_Frames(CUSTOM_FRAME* pFrame)
{
	CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pFrame->pMeshContainer;

	if (pMeshContainer && pMeshContainer->pSkinInfo)
	{
		// loop through each frame
		for (UINT i = 0; i < pMeshContainer->pSkinInfo->GetNumBones(); i++)
		{
			CUSTOM_FRAME* pTempFrame;
			pTempFrame = (CUSTOM_FRAME*)D3DXFrameFind(topFrame, pMeshContainer->pSkinInfo->GetBoneName(i));
			pMeshContainer->ppFrameMatrices[i] = &pTempFrame->combinedTransformationMatrix;
		}
	}

	// run for all siblings
	if (pFrame->pFrameSibling)
		Link_Frames((CUSTOM_FRAME*)pFrame->pFrameSibling);

	// run for the first child which will then run all other children
	if (pFrame->pFrameFirstChild)
		Link_Frames((CUSTOM_FRAME*)pFrame->pFrameFirstChild);
}

void Update_Frames(CUSTOM_FRAME* pFrame, D3DXMATRIX* pParentMatrix)
{
	if (pParentMatrix)
		pFrame->combinedTransformationMatrix = pFrame->TransformationMatrix * *pParentMatrix;
	else
		pFrame->combinedTransformationMatrix = pFrame->TransformationMatrix;

	// run for all siblings
	if (pFrame->pFrameSibling)
		Update_Frames((CUSTOM_FRAME*)pFrame->pFrameSibling, pParentMatrix);

	// run for the first child which will then run all other children
	if (pFrame->pFrameFirstChild)
		Update_Frames((CUSTOM_FRAME*)pFrame->pFrameFirstChild, &pFrame->combinedTransformationMatrix);
}

void Update_MeshContainers(CUSTOM_FRAME* pFrame)
{
	CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pFrame->pMeshContainer;

	if (pMeshContainer && pMeshContainer->pSkinInfo)
	{
		UINT numFrames = pMeshContainer->pSkinInfo->GetNumBones();

		// for each frame in the mesh container
		for (UINT i = 0; i < numFrames; i++)
		{
			finalMatrices[i] = *pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
			finalMatrices[i] *= *pMeshContainer->ppFrameMatrices[i];
		}

		// pointer to original and modified mesh
		void* pSrc = NULL;
		void *pDst = NULL;

		// lock the two meshes
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(NULL, &pSrc);
		pMeshContainer->pFinalMesh->LockVertexBuffer(NULL, &pDst);

		// store the animated mesh into final mesh
		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(finalMatrices, NULL, pSrc, pDst);

		// unlock the two meshes
		pMeshContainer->pFinalMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

		// run for all siblings
		if (pFrame->pFrameSibling)
			Update_MeshContainers((CUSTOM_FRAME*)pFrame->pFrameSibling);

		// run for the first child which will then run all other children
		if (pFrame->pFrameFirstChild)
			Update_MeshContainers((CUSTOM_FRAME*)pFrame->pFrameFirstChild);
	}
}

void Draw_Mesh(CUSTOM_FRAME* pFrame)
{
	auto device = MyDirect3D_GetDevice();

	CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pFrame->pMeshContainer;

	if (pMeshContainer)
	{
		for (UINT i = 0; i < pMeshContainer->NumMaterials; i++)
		{
			device->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D);
			device->SetTexture(0, pMeshContainer->pTexture[i]);

			pMeshContainer->pFinalMesh->DrawSubset(i);
		}
	}

	// run for all siblings
	if (pFrame->pFrameSibling)
		Draw_Mesh((CUSTOM_FRAME*)pFrame->pFrameSibling);

	// run for the first child which will then run all other children
	if (pFrame->pFrameFirstChild)
		Draw_Mesh((CUSTOM_FRAME*)pFrame->pFrameFirstChild);
}

void InitFrameMesh()
{
	auto device = MyDirect3D_GetDevice();

	MeshAllocation memAllocator;
	LPCSTR str = "asset/mesh/tiny.x";
	D3DXLoadMeshHierarchyFromX(str, D3DXMESH_MANAGED, device, &memAllocator, NULL, &topFrame, &animController);
	finalMatrices = new D3DXMATRIX[maxFrames];
	ZeroMemory(finalMatrices, sizeof(D3DXMATRIX) * maxFrames);
	Link_Frames((CUSTOM_FRAME*)topFrame);
}

void DrawFrameMesh()
{
	TransformObject(D3DXVECTOR3(40, 3, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.5F, 0.5F, 0.5F));
	if (animController)
	{
		static DWORD time = GetTickCount();
		animController->AdvanceTime((GetTickCount() - time) * 0.001F, NULL);
		time = GetTickCount();
	}

	Update_Frames((CUSTOM_FRAME*)topFrame, NULL);
	Update_MeshContainers((CUSTOM_FRAME*)topFrame);
	Draw_Mesh((CUSTOM_FRAME*)topFrame);
}
