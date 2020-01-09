#pragma once

#include <Windows.h>


// MASTER AUDIO VOLUME
#define AUDIO_MASTER 1 //MUST NOT AUDIO_MASTER > 1 (ear rape)



//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE,			// TITLE BGM
	SOUND_LABEL_MAX
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
void UpdateSound(void);
void StopSound(void);

HRESULT PlaySound(SOUND_LABEL label, float volume = 1.0F);

void StopSound(SOUND_LABEL label);

HRESULT SetVolume(SOUND_LABEL LABEL, float volume, UINT32 OperationSet = 0);

void StartFade(SOUND_LABEL LABEL);

void UpdateFadeSound(SOUND_LABEL LABEL, float TargetVolume, float TargetTime);
