#pragma once

#include <d3dx9.h>

bool MyDirect3D_Initialize(HWND hWnd);

void MyDirect3D_Finalize(void);

LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void);
