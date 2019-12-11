#include <d3dx9.h>
#include <string>
#include "mydirect3d.h"
#include "font.h"

// fonts
LPD3DXFONT g_pFont = NULL;

HRESULT InitFont()
{
	//�t�H���g�̐��� MS�S�V�b�N
	//�f�o�C�X,��������,������,�t�H���g�X�^�C��,�~�b�v�}�b�v�̃��x��,�Α̂ɂ��邩�ǂ���,�����Z�b�g,�o�͐��x,�o�͕i��,�t�H���g�s�b�`�ƃt�@�~��,�t�H���g��,�t�H���g�|�C���^
	HRESULT hr = D3DXCreateFont(MyDirect3D_GetDevice(), 40, 0, FW_HEAVY, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, "�l�r �S�V�b�N", &g_pFont);
	if FAILED(hr) { return(E_FAIL); }

	return S_OK;
}

void DrawTextTo(RECT pos, const char* text, int textLength)
{
	g_pFont->DrawTextA(NULL, text, textLength, &pos, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 50));
}
