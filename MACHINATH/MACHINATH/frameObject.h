#pragma once

#include <d3dx9.h>
#include <string>
#include <map>
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



std::map<std::string, LPDIRECT3DTEXTURE9> g_TextureList;
//std::map<std::string, SkinMeshFile *>g_pXFileList;

// ����t���[���f�[�^
struct FrameData : public D3DXFRAME
{
	// �����s��
	D3DXMATRIXA16 m_CombinedTransformationMatrix;
};

// ���상�b�V���R���e�i
struct MeshContainer : public D3DXMESHCONTAINER
{
	// �}�e���A���Ŏg�p����e�N�X�`��
	LPDIRECT3DTEXTURE9 *m_TextureList;

	// �E�F�C�g�̐�
	DWORD m_BoneWeightNum;

	// �{�[���̐�
	DWORD m_BoneNum;

	// �{�[�����̃o�b�t�@
	LPD3DXBUFFER m_BoneBuffer;

	// �S�{�[���̃}�g���b�N�X�̃|�C���^�̔z��
	D3DXMATRIX **m_BoneMatrix;

	// �{�[���̃I�t�Z�b�g�s��
	D3DXMATRIX *m_BoneOffsetMatrix;
};


// �K�w�N���X(ID3DXAllocateHierarchy�Ŏg�����z�֐��̂ݒ�`)
class SkinMeshData : public ID3DXAllocateHierarchy
{
public:
	SkinMeshData() {}

	STDMETHOD(CreateFrame)(THIS_ LPCSTR name, LPD3DXFRAME *new_frame)
	{
		// ����t���[���̍쐬
		FrameData *frame = new FrameData();
		ZeroMemory(frame, sizeof(FrameData));
		*new_frame = frame;

		// ���O�̃R�s�[
		if (name != NULL)
		{
			frame->Name = new char[lstrlenA(name) + 1];
			strcpy_s(frame->Name, lstrlenA(name) + 1, name);
		}
		else {
			frame->Name = NULL;
		}

		return S_OK;
	}

