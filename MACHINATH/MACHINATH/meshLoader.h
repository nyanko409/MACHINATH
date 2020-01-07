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

// 自作フレームデータ
struct FrameData : public D3DXFRAME
{
	// 合成行列
	D3DXMATRIXA16 m_CombinedTransformationMatrix;
};

// 自作メッシュコンテナ
struct MeshContainer : public D3DXMESHCONTAINER
{
	// マテリアルで使用するテクスチャ
	LPDIRECT3DTEXTURE9 *m_TextureList;

	// ウェイトの数
	DWORD m_BoneWeightNum;

	// ボーンの数
	DWORD m_BoneNum;

	// ボーン情報のバッファ
	LPD3DXBUFFER m_BoneBuffer;

	// 全ボーンのマトリックスのポインタの配列
	D3DXMATRIX **m_BoneMatrix;

	// ボーンのオフセット行列
	D3DXMATRIX *m_BoneOffsetMatrix;
};

// 階層クラス(ID3DXAllocateHierarchyで使う仮想関数のみ定義)
class SkinMeshData : public ID3DXAllocateHierarchy
{
public:
	SkinMeshData() {}

	STDMETHOD(CreateFrame)(THIS_ LPCSTR name, LPD3DXFRAME *new_frame)
	{
		// 自作フレームの作成
		FrameData *frame = new FrameData();
		ZeroMemory(frame, sizeof(FrameData));
		*new_frame = frame;

		// 名前のコピー
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
		// メッシュの取得
		LPD3DXMESH mesh = mesh_data->pMesh;

		// 自作メッシュコンテナの作成
		MeshContainer *container = new MeshContainer;

		// デバイス
		LPDIRECT3DDEVICE9 device = NULL;

		// ポリゴンの数
		int porigon_num = 0;

		if (container == NULL)
		{
			return E_FAIL;
		}

		ZeroMemory(container, sizeof(MeshContainer));

		// 名前のコピー
		container->Name = new char[lstrlenA(name) + 1];
		strcpy_s(container->Name, lstrlenA(name) + 1, name);

		// ポリゴン数の取得
		porigon_num = mesh->GetNumFaces();

		// デバイスの取得
		mesh->GetDevice(&device);

		// メッシュのコピー
		container->MeshData.pMesh = mesh;
		container->MeshData.Type = D3DXMESHTYPE_MESH;

		// 参照カウンタを加算する
		mesh->AddRef();

		// 隣接ポリゴン番号のコピー
		container->pAdjacency = new DWORD[porigon_num * 3];
		memcpy(container->pAdjacency, adjacency, sizeof(DWORD) * porigon_num * 3);

		// マテリアルのコピー
		container->NumMaterials = material_num;
		if (material_num > 0)
		{
			container->pMaterials = new D3DXMATERIAL[container->NumMaterials];
			container->m_TextureList = new LPDIRECT3DTEXTURE9[container->NumMaterials];
			memcpy(container->pMaterials, material_data, sizeof(D3DXMATERIAL) * material_num);

			std::string path = "";

			// ファイル読み込み
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
			// マテリアルの数が0だったら時の仮のマテリアル追加
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

		// スキン情報があれば情報を取得する
		if (skin_info != NULL)
		{
			container->pSkinInfo = skin_info;
			skin_info->AddRef();

			// ボーンの数を取得
			DWORD bone_num = container->m_BoneNum = skin_info->GetNumBones();
			container->m_BoneOffsetMatrix = new D3DXMATRIX[bone_num];

			for (DWORD i = 0; i < bone_num; i++)
			{
				// オフセット行列をコピーする
				memcpy(&container->m_BoneOffsetMatrix[i], container->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DXMATRIX));
			}

			// ボーンの最適化
			if (container->pSkinInfo->ConvertToBlendedMesh(
				mesh,						// 入力メッシュ
				NULL,						// 未使用
				container->pAdjacency,		// メッシュの隣接データ(入力)
				NULL,						// メッシュの隣接データ(出力)
				NULL,						// ポリゴンの新規インデックスのバッファ
				NULL,						// 頂点の新規インデックスのバッファ
				&container->m_BoneWeightNum,// １つの頂点に影響を及ぼす重みの数
				&container->m_BoneNum,		// ボーンの数
				&container->m_BoneBuffer,	// ボーンデータが格納されたバッファ
				&container->MeshData.pMesh	// 変換後のメッシュ
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

// 階層付きXFile
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

		// ボーンの行列領域を確保
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

		// ボーンのマトリクス分の領域を確保してデータを保存する
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

		// 子供があるかどうか
		if (frame->pFrameFirstChild != NULL)
		{
			if (FAILED(AllocateAllBoneMatrix(frame->pFrameFirstChild)))
			{
				return E_FAIL;
			}
		}

		// 兄弟があるかどうか
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
		// フレームの行列を更新
		UpdateFrame(m_RootFrame, matrix);

		// フレーム描画
		DrawFrame(m_RootFrame, shader);
	}

	void DrawFrame(LPD3DXFRAME frame, LPD3DXEFFECT shader = nullptr)
	{
		FrameData *frame_data = (FrameData*)frame;
		LPD3DXMESHCONTAINER container_data = frame_data->pMeshContainer;

		// コンテナの数だけ描画する
		while (container_data != NULL)
		{
			DrawMeshContainer(frame, container_data, shader);

			container_data = container_data->pNextMeshContainer;
		}

		// 兄弟がいれば再帰で呼び出す
		if (frame_data->pFrameSibling != NULL)
		{
			DrawFrame(frame_data->pFrameSibling, shader);
		}

		// 子がいれば再帰で呼び出す
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

			// ボーンの数まわす
			for (DWORD i = 0; i < original_container->m_BoneNum; i++)
			{
				// ブレンドするボーンの数
				DWORD bone_blend_num = 0;

				// ボーンIDからブレンドする個数を割り出す
				for (DWORD j = 0; j < original_container->m_BoneWeightNum; j++)
				{
					if (bone_buffer[i].BoneId[j] != UINT_MAX)
					{
						bone_blend_num++;
					}
				}

				// 頂点ブレンドの設定
				// 第二引数には_D3DVERTEXBLENDFLAGSが使われているので
				// 対象となっているボーンの数と差異に注意
				device->SetRenderState(D3DRS_VERTEXBLEND, bone_blend_num - 1);

				for (DWORD j = 0; j < original_container->m_BoneWeightNum; j++)
				{
					DWORD matrix_index = bone_buffer[i].BoneId[j];
					D3DXMATRIX matrix;

					if (matrix_index != UINT_MAX)
					{
						// オフセット行列(m_BoneOffsetMatrix) * ボーンの行列(m_BoneMatrix)で最新の位置を割り出す
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
			// 描画位置行列の設定
			if (shader)
			{
				shader->SetMatrix("World", &frame_data->m_CombinedTransformationMatrix);
				shader->CommitChanges();
			}
			else
				device->SetTransform(D3DTS_WORLD, &frame_data->m_CombinedTransformationMatrix);

			// メッシュの描画
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

		// 姿勢行列の更新
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

		// 兄弟があれば再帰で呼び出す
		if (frame->pFrameSibling != NULL)
		{
			UpdateFrame(frame->pFrameSibling, parent_matrix);
		}

		// 子供がいれば再帰で呼び出す
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
	// 階層データ
	SkinMeshData *m_SkinMeshData;

	// ルートフレーム
	LPD3DXFRAME m_RootFrame;

	// アニメーション管理
	LPD3DXANIMATIONCONTROLLER m_AnimController;
};
