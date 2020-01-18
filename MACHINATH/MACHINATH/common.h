#pragma once

#include <d3dx9.h>


/*-----------------------------------------------------------------------
   ’è”’è‹`
------------------------------------------------------------------------*/
#define CUSTOM_FVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

/*
	FULLSCREEN: 1440 x 900 (wide)
	WINDOWED:	1920 x 1080
*/
#define FULLSCREEN		FALSE
#define SCREEN_WIDTH	(1920)
#define SCREEN_HEIGHT	(1080)

#define FPS				60
#define TIME_PER_FRAME	(float)1/FPS

#define SAFE_RELEASE(p)			if(p){ p->Release();	p = nullptr; }
#define SAFE_DELETE(p)			if(p){ delete p;		p = nullptr; }
#define SAFE_DELETE_ARRAY(p)	if(p){ delete[] p;		p = nullptr; }

// custom vertex struct
struct CUSTOM_VERTEX
{
	float x, y, z;
	D3DVECTOR normal;
	DWORD color;
	D3DXVECTOR2 uv;
};