	STDMETHOD(CreateMeshContainer)(
		THIS_ LPCSTR name,
		CONST D3DXMESHDATA *mesh_data,
		CONST D3DXMATERIAL *material_data,
		CONST D3DXEFFECTINSTANCE *effect_instance,
		DWORD material_num,
		CONST DWORD *adjacency,
		LPD3DXSKININFO skin_info,
		LPD3DXMESHCONTAINER *new_mesh_container)
	{
		// ���b�V���̎擾
		LPD3DXMESH mesh = mesh_data->pMesh;

		// ���상�b�V���R���e�i�̍쐬
		MeshContainer *container = new MeshContainer;

		// �f�o�C�X
		LPDIRECT3DDEVICE9 device = NULL;

		// �|���S���̐�
		int porigon_num = 0;

		if (container == NULL)
		{
			return E_FAIL;
		}

		ZeroMemory(container, sizeof(MeshContainer));

		// ���O�̃R�s�[
		container->Name = new char[lstrlenA(name) + 1];
		strcpy_s(container->Name, lstrlenA(name) + 1, name);

		// �|���S�����̎擾
		porigon_num = mesh->GetNumFaces();

		// �f�o�C�X�̎擾
		mesh->GetDevice(&device);

		// ���b�V���̃R�s�[
		container->MeshData.pMesh = mesh;
		container->MeshData.Type = D3DXMESHTYPE_MESH;

		// �Q�ƃJ�E���^�����Z����
		mesh->AddRef();

		// �אڃ|���S���ԍ��̃R�s�[
		container->pAdjacency = new DWORD[porigon_num * 3];
		memcpy(container->pAdjacency, adjacency, sizeof(DWORD) * porigon_num * 3);

		// �}�e���A���̃R�s�[
		container->NumMaterials = material_num;
		if (material_num > 0)
		{
			container->pMaterials = new D3DXMATERIAL[container->NumMaterials];
			container->m_TextureList = new LPDIRECT3DTEXTURE9[container->NumMaterials];
			memcpy(container->pMaterials, material_data, sizeof(D3DXMATERIAL) * material_num);

			std::string path = "";

			// �t�@�C���ǂݍ���
			for (int i = 0; i < material_num; i++)
			{
				path = "";

				if (container->pMaterials[i].pTextureFilename != NULL)
				{
					if (g_TextureList[container->pMaterials[i].pTextureFilename] == NULL)
					{
						LPDIRECT3DTEXTURE9 texture;
						path = "asset/texture/";
						path += container->pMaterials[i].pTextureFilename;

						if (SUCCEEDED(D3DXCreateTextureFromFileA(
							device,
							path.c_str(),
							&texture)))
						{
							g_TextureList[container->pMaterials[i].pTextureFilename] = texture;
							container->m_TextureList[i] = texture;
						}
					}
					else {
						container->m_TextureList[i] = g_TextureList[container->pMaterials[i].pTextureFilename];
					}
				}
				else {
					container->m_TextureList[i] = NULL;
				}
			}
		}
		else 
		{
			// �}�e���A���̐���0�������玞�̉��̃}�e���A���ǉ�
			container->NumMaterials = 1;
			container->pMaterials = new D3DXMATERIAL[container->NumMaterials];
			container->m_TextureList = NULL;
			container->pMaterials[0].pTextureFilename = NULL;
			memset(&container->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
			container->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
			container->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
			container->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
			container->pMaterials[0].MatD3D.Diffuse.a = 1.0f;
			container->pMaterials[0].MatD3D.Ambient = container->pMaterials[0].MatD3D.Diffuse;
		}

		// �X�L����񂪂���Ώ����擾����
		if (skin_info != NULL)
		{
			container->pSkinInfo = skin_info;
			skin_info->AddRef();

			// �{�[���̐����擾
			DWORD bone_num = container->m_BoneNum = skin_info->GetNumBones();
			container->m_BoneOffsetMatrix = new D3DXMATRIX[bone_num];

			for (DWORD i = 0; i < bone_num; i++)
			{
				// �I�t�Z�b�g�s����R�s�[����
				memcpy(&container->m_BoneOffsetMatrix[i], container->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DXMATRIX));
			}

			// �{�[���̍œK��
			if (container->pSkinInfo->ConvertToBlendedMesh(
				mesh,						// ���̓��b�V��
				NULL,						// ���g�p
				container->pAdjacency,		// ���b�V���̗אڃf�[�^(����)
				NULL,						// ���b�V���̗אڃf�[�^(�o��)
				NULL,						// �|���S���̐V�K�C���f�b�N�X�̃o�b�t�@
				NULL,						// ���_�̐V�K�C���f�b�N�X�̃o�b�t�@
				&container->m_BoneWeightNum,// �P�̒��_�ɉe�����y�ڂ��d�݂̐�
				&container->m_BoneNum,		// �{�[���̐�
				&container->m_BoneBuffer,	// �{�[���f�[�^���i�[���ꂽ�o�b�t�@
				&container->MeshData.pMesh	// �ϊ���̃��b�V��
			))
			{
				return E_FAIL;
			}
		}

		*new_mesh_container = container;

		return S_OK;
	}

	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME frame)
	{
		if (frame != NULL)
		{
			if (frame->Name != NULL)
			{
				delete[](frame->Name);
			}
			delete(frame);
		}
		return S_OK;
	}

	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER base)
	{
		MeshContainer *container = (MeshContainer*)base;

		if (container->Name != NULL)
		{
			delete[](container->Name);
		}

		if (container->pAdjacency != NULL)
		{
			delete[](container->pAdjacency);
		}

		if (container->pMaterials != NULL)
		{
			delete[](container->pMaterials);
		}

		if (container->NumMaterials > 0)
		{
			for (int i = 0; i < container->NumMaterials; i++)
			{
				if (container->m_TextureList[i] != NULL)
				{
					container->m_TextureList[i]->Release();
					container->m_TextureList[i] = NULL;
				}
			}
			delete[](container->m_TextureList);
		}

		if (container->m_BoneMatrix != NULL)
		{
			delete[](container->m_BoneMatrix);
		}

		if (container->m_BoneOffsetMatrix != NULL)
		{
			delete[](container->m_BoneOffsetMatrix);
		}

		if (container->MeshData.pMesh != NULL)
		{
			container->MeshData.pMesh->Release();
		}

		delete(container);

		return S_OK;
	}
};

