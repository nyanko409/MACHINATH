#pragma once

#include <Windows.h>


// MASTER AUDIO VOLUME
#define AUDIO_MASTER 1



//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE,			// TITLE BGM
	SOUND_LABEL_MAX
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
HRESULT SetVolume(SOUND_LABEL LABEL, float volume, UINT32 OperationSet = 0);
