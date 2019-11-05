#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>
#include "common.h"

static LPDIRECT3D9 g_pD3D = NULL;	//Direct3D インターフェース

static LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	//Direct3Dデバイス インターフェース


LPDIRECT3DDEVICE9 MyDirect3D_GetDevice()
{
	return g_pD3DDevice;
}

void MyDirect3D_Finalize()
{
	//デバイスの解放
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct 3Dの解放
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}

bool MyDirect3D_Initialize(HWND hWnd)
{
	//Direct3Dインターフェース作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);	//インターフェースのポインタ
	if (g_pD3D == NULL)
	{
		//作成失敗
		MessageBox(hWnd, "Direct3Dインターフェース作成失敗", "Initialize Error", MB_OK);
		return false;
	}

	//デバイス作成用構造体の用意
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = !FULLSCREEN;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.BackBufferCount = 1;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// enable/disable vsync
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//Direct 3Dデバイスの作成
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//デバイス作成失敗
		MessageBox(hWnd, "Direct3Dデバイス作成失敗", "Initialize Error", MB_OK);
		return false;
	}

	return true;	//初期化成功
}