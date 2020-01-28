#pragma once

#include <Windows.h>


// MASTER AUDIO VOLUME
#define AUDIO_MASTER 1 //MUST NOT AUDIO_MASTER > 1 (ear rape)



//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	AUDIO_BGM_TITLE	,		// TITLE BGM
	AUDIO_BGM_GAME,			// INGAME BGM
	AUIDO_SE_PICKUP,		// pickup sound effect
	AUDIO_SE_GETREADY,
	AUDIO_SE_ONE,
	AUDIO_SE_TWO,
	AUDIO_SE_THREE,
	AUDIO_SE_GO,
	AUDIO_SE_TITLE_PUSHBUTTON,

	SOUND_LABEL_MAX
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
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

