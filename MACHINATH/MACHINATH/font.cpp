#include <d3dx9.h>
#include <string>
#include "mydirect3d.h"
#include "font.h"

// fonts
LPD3DXFONT g_pFont = NULL;

HRESULT InitFont()
{
	//フォントの生成 MSゴシック
	//デバイス,文字高さ,文字幅,フォントスタイル,ミップマップのレベル,斜体にするかどうか,文字セット,出力精度,出力品質,フォントピッチとファミリ,フォント名,フォントポインタ
	HRESULT hr = D3DXCreateFont(MyDirect3D_GetDevice(), 40, 0, FW_HEAVY, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, "ＭＳ ゴシック", &g_pFont);
	if FAILED(hr) { return(E_FAIL); }

	return S_OK;
}

void DrawTextTo(RECT pos, const char* text, int textLength)
{
	g_pFont->DrawTextA(NULL, text, textLength, &pos, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 50));
}
