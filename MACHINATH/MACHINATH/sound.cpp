//=============================================================================
//
// �T�E���h���� [sound.cpp]
//
//=============================================================================
#include <XAudio2.h>
#include <mmsystem.h>
#include "sound.h"
#include "common.h"

#pragma comment (lib, "xaudio2.lib") 
#pragma comment (lib, "winmm.lib") 

//*****************************************************************************
// sound parameters
//*****************************************************************************
struct SOUNDPARAM
{
	const char *pFilename;	// path to sound file
	int nCntLoop;			// -1 == LOOP EDNLESSLY, 0 == DONT LOOP, >= 1 == LOOP
};

//*****************************************************************************
// methods
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
void UpdateFadeSound();

//*****************************************************************************
// globals
//*****************************************************************************
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2 interface
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// master voice
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// source voice
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};

float g_DeltaTime;
float g_targetTime, g_targetVolume;
bool g_FadeFlag;
float g_curVolume;
SOUND_LABEL g_curFadeSound;

// sound files to load
SOUNDPARAM g_aParam[SOUND_LABEL_MAX] =
{
	{"asset/sound/BGM/title.wav", -1},			// title bgm
	{"asset/sound/BGM/game.wav", -1},			// ingame bgm
};

//=============================================================================
// init sound
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	g_DeltaTime = 0;
	g_FadeFlag = false;

	HRESULT hr;

	// enable multithread for sound playback
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// initialize xaudio2
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "Failed to initialize XAudio2!", "Error!", MB_ICONWARNING);

		CoUninitialize();

		return E_FAIL;
	}
	
	// initialize master voice
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "Failed to initialize Mastering Voice!", "Error!", MB_ICONWARNING);

		if(g_pXAudio2)
		{
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		CoUninitialize();

		return E_FAIL;
	}

	// load sound files
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// load sound files from given path
		hFile = CreateFile(g_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "Invalid Handle Value!", "Error!", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			MessageBox(hWnd, "Invalid Set File Pointer!", "Error!", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "Failed at checking chunk!", "Error!", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "Failed to read chunk data!", "Error!", MB_ICONWARNING);
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "Invalid file type!", "Error!", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// create source voice
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "Failed at creating source voice!", "Error!", MB_ICONWARNING);
			return S_FALSE;
		}

		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aParam[nCntSound].nCntLoop;

		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
	}

	return S_OK;
}

//=============================================================================
// uninit sound
//=============================================================================
void UninitSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound])
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;
	
			// �I�[�f�B�I�f�[�^�̊J��
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	if(g_pXAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// Update Sound
//=============================================================================
void UpdateSound(void)
{
	if (g_FadeFlag)
		UpdateFadeSound();
}

//=============================================================================
// Play Sound
//=============================================================================
HRESULT PlaySound(SOUND_LABEL label, float volume)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// init buffer
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = g_aParam[label].nCntLoop;

	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{
		g_apSourceVoice[label]->Stop(0);
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// submit source buffer
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// play sound
	g_apSourceVoice[label]->Start(0);

	// set volume
	SetVolume(label, volume);

	return S_OK;
}

//=============================================================================
// Stop Sound
//=============================================================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// Stop all sound
//=============================================================================
void StopSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound])
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//=============================================================================
// check chunk
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^��`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// read cunk data
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}


HRESULT SetVolume(SOUND_LABEL LABEL,float volume,UINT32 OperationSet)
{
	return g_apSourceVoice[LABEL]->SetVolume(volume * AUDIO_MASTER, OperationSet);
}

float GetVolume(SOUND_LABEL label)
{
	float vol;
	g_apSourceVoice[label]->GetVolume(&vol);
	return vol;
}

void UpdateFadeSound()
{
	if (g_DeltaTime < g_targetTime)
	{
		SetVolume(g_curFadeSound, g_targetVolume * (g_DeltaTime/ g_targetTime) + 
			g_curVolume * ((g_targetTime - g_DeltaTime )/ g_targetTime));
		g_DeltaTime += (1.0f / 60.0f);
	}
	else
	{
		SetVolume(g_curFadeSound, g_targetVolume);
		if (g_targetVolume == 0)
		{
			StopSound(g_curFadeSound);
		}

		g_DeltaTime = 0;
		g_FadeFlag = false;
	}
}

void StartFade(SOUND_LABEL label, float targetVolume, float targetTime)
{
	// return if a sound is currently fading
	if (g_FadeFlag) return;

	g_curFadeSound = label;
	g_apSourceVoice[label]->GetVolume(&g_curVolume);
	g_targetVolume = targetVolume;
	g_targetTime = targetTime;
	g_FadeFlag = true;
}
