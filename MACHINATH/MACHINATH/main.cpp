#include <Windows.h>
#include <d3dx9.h>
#include <time.h>
#include <vector>
#include "common.h"
#include "mydirect3d.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "effect.h"
#include "font.h"
#include "material.h"
#include "light.h"
#include "sprite.h"
#include "meshLoader.h"
#include "transform.h"
#include "transformation.h"
#include "gameObject.h"
#include "lighting.h"
#include "player.h"
#include "pickup.h"
#include "playTime.h"
#include "sceneManagement.h"
#include "mesh.h"
#include "shader.h"
#include "qte.h"
#include "score.h"
#include "map.h"
#include "billboard.h"
#include "Title.h"
#include "sound.h"
#include "collider.h"
#include "water.h"

//ライブラリファイルのリンク（exeファイルに含める）
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "dinput8.lib")


// globals
#define CLASS_NAME      "GameWindow"
#define WINDOW_CAPTION  "MACHINATH"

static HWND g_hWnd;

// プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND g_hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitRenderState();

static bool InitLibrary();
static void FinalizeLibrary();

static bool InitTitle();
static void UpdateTitle();
static void DrawTitle();
static void FinalizeTitle();

static bool InitGame();
static void UpdateGame();
static void DrawGame();
static void FinalizeGame();


// TEST
void InitLight();
void DrawTest();
void InitTest();


/*-----------------------------------------------------------------------
   メイン
------------------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	srand(time(NULL));

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

	// init library once
	if (!InitLibrary())
	{
		//初期化失敗
		return -1;
	}

	Keyboard_Initialize(hInstance, g_hWnd);

	ShowCursor(false);

	MSG msg = {};
	bool init_title = false;
	bool init_game = false;

	while (WM_QUIT != msg.message) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Sleep(6);
			Keyboard_Update();

			// update title
			if (GetScene() == SCENE_TITLESCREEN)
			{
				// free memory used in game and init title screen
				if (!init_title)
				{
					FinalizeGame();
					InitTitle();
					init_game = false;
					init_title = true;
				}

				UpdateTitle();
				DrawTitle();
			}

			// update game
			else if (GetScene() == SCENE_GAMESCREEN)
			{
				// free memory used in title and init title screen
				if (!init_game)
				{
					FinalizeTitle();
					InitGame();
					init_title = false;
					init_game = true;
				}

				UpdateGame();
				DrawGame();
			}
		}
	}

	//ゲームの終了処理
	FinalizeTitle();
	FinalizeGame();
	FinalizeLibrary();
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND g_hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			ShowCursor(true);
			SendMessage(g_hWnd, WM_CLOSE, 0, 0);
			ShowCursor(false);
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

// load tasks that only need to be initialized once
bool InitLibrary()
{
	//Direct3Dインターフェイス作成
	if (!MyDirect3D_Initialize(g_hWnd))
	{
		return false;
	}

	InitRenderState();
	InitSound(g_hWnd);
	InitFont();
	InitCamera();
	InitEffect();
	Texture_Load();
	LoadMesh();
	InitSprite();
	InitScene();
	InitQTE();
	InitShader();

	return true;
}

// free memory used in library
void FinalizeLibrary()
{
	UninitQTE();
	FinalizeTitle();
	UninitEffect();
	UninitShader();
	UninitCamera();
	UninitFont();
	UninitSound();
	UninitSprite();
	UninitMesh();
	Texture_Release();
	MyDirect3D_Finalize();
}

// init title screen
bool InitTitle()
{
	InitTitleScreen();
	return true;
}

// update title
void UpdateTitle()
{
	UpdateTitleScreen();
}

// draw title
void DrawTitle()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//バックバッファーのクリア 紫色は230，0，255，255
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(80, 80, 255), 1.0F, 0);

	// draw 3d meshes
	pDevice->BeginScene();


	// draw 2d sprites
	SpriteStart();

	DrawTitleScreen();

	SpriteEnd();
	pDevice->EndScene();

	// draw to screen
	pDevice->Present(NULL, NULL, NULL, NULL);
}

// free memory used in title screen
void FinalizeTitle()
{
	FinalizeTitleScreen();
}

// init game screen
bool InitGame()
{
	InitLighting();
	InitScore();
	InitPlayer();
	InitPickup();
	InitMap();

	InitTest();

	return true;
}

// update game
void UpdateGame()
{
	UpdateTimer();
	UpdateScore();
	UpdateMap();
	//UpdatePlayer();
	UpdatePickup();
	UpdateLighting();
	UpdateCamera();
	UpdateQTE();
}

// draw game
void DrawGame()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	
	//バックバッファーのクリア 紫色は230，0，255，255
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(80, 80, 255), 1.0F, 0);
	pDevice->BeginScene();

	// draw 3d meshes
	DrawObjects();
	DrawEffect();

	SpriteStart();

	// draw 2d sprites
	DrawQTE();
	DrawScore();

	// display text and finish
	SpriteEnd();
	DrawTimer();
	pDevice->EndScene();

	// draw to screen
	pDevice->Present(NULL, NULL, NULL, NULL);
}

// free memory used in game screen
void FinalizeGame()
{
	UninitPlayer();
	UninitMap();
	UninitPickup();
	UninitLighting();
}

// init render state
void InitRenderState()
{
	auto device = MyDirect3D_GetDevice();

	// init 3d rendering
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	device->SetRenderState(D3DRS_ZENABLE, true);
	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(100, 100, 100));
	device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	// set alpha blending
	device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	device->SetRenderState(D3DRS_ALPHAREF, 0x80);

	// set source of color and alpha
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// set texture sampling
	device->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 8);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}	









Water* water;
MeshObject* boss;

void InitTest()
{
	auto pDevice = MyDirect3D_GetDevice();
	
	Transform trans({ 0, 0, 0 }, {0, 0, 0}, {0, 0, 0}, {10, 1, 10});
	water = new Water(trans, MESH_WATER, SHADER_WATER);

	trans.position.y += 30;
	trans.position.x += 150;
	trans.scale = { 1,1,1 };
	//boss = new MeshObject(trans, MESH_BOSS, SHADER_DEFAULT);
}

void DrawTest()
{
	auto pDevice = MyDirect3D_GetDevice();
	//MoveLight(1, 0, 0, GetPlayer()->moveSpeed, false);

	if(Keyboard_IsTrigger(DIK_Z))
		PlayEffect(EFFECT_JUMP, { 0, 10, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, {0.1,0.1,0.1});

	// draw text
	//char f[] = "fuck";
	//DrawTextTo(RECT{300, 100, 100, 50}, f, sizeof(f) / sizeof(char));
}
