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
#include "fade.h"
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
#include "edge.h"
#include "countdown.h"

//���C�u�����t�@�C���̃����N�iexe�t�@�C���Ɋ܂߂�j
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "dinput8.lib")


// globals
#define CLASS_NAME      "GameWindow"
#define WINDOW_CAPTION  "MACHINATH"

static HWND g_hWnd;

// �v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND g_hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitRenderState();

static void InitLibrary();
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
void UpdateTest();
void InitTest();

/*-----------------------------------------------------------------------
   ���C��
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

	//�E�B���h�E�X�^�C���i�����ځj�̐ݒ�
	DWORD window_style;
	if(FULLSCREEN) window_style = WS_EX_TOPMOST | WS_POPUP;
	else		   window_style = WS_OVERLAPPEDWINDOW;

	//��{��`���W
	RECT window_rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//�w�肵���N���C�A���g�̈�ɍ��킹�ċ�`�𒲐�
	AdjustWindowRect(&window_rect, window_style, FALSE);

	//�E�B���h�E�̋�`���W���畝�ƍ������v�Z
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	//�f�B�X�v���C�̉𑜓x�̎擾
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	//��ʂ̒��ɃE�B���h�E��\������悤���W�v�Z
	//��������ʂ��傫���E�B���h�E�͍���ɍ��킹�ĕ\��
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

	//�E�B���h�E���쐬�ł��Ȃ�������I��
	if (g_hWnd == NULL)
	{
		return 0;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	ShowCursor(false);

	// init once
	InitLibrary();
	Keyboard_Initialize(hInstance, g_hWnd);

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
			// MAIN LOOP
			Sleep(6);
			Keyboard_Update();
			UpdateFade();

			switch (GetScene())
			{
			case SCENE_TITLESCREEN:
				if (!init_title)
				{
					FinalizeGame();
					InitTitle();
					init_game = false;
					init_title = true;
				}

				UpdateTitle();
				DrawTitle();
				break;
			case SCENE_GAMESCREEN:
				if (!init_game)
				{
					FinalizeTitle();
					InitGame();
					init_title = false;
					init_game = true;
				}

				UpdateGame();
				DrawGame();
				break;
			default:
				break;
			}
		}
	}

	//�Q�[���̏I������
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
		if (MessageBox(g_hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
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
void InitLibrary()
{
	//Direct3D�C���^�[�t�F�C�X�쐬
	MyDirect3D_Initialize(g_hWnd);

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
	InitFade();
	InitCountdown();
}

// free memory used in library
void FinalizeLibrary()
{
	UninitCountdown();
	UninitQTE();
	FinalizeTitle();
	UninitEffect();
	UninitShader();
	UninitCamera();
	UninitFont();
	UninitSound();
	UninitSprite();
	UninitMesh();
	UninitFade();
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
	UpdateSound();
}

// draw title
void DrawTitle()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//�o�b�N�o�b�t�@�[�̃N���A ���F��230�C0�C255�C255
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(80, 80, 255), 1.0F, 0);

	// draw 3d meshes
	pDevice->BeginScene();


	// draw 2d sprites
	SpriteStart();

	DrawTitleScreen();
	DrawFade();

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
	InitEdge();

	//InitTest();

	return true;
}

// update game
void UpdateGame()
{
	UpdateCountdown();
	UpdateSound();
	UpdateTimer();
	UpdateScore();
	UpdateMap();
	UpdatePlayer();
	UpdatePickup();
	UpdateLighting();
	UpdateCamera();
	UpdateQTE();
	UpdateEdge();

	UpdateTest();
}

// draw game
void DrawGame()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	
	//�o�b�N�o�b�t�@�[�̃N���A ���F��230�C0�C255�C255
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(80, 80, 255), 1.0F, 0);
	pDevice->BeginScene();

	// draw 3d meshes
	DrawObjects();
	DrawEffect();

	SpriteStart();

	// draw 2d sprites
	DrawQTE();
	DrawScore();
	DrawEdge();
	DrawCountdown();
	DrawFade();

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
	UninitEdge();
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

void UpdateTest()
{
	auto pDevice = MyDirect3D_GetDevice();
	//MoveLight(1, 0, 0, GetPlayer()->moveSpeed, false);

	if(Keyboard_IsTrigger(DIK_Z))
		FadeEdge(5, 20, 50, 255, 190);

	if (Keyboard_IsTrigger(DIK_X))
		StartQTE(QTE_MULTIPRESS);

	if (Keyboard_IsTrigger(DIK_C))
		StartQTE(QTE_DEFAULT);

	//PlayEffect(EFFECT_GOLD, { 0, 10, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, {1,1,1});

	// draw text
	//char f[] = "fuck";
	//DrawTextTo(RECT{300, 100, 100, 50}, f, sizeof(f) / sizeof(char));
}