// �K�w�t��XFile
class SkinMeshFile
{
public:
	SkinMeshFile() :
		m_SkinMeshData(),
		m_RootFrame(NULL),
		m_AnimController(NULL)
	{}

	~SkinMeshFile()
	{
		if (m_SkinMeshData != NULL)
		{
			m_SkinMeshData->DestroyFrame(m_RootFrame);
			delete (m_SkinMeshData);
		}
	}

	bool Load(std::string file_name)
	{
		auto device = MyDirect3D_GetDevice();

		m_SkinMeshData = new SkinMeshData();

		if (FAILED(D3DXLoadMeshHierarchyFromXA(
			file_name.c_str(),
			D3DXMESH_MANAGED,
			device,
			m_SkinMeshData,
			NULL,
			&m_RootFrame,
			&m_AnimController)))
		{
			return false;
		}

		// �{�[���̍s��̈���m��
		AllocateAllBoneMatrix(m_RootFrame);
		m_AnimController->SetTrackSpeed(0, 0.5f);

		return true;
	}

	HRESULT AllocateBoneMatrix(LPD3DXMESHCONTAINER container)
	{
		FrameData *pFrame = NULL;
		DWORD bone_num = 0;

		MeshContainer *original_container = (MeshContainer*)container;
		if (original_container->pSkinInfo == NULL)
		{
			return S_OK;
		}

		bone_num = original_container->pSkinInfo->GetNumBones();

		// �{�[���̃}�g���N�X���̗̈���m�ۂ��ăf�[�^��ۑ�����
		original_container->m_BoneMatrix = new D3DXMATRIX*[bone_num];
		for (DWORD i = 0; i < bone_num; i++)
		{
			pFrame = (FrameData*)D3DXFrameFind(m_RootFrame, container->pSkinInfo->GetBoneName(i));
			if (pFrame == NULL)
			{
				return E_FAIL;
			}

			original_container->m_BoneMatrix[i] = &pFrame->m_CombinedTransformationMatrix;
		}

		return S_OK;
	}

	HRESULT AllocateAllBoneMatrix(LPD3DXFRAME frame)
	{
		if (frame->pMeshContainer != NULL)
		{
			if (FAILED(AllocateBoneMatrix(frame->pMeshContainer)))
			{
				return E_FAIL;
			}
		}

		// �q�������邩�ǂ���
		if (frame->pFrameFirstChild != NULL)
		{
			if (FAILED(AllocateAllBoneMatrix(frame->pFrameFirstChild)))
			{
				return E_FAIL;
			}
		}

		// �Z�킪���邩�ǂ���
		if (frame->pFrameSibling != NULL)
		{
			if (FAILED(AllocateAllBoneMatrix(frame->pFrameSibling)))
			{
				return E_FAIL;
			}
		}

		return S_OK;
	}

	void Draw(LPD3DXMATRIX matrix)
	{
		// �t���[���̍s����X�V
		UpdateFrame(m_RootFrame, matrix);

		// �t���[���`��
		DrawFrame(m_RootFrame);
	}

	void DrawFrame(LPD3DXFRAME frame)
	{
		FrameData *frame_data = (FrameData*)frame;
		LPD3DXMESHCONTAINER container_data = frame_data->pMeshContainer;

		// �R���e�i�̐������`�悷��
		while (container_data != NULL)
		{
			DrawMeshContainer(frame, container_data);

			container_data = container_data->pNextMeshContainer;
		}

		// �Z�킪����΍ċA�ŌĂяo��
		if (frame_data->pFrameSibling != NULL)
		{
			DrawFrame(frame_data->pFrameSibling);
		}

		// �q������΍ċA�ŌĂяo��
		if (frame_data->pFrameFirstChild != NULL)
		{
			DrawFrame(frame_data->pFrameFirstChild);
		}
	}

