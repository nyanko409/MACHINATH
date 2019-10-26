#pragma once


/*-----------------------------------------------------------------------
   íËêîíËã`
------------------------------------------------------------------------*/
#define CUSTOM_FVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

/*
	FULLSCREEN: 1440 x 900 (wide)
	WINDOWED:	2560 x 1440
*/
#define FULLSCREEN		false
#define SCREEN_WIDTH	(2560)
#define SCREEN_HEIGHT	(1440)
#define FPS				60

// custom vertex struct
struct CUSTOM_VERTEX
{
	float x, y, z;
	D3DVECTOR normal;
	DWORD color;
	D3DXVECTOR2 uv;
};
