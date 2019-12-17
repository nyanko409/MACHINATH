#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <thread>
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

static bool Initialize(void);
static void Update(void);
static void Draw(void);
static void Finalize(void);

void InitRenderState();
void InitLight();

// TEST
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
			// update and draw every frame
			Sleep(7);
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
	
	InitShader();
	InitCamera();
	InitFont();
	InitEffect();
	Texture_Load();
	LoadMesh();
	InitSprite();
	InitLight();

	InitBillboard();
	InitQTE();
	InitTitle();
	InitScore();
	InitPlayer();
	InitPickup();
	InitMap();

	InitTest();

	return true;
}

//ゲーム更新関数
void Update(void)
{
	Keyboard_Update();

	UpdateTitle();

	UpdateScene();
	UpdateTimer();
	UpdateScore();
	UpdateMap();
	UpdatePlayer();
	UpdatePickup();
	UpdateQTE();
	UpdateBillboard();

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
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(80, 80, 255), 1.0F, 0);

	// draw 3d meshes
	pDevice->BeginScene();


	DrawObjects();
	DrawTest();
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DrawBillboard();
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DrawEffect();

	// draw 2d sprites
	SpriteStart();

	DrawTitle();

	DrawQTE();
	DrawScore();

	SpriteEnd();

	// display text and finish
	DrawTimer();
	pDevice->EndScene();

	// draw to screen
	pDevice->Present(NULL, NULL, NULL, NULL);
}

//終了処理
void Finalize(void)
{
	UninitBillboard();
	UninitQTE();
	UninitPlayer();
	UninitMap();
	UninitPickup();
	UninitCamera();
	UninitEffect();
	UninitShader();

	Texture_Release();
	MyDirect3D_Finalize();
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

// init lighting
void InitLight()
{
	AddDirectionalLight(0, D3DXVECTOR3(0.0F, -1.0F, 1.0F), D3DXCOLOR(0.0F, 1.0F, 1.0F, 1.0F));
	AddPointLight(1, D3DXVECTOR3(0, 5, 0), 100, D3DXCOLOR(0.0F, 1.0F, 1.0F, 1.0F));
}









void InitTest()
{
	auto pDevice = MyDirect3D_GetDevice();
	//SetMaterial();
}

void DrawTest()
{
	auto pDevice = MyDirect3D_GetDevice();
	//MoveLight(1, 0, 0, GetPlayer()->moveSpeed, false);

	// draw text
	char f[] = "fuck";
	DrawTextTo(RECT{300, 100, 100, 50}, f, sizeof(f) / sizeof(char));
}