	void DrawMeshContainer(LPD3DXFRAME frame, LPD3DXMESHCONTAINER container)
	{
		auto device = MyDirect3D_GetDevice();

		FrameData *frame_data = (FrameData*)frame;
		MeshContainer *original_container = (MeshContainer*)container;

		if (original_container->pSkinInfo != NULL)
		{
			LPD3DXBONECOMBINATION bone_buffer = (LPD3DXBONECOMBINATION)original_container->m_BoneBuffer->GetBufferPointer();

			// �{�[���̐��܂킷
			for (DWORD i = 0; i < original_container->m_BoneNum; i++)
			{
				// �u�����h����{�[���̐�
				DWORD bone_blend_num = 0;

				// �{�[��ID����u�����h�����������o��
				for (DWORD j = 0; j < original_container->m_BoneWeightNum; j++)
				{
					if (bone_buffer[i].BoneId[j] != UINT_MAX)
					{
						bone_blend_num++;
					}
				}

				// ���_�u�����h�̐ݒ�
				// �������ɂ�_D3DVERTEXBLENDFLAGS���g���Ă���̂�
				// �ΏۂƂȂ��Ă���{�[���̐��ƍ��قɒ���
				device->SetRenderState(D3DRS_VERTEXBLEND, bone_blend_num - 1);

				for (DWORD j = 0; j < original_container->m_BoneWeightNum; j++)
				{
					DWORD matrix_index = bone_buffer[i].BoneId[j];
					D3DXMATRIX matrix;

					if (matrix_index != UINT_MAX)
					{
						// �I�t�Z�b�g�s��(m_BoneOffsetMatrix) * �{�[���̍s��(m_BoneMatrix)�ōŐV�̈ʒu������o��
						matrix = original_container->m_BoneOffsetMatrix[matrix_index] * (*original_container->m_BoneMatrix[matrix_index]);
						device->SetTransform(D3DTS_WORLDMATRIX(j), &matrix);
					}
				}

				device->SetMaterial(&original_container->pMaterials[bone_buffer[i].AttribId].MatD3D);
				device->SetTexture(0, original_container->m_TextureList[bone_buffer[i].AttribId]);
				original_container->MeshData.pMesh->DrawSubset(i);
			}
		}
		else 
		{
			// �`��ʒu�s��̐ݒ�
			device->SetTransform(D3DTS_WORLD, &frame_data->m_CombinedTransformationMatrix);

			// ���b�V���̕`��
			for (int i = 0; i < original_container->NumMaterials; i++)
			{
				device->SetMaterial(&original_container->pMaterials[i].MatD3D);
				device->SetTexture(0, original_container->m_TextureList[i]);
				original_container->MeshData.pMesh->DrawSubset(i);
			}
		}
	}

	void UpdateFrame(LPD3DXFRAME base, LPD3DXMATRIX parent_matrix)
	{
		FrameData *frame = (FrameData*)base;

		// �p���s��̍X�V
		if (parent_matrix != NULL)
		{
			D3DXMatrixMultiply(&frame->m_CombinedTransformationMatrix,
				&frame->TransformationMatrix,
				parent_matrix);
		}
		else
		{
			frame->m_CombinedTransformationMatrix = frame->TransformationMatrix;
		}

		// �Z�킪����΍ċA�ŌĂяo��
		if (frame->pFrameSibling != NULL)
		{
			UpdateFrame(frame->pFrameSibling, parent_matrix);
		}

		// �q��������΍ċA�ŌĂяo��
		if (frame->pFrameFirstChild != NULL)
		{
			UpdateFrame(frame->pFrameFirstChild,
				&frame->m_CombinedTransformationMatrix);
		}
	}

	void UpdateAnim()
	{
		if (m_AnimController != NULL)
		{
			// 0.016�b���A�j���[�V������i�߂�
			m_AnimController->AdvanceTime(0.016f, NULL);
		}
	}

private:
	// �K�w�f�[�^
	SkinMeshData *m_SkinMeshData;

	// ���[�g�t���[��
	LPD3DXFRAME m_RootFrame;

