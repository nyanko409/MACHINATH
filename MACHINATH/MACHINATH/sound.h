#pragma once

#include <Windows.h>


// MASTER AUDIO VOLUME
#define AUDIO_MASTER 1 //MUST NOT AUDIO_MASTER > 1 (ear rape)



//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	AUDIO_BGM_TITLE,
	AUDIO_BGM_GAME,	
	AUIDO_SE_PICKUP,
	AUDIO_SE_GETREADY,
	AUDIO_SE_ONE,
	AUDIO_SE_TWO,
	AUDIO_SE_THREE,
	AUDIO_SE_GO,
	AUDIO_SE_TITLE_PUSHBUTTON,
	AUDIO_SE_TITLE_NOISE,
	AUDIO_SE_SLOWMO_START,
	AUDIO_SE_SLOWMO_END,
	AUDIO_SE_QTE_FAIL,
	AUDIO_SE_QTE_SUCCESS,
	AUDIO_SE_QTE_STANDBY,
	AUDIO_SE_EXPLOSION,

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

float GetVolume(SOUND_LABEL label);

void StartFade(SOUND_LABEL label, float targetVolume, float targetTime);

void SetPlaybackSpeed(SOUND_LABEL label, float speed);
