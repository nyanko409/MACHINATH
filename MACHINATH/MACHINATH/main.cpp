/*=======================================================================

  windowsの表示[main.cpp]
			 Author : Ato
			 Date   : 2018/4/22
-------------------------------------------------------------------------

========================================================================*/
#include<Windows.h>
#include<d3d9.h>		//DirectXの奴を選ぶ
#include<d3dx9.h>		//この二つは最低限必要
#include <chrono>
#include <thread>
#include "common.h"
#include "mydirect3d.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "effect.h"
#include "font.h"
#include "material.h"
#include "light.h"
#include "meshLoader.h"
#include "transform.h"
#include "transformation.h"
#include "gameObject.h"
#include "player.h"
#include "pickup.h"

//ライブラリファイルのリンク（exeファイルに含める）
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "dinput8.lib")

#define CLASS_NAME      "GameWindow"
#define WINDOW_CAPTION  "MACHINATH"


/*-----------------------------------------------------------------------
   プロトタイプ宣言
------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND g_hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//ゲーム関係初期化関数
static bool Initialize(void);

//ゲーム更新関数
static void Update(void);

//ゲーム描画関数
static void Draw(void);

//ゲーム終了の関数
static void Finalize(void);

// window
static HWND g_hWnd;

// timer
std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

void InitRenderState();
void InitLight();

// TEST
void DrawTriangle();
void InitModel();
GameObject* flooor;
GameObject* block;
GameObject* slime;
LPD3DXMESH pot;


/*-----------------------------------------------------------------------
   メイン
------------------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	RegisterClass(&wc);

	//ウィンドウスタイル（見た目）の設定
	DWORD window_style;
	if(FULLSCREEN) window_style = WS_EX_TOPMOST | WS_POPUP;
	else		   window_style = WS_OVERLAPPEDWINDOW;

	//基本矩形座標
	RECT window_rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//指定したクライアント領域に合わせて矩形を調整
	AdjustWindowRect(&window_rect, window_style, FALSE);

	//ウィンドウの矩形座標から幅と高さを計算
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	//ディスプレイの解像度の取得
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	//画面の中にウィンドウを表示するよう座標計算
	//ただし画面より大きいウィンドウは左上に合わせて表示
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	g_hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_CAPTION,
		window_style,
		window_x,
		window_y,
		window_width,
		window_height,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	//ウィンドウが作成できなかったら終了
	if (g_hWnd == NULL)
	{
		return 0;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// init
	if (!Initialize())
	{
		//初期化失敗
		return -1;
	}

	Keyboard_Initialize(hInstance, g_hWnd);

	MSG msg = {};

	while (WM_QUIT != msg.message) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// FPS 管理
			a = std::chrono::system_clock::now();
			std::chrono::duration<double, std::milli> work_time = a - b;

			if (work_time.count() < 1.0F/FPS * 1000)
			{
				std::chrono::duration<double, std::milli> delta_ms(1.0F / FPS * 1000 - work_time.count());
				auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
				std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
			}

			b = std::chrono::system_clock::now();

			// update and draw every frame
			Update();
			Draw();
		}
	}

	//ゲームの終了処理
	Finalize();
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND g_hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			SendMessage(g_hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case WM_CLOSE:
		if (MessageBox(g_hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(g_hWnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(g_hWnd, uMsg, wParam, lParam);
}

// ゲームの初期化
bool Initialize(void)
{
	//Direct3Dインターフェイス作成
	if (!MyDirect3D_Initialize(g_hWnd))
	{
		return false;
	}

	InitRenderState();
	
	InitCamera();
	InitFont();
	InitEffect();
	Texture_Load();
	LoadMesh();
	InitLight();

	InitPlayer();
	InitPickup();

	InitModel();

	return true;
}

//ゲーム更新関数
void Update(void)
{
	Keyboard_Update();

	UpdatePlayer();
	UpdatePickup();

	UpdateCamera();
	//std::thread t_input(Keyboard_Update);
	//std::thread t_camera(UpdateCamera);
	//t_input.join();
	//t_camera.join();
}

//ゲームの描画処理
void Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	
	//バックバッファーのクリア 紫色は230，0，255，255
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0F, 0);

	//絵画処理開始
	pDevice->BeginScene();

	DrawPlayer();
	DrawTriangle();
	DrawPickup();
	DrawEffect();

	pDevice->EndScene();

	//バックバッファを画面に表示する
	pDevice->Present(NULL, NULL, NULL, NULL);
}

//終了処理
void Finalize(void)
{
	UninitPlayer();
	UninitPickup();
	UninitCamera();
	UninitEffect();

	Texture_Release();
	MyDirect3D_Finalize();//DirectXの終了
}

// init render state
void InitRenderState()
{
	auto device = MyDirect3D_GetDevice();

	// init 3d rendering
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_ZENABLE, true);
	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));
	device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	// set alpha blending
	device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// set source of color and alpha
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);

	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	// set texture sampling
	device->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 8);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}

// init lights
void InitLight()
{
	AddDirectionalLight(0, D3DXVECTOR3(0.0F, -1.0F, 0.0F), D3DXCOLOR(.5F, .5F, .5F, 1.0F));
	AddPointLight(1, D3DXVECTOR3(10.0F, 3.0F, 0.0F), 100.0F, D3DXCOLOR(1.0F, 0.0F, 0.0F, 1.0F));
}








D3DXCOLOR operator - (D3DXCOLOR o1, float o2) { return D3DXCOLOR(o1.r - o2, o1.g - o2, o1.b - o2, o1.a); };

void InitModel()
{
	auto device = MyDirect3D_GetDevice();

	// teapot
	D3DXCreateTeapot(device, &pot, NULL);

	// room
	flooor = new GameObject(Transform(), MESH_FLOOR, nullptr);

	// block
	block = new GameObject(Transform(), MESH_BLOCK, nullptr);

	// slime
	slime = new GameObject(Transform(), MESH_EGG, nullptr);
}

void DrawTriangle()
{
	if (Keyboard_IsTrigger(DIK_V)) PlayEffect(EFFECT_EXPLOSION, 0, 5, 0);

	auto pDevice = MyDirect3D_GetDevice();

	//static float speed = 0.0F; speed += 0.1F;
	static float deg = 0.0F; deg += 3.0F;

	RotateLight(1, 0, 5, 0, false);
	
	// enable alpha blending
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	
	// set default material and texture
	SetMaterial();
	pDevice->SetTexture(0, NULL);

	// draw pot
	TransformObject(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, deg), D3DXVECTOR3(1.0F, 1.0F, 1.0F));
	pot->DrawSubset(0);

	// draw block
	TransformObject(D3DXVECTOR3(40, 40.0F, 0), D3DXVECTOR3(deg, deg, deg), D3DXVECTOR3(1.0F, 1.0F, 1.0F));
	for (DWORD i = 0; i < block->mesh->numMaterial; i++)
	{
		SetMaterial(&(block->mesh->material[i]));
		block->mesh->mesh->DrawSubset(i);
	}

	// draw floor
	TransformObject(D3DXVECTOR3(0, 0.0F, 0), D3DXVECTOR3(0, 0 , 0), D3DXVECTOR3(1.0F, 1.0F, 1.0F));
	for (DWORD i = 0; i < flooor->mesh->numMaterial; i++)
	{
		SetMaterial(&(flooor->mesh->material[i]));
	
		if (flooor->mesh->texture[i] != NULL)
			pDevice->SetTexture(0, flooor->mesh->texture[i]);
	
		flooor->mesh->mesh->DrawSubset(i);
	}

	// draw egg
	TransformObject(D3DXVECTOR3(100, 0, 0), D3DXVECTOR3(0, deg, 0), D3DXVECTOR3(0.5F, 0.5F, 0.5F));
	for (DWORD i = 0; i < slime->mesh->numMaterial; i++)
	{
		SetMaterial(&(slime->mesh->material[i]));
	
		if (slime->mesh->texture[i] != NULL)
			pDevice->SetTexture(0, slime->mesh->texture[i]);
	
		slime->mesh->mesh->DrawSubset(i);
	}

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}