	// �A�j���[�V�����Ǘ�
	LPD3DXANIMATIONCONTROLLER m_AnimController;
};






SkinMeshFile* file;

void InitBone()
{
	file = new SkinMeshFile();
	file->Load("asset/mesh/tiny.x");
}

void DrawAnim()
{
	D3DXMATRIX world = TransformObject(D3DXVECTOR3(0, 10, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.05F, 0.05F, 0.05F));
	
	auto device = MyDirect3D_GetDevice();
	device->SetRenderState(D3DRS_LIGHTING, true);
	file->UpdateAnim();
	file->Draw(&world);
	device->SetRenderState(D3DRS_LIGHTING, false);
}





//LPD3DXFRAME topFrame = NULL;
//LPD3DXANIMATIONCONTROLLER animController = NULL;
//D3DXMATRIX* finalMatrices = NULL;
//int maxFrames;

////struct CUSTOM_FRAME : public D3DXFRAME
////{
////	D3DXMATRIX combinedTransformationMatrix;
////};
////
////struct CUSTOM_MESHCONTAINER : public D3DXMESHCONTAINER
////{
////	D3DXMATRIX** ppFrameMatrices;
////	LPD3DXMESH pFinalMesh;
////	LPDIRECT3DTEXTURE9 *pTexture;
////};
////
////
////class MeshAllocation : public ID3DXAllocateHierarchy
////{
////	STDMETHOD(CreateFrame)(LPCSTR name, LPD3DXFRAME* ppNewFrame)
////	{
////		// create custom frame
////		CUSTOM_FRAME* pFrame = new CUSTOM_FRAME;
////		*ppNewFrame = pFrame;
////		ZeroMemory(pFrame, sizeof(CUSTOM_FRAME));
////
////		// copy the name
////		if (name)
////		{
////			UINT len = strlen(name) + 1;
////			pFrame->Name = new char[len];
////			memcpy(pFrame->Name, name, len);
////		}
////
////		return S_OK;
////	}
////
////	STDMETHOD(CreateMeshContainer) (LPCSTR name, const D3DXMESHDATA* pMeshData, const D3DXMATERIAL* pMaterial,
////		const D3DXEFFECTINSTANCE* pEffectInstance, DWORD numMaterials, const DWORD* pAdjacency,
////		LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER* ppNewMeshContainer)
////	{
////		// get device
////		auto device = MyDirect3D_GetDevice();
////
////		// init custom mesh container
////		struct CUSTOM_MESHCONTAINER *pMeshContainer = new CUSTOM_MESHCONTAINER;
////		*ppNewMeshContainer = pMeshContainer;
////		ZeroMemory(pMeshContainer, sizeof(CUSTOM_MESHCONTAINER));
////
////		// check if it is a normal mesh
////		if (pMeshData->Type != D3DXMESHTYPE_MESH)
////		{
////			DestroyMeshContainer(pMeshContainer);
////			return E_FAIL;
////		}
////
////		// copy the name
////		if (name)
////		{
////			UINT len = strlen(name) + 1;
////			pMeshContainer->Name = new char[len];
////			memcpy(pMeshContainer->Name, name, len);
////		}
////
////		// copy the mesh data
////		pMeshContainer->MeshData.Type = pMeshData->Type;
////		pMeshContainer->MeshData.pMesh = pMeshData->pMesh;
////		pMeshContainer->MeshData.pMesh->AddRef();
////
////		// copy the materials
////		pMeshContainer->pMaterials = new D3DXMATERIAL[numMaterials];
////		for (DWORD i = 0; i < numMaterials; i++)
////		{
////			pMeshContainer->pMaterials[i] = pMaterial[i];
////			pMeshContainer->pMaterials[i].MatD3D.Ambient = pMaterial[i].MatD3D.Diffuse;
////		}
////
////		pMeshContainer->pEffects = NULL;
////		pMeshContainer->NumMaterials = max(1, numMaterials);
////
////		// copy the adjacency data
////		pMeshContainer->pAdjacency = new DWORD[pMeshData->pMesh->GetNumFaces() * 3];
////		memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * pMeshData->pMesh->GetNumFaces() * 3);
////
////		// copy skin info if available
////		if (pSkinInfo)
////		{
////			pMeshContainer->pSkinInfo = pSkinInfo;
////			pSkinInfo->AddRef();
////
////			pMeshContainer->ppFrameMatrices = new D3DXMATRIX*[pSkinInfo->GetNumBones()];
////			for (DWORD i = 0; i < pSkinInfo->GetNumBones(); i++)
////			{
////				pMeshContainer->ppFrameMatrices[i] = NULL;
////			}
////		}
////
////		// create a duplicate of the mesh
////		pMeshContainer->MeshData.pMesh->CloneMesh(D3DXMESH_MANAGED, NULL, device, &pMeshContainer->pFinalMesh);
////
////		// load the textures
////		pMeshContainer->pTexture = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
////		std::string path;
////		for (DWORD i = 0; i < numMaterials; i++)
////		{
////			pMeshContainer->pTexture[i] = 0;
////			if (pMaterial[i].pTextureFilename)
////			{
////				if (pMeshContainer->pMaterials[i].pTextureFilename)
////				{
////					path = "asset/texture/";
////					path += pMeshContainer->pMaterials[i].pTextureFilename;
////				}
////				else
////					path = "";
////
////				D3DXCreateTextureFromFileA(device, path.c_str(), &pMeshContainer->pTexture[i]);
////			}
////		}
////
////		if (pMeshContainer->pSkinInfo)
////			maxFrames = max(maxFrames, (int)pMeshContainer->pSkinInfo->GetNumBones());
////
////		return S_OK;
////	}
////
////	STDMETHOD(DestroyFrame)(LPD3DXFRAME pFrameToFree)
////	{
////		// free the name and the frame
////		SAFE_DELETE_ARRAY(pFrameToFree->Name);
////		SAFE_DELETE(pFrameToFree);
////
////		return S_OK;
////	}
////
////	STDMETHOD(DestroyMeshContainer)(LPD3DXMESHCONTAINER pMeshContainerToFree)
////	{
////		CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pMeshContainerToFree;
////
////		// free memory
////		SAFE_DELETE_ARRAY(pMeshContainer->Name);
////		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
////		SAFE_RELEASE(pMeshContainer->pFinalMesh);
////		SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
////		SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
////		SAFE_RELEASE(pMeshContainer->pSkinInfo);
////		SAFE_DELETE_ARRAY(pMeshContainer->ppFrameMatrices);
////		for (UINT i = 0; i < pMeshContainer->NumMaterials; i++)
////			SAFE_RELEASE(pMeshContainer->pTexture[i]);
////		SAFE_DELETE(pMeshContainer);
////
////		return S_OK;
////	}
////};
////
////void Link_Frames(CUSTOM_FRAME* pFrame)
////{
////	CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pFrame->pMeshContainer;
////
////	if (pMeshContainer && pMeshContainer->pSkinInfo)
////	{
////		// loop through each frame
////		for (UINT i = 0; i < pMeshContainer->pSkinInfo->GetNumBones(); i++)
////		{
////			CUSTOM_FRAME* pTempFrame;
////			pTempFrame = (CUSTOM_FRAME*)D3DXFrameFind(topFrame, pMeshContainer->pSkinInfo->GetBoneName(i));
////			pMeshContainer->ppFrameMatrices[i] = &pTempFrame->combinedTransformationMatrix;
////		}
////	}
////
////	// run for all siblings
////	if (pFrame->pFrameSibling)
////		Link_Frames((CUSTOM_FRAME*)pFrame->pFrameSibling);
////
////	// run for the first child which will then run all other children
////	if (pFrame->pFrameFirstChild)
////		Link_Frames((CUSTOM_FRAME*)pFrame->pFrameFirstChild);
////}
////
////void Update_Frames(CUSTOM_FRAME* pFrame, D3DXMATRIX* pParentMatrix)
////{
////	if (pParentMatrix)
////		pFrame->combinedTransformationMatrix = pFrame->TransformationMatrix * *pParentMatrix;
////	else
////		pFrame->combinedTransformationMatrix = pFrame->TransformationMatrix;
////
////	// run for all siblings
////	if (pFrame->pFrameSibling)
////		Update_Frames((CUSTOM_FRAME*)pFrame->pFrameSibling, pParentMatrix);
////
////	// run for the first child which will then run all other children
////	if (pFrame->pFrameFirstChild)
////		Update_Frames((CUSTOM_FRAME*)pFrame->pFrameFirstChild, &pFrame->combinedTransformationMatrix);
////}
////
////void Update_MeshContainers(CUSTOM_FRAME* pFrame)
////{
////	CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pFrame->pMeshContainer;
////
////	if (pMeshContainer && pMeshContainer->pSkinInfo)
////	{
////		UINT numFrames = pMeshContainer->pSkinInfo->GetNumBones();
////
////		// for each frame in the mesh container
////		for (UINT i = 0; i < numFrames; i++)
////		{
////			finalMatrices[i] = *pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
////			finalMatrices[i] *= *pMeshContainer->ppFrameMatrices[i];
////		}
////
////		// pointer to original and modified mesh
////		void* pSrc = NULL;
////		void *pDst = NULL;
////
////		// lock the two meshes
////		pMeshContainer->MeshData.pMesh->LockVertexBuffer(NULL, &pSrc);
////		pMeshContainer->pFinalMesh->LockVertexBuffer(NULL, &pDst);
////
////		// store the animated mesh into final mesh
////		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(finalMatrices, NULL, pSrc, pDst);
////
////		// unlock the two meshes
////		pMeshContainer->pFinalMesh->UnlockVertexBuffer();
////		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
////
////		// run for all siblings
////		if (pFrame->pFrameSibling)
////			Update_MeshContainers((CUSTOM_FRAME*)pFrame->pFrameSibling);
////
////		// run for the first child which will then run all other children
////		if (pFrame->pFrameFirstChild)
////			Update_MeshContainers((CUSTOM_FRAME*)pFrame->pFrameFirstChild);
////	}
////}
////
////void Draw_Mesh(CUSTOM_FRAME* pFrame)
////{
////	auto device = MyDirect3D_GetDevice();
////
////	CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pFrame->pMeshContainer;
////
////	if (pMeshContainer)
////	{
////		for (UINT i = 0; i < pMeshContainer->NumMaterials; i++)
////		{
////			device->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D);
////			device->SetTexture(0, pMeshContainer->pTexture[i]);
////
////			pMeshContainer->pFinalMesh->DrawSubset(i);
////		}
////	}
////
////	// run for all siblings
////	if (pFrame->pFrameSibling)
////		Draw_Mesh((CUSTOM_FRAME*)pFrame->pFrameSibling);
////
////	// run for the first child which will then run all other children
////	if (pFrame->pFrameFirstChild)
////		Draw_Mesh((CUSTOM_FRAME*)pFrame->pFrameFirstChild);
////}
////
////void InitFrameMesh()
////{
////	auto device = MyDirect3D_GetDevice();
////
////	MeshAllocation memAllocator;
////	LPCSTR str = "asset/mesh/tiny.x";
////	D3DXLoadMeshHierarchyFromX(str, D3DXMESH_MANAGED, device, &memAllocator, NULL, &topFrame, &animController);
////	finalMatrices = new D3DXMATRIX[maxFrames];
////	ZeroMemory(finalMatrices, sizeof(D3DXMATRIX) * maxFrames);
////	Link_Frames((CUSTOM_FRAME*)topFrame);
////}
////
////void DrawFrameMesh()
////{
////	TransformObject(D3DXVECTOR3(40, 3, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.5F, 0.5F, 0.5F));
////	if (animController)
////	{
////		static DWORD time = GetTickCount();
////		animController->AdvanceTime((GetTickCount() - time) * 0.001F, NULL);
////		time = GetTickCount();
////	}
////
////	Update_Frames((CUSTOM_FRAME*)topFrame, NULL);
////	Update_MeshContainers((CUSTOM_FRAME*)topFrame);
////	Draw_Mesh((CUSTOM_FRAME*)topFrame);
////}
