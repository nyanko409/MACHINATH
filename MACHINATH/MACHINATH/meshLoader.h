#pragma once

#include <d3dx9.h>
#include <string>
#include <map>
#include "mydirect3d.h"
#include "common.h"


// add mesh name to this enum
enum MESH_NAME
{
	MESH_COIN,
	MESH_SKATEBOARD,
	MESH_BOSS,
	MESH_WATER,
	MESH_MAP_STRAIGHT,
	MESH_MAP_CURVELEFT,
	MESH_MAP_CURVERIGHT,
	MESH_MAP_STRAIGHT_BRIDGE,
	MESH_MAP_STRAIGHT_UP,
	MESH_MAP_STRAIGHT_TUNNEL_DOWN,
	MESH_MAP_CLIFF,
	MESH_MAP_START
};

// for animated meshes
enum ANIMATED_MESH_NAME
{
	A_MESH_ROBOT
};

// data struct for unanimated meshes
struct MESH_DATA
{
	std::string meshPath;
	LPD3DXMESH mesh;
	D3DMATERIAL9* pMaterial;
	LPDIRECT3DTEXTURE9* pTexture;
	DWORD numMaterial;
};


// call once to load every mesh into memory
void LoadMesh();

// free mesh from memory
void UninitMesh();

// get unanimted mesh
MESH_DATA* GetMesh(MESH_NAME name);

// get animated mesh
class SkinMeshFile;
SkinMeshFile* GetAnimatedMesh(ANIMATED_MESH_NAME name);










static std::map<std::string, LPDIRECT3DTEXTURE9> g_TextureList;

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
						path = "asset/texture/_bone_mesh/";
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

	void Draw(LPD3DXMATRIX matrix, LPD3DXEFFECT shader = nullptr)
	{
		// �t���[���̍s����X�V
		UpdateFrame(m_RootFrame, matrix);

		// �t���[���`��
		DrawFrame(m_RootFrame, shader);
	}

	void DrawFrame(LPD3DXFRAME frame, LPD3DXEFFECT shader = nullptr)
	{
		FrameData *frame_data = (FrameData*)frame;
		LPD3DXMESHCONTAINER container_data = frame_data->pMeshContainer;

		// �R���e�i�̐������`�悷��
		while (container_data != NULL)
		{
			DrawMeshContainer(frame, container_data, shader);

			container_data = container_data->pNextMeshContainer;
		}

		// �Z�킪����΍ċA�ŌĂяo��
		if (frame_data->pFrameSibling != NULL)
		{
			DrawFrame(frame_data->pFrameSibling, shader);
		}

		// �q������΍ċA�ŌĂяo��
		if (frame_data->pFrameFirstChild != NULL)
		{
			DrawFrame(frame_data->pFrameFirstChild, shader);
		}
	}

	void DrawMeshContainer(LPD3DXFRAME frame, LPD3DXMESHCONTAINER container, LPD3DXEFFECT shader = nullptr)
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
						
						if(shader)
							shader->SetMatrix("World", &matrix);
						else
							device->SetTransform(D3DTS_WORLDMATRIX(j), &matrix);
					}
				}

				if(shader)
					shader->CommitChanges();
				else
				{
					device->SetMaterial(&original_container->pMaterials[bone_buffer[i].AttribId].MatD3D);
					device->SetTexture(0, original_container->m_TextureList[bone_buffer[i].AttribId]);
				}

				original_container->MeshData.pMesh->DrawSubset(i);
			}
		}
		else
		{
			// �`��ʒu�s��̐ݒ�
			if (shader)
			{
				shader->SetMatrix("World", &frame_data->m_CombinedTransformationMatrix);
				shader->CommitChanges();
			}
			else
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
			// update animation by time per frame
			m_AnimController->AdvanceTime(TIME_PER_FRAME, NULL);
		}
	}

	void SetAnimationSpeed(const UINT track, const float speed)
	{
		if (m_AnimController)
			m_AnimController->SetTrackSpeed(track, speed);
	}

	LPD3DXANIMATIONCONTROLLER GetAnimator() { return m_AnimController; }

private:
	// �K�w�f�[�^
	SkinMeshData *m_SkinMeshData;

	// ���[�g�t���[��
	LPD3DXFRAME m_RootFrame;

	// �A�j���[�V�����Ǘ�
	LPD3DXANIMATIONCONTROLLER m_AnimController;
};
