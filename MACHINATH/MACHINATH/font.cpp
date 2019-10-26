#include <d3dx9.h>
#include <string>
#include "mydirect3d.h"
#include "font.h"

// fonts
LPD3DXFONT pFont = NULL;
LPD3DXFONT pBigFont = NULL;

HRESULT InitFont()
{
	//フォントの生成 MSゴシック
	//デバイス,文字高さ,文字幅,フォントスタイル,ミップマップのレベル,斜体にするかどうか,文字セット,出力精度,出力品質,フォントピッチとファミリ,フォント名,フォントポインタ
	HRESULT hr = D3DXCreateFont(MyDirect3D_GetDevice(), 40, 0, FW_HEAVY, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, "ＭＳ ゴシック", &pFont);
	if FAILED(hr) { return(E_FAIL); }

	hr = D3DXCreateFont(MyDirect3D_GetDevice(), 300, 0, FW_HEAVY, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, "ＭＳ ゴシック", &pBigFont);
	if FAILED(hr) { return(E_FAIL); }

	return S_OK;
}

void DrawTextTo(RECT pos, int text, int fontType)
{
	LPD3DXFONT font = NULL;
	if (fontType == 0) font = pFont;
	if (fontType == 1) font = pBigFont;

	auto str = std::to_string(text);

	pFont->DrawTextA(NULL, str.c_str(), -1, &pos, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
}