#pragma once
#include <windows.h>
#include <stdio.h>
#include "fmod.h"

// BGM ���� ���
#define SOUND_BGM_MAIN "content\\sound\\BGM\\Main_BGM.wav"
#define SOUND_BGM_INTRO "content\\sound\\BGM\\Main_BGM.wav"
#define SOUND_BGM_TUTORIAL "content\\sound\\BGM\\Tutorial_BGM.wav"
//#define SOUND_BGM_TUTORIAL_CUTSCENE "content\\sound\\BGM\\Title_BGM.wav"
#define SOUND_BGM_NOTE_CUTSCENE "content\\sound\\BGM\\Mother_BGM.wav"
#define SOUND_BGM_STAGE4 "content\\sound\\BGM\\Ending_point_BGM.wav"
#define SOUND_BGM_INGAME "content\\sound\\BGM\\Play_BGM.wav"
#define SOUND_BGM_HAPPYEND "content\\sound\\BGM\\Happy_ending_BGM.wav"
#define SOUND_BGM_BADEND "content\\sound\\BGM\\Bad_ending_BGM.wav"

// SFX ���� ���
#define SOUND_EFFECT_PUSH "content\\sound\\SFX\\Push_Effect.wav"
#define SOUND_EFFECT_MERGE "content\\sound\\SFX\\Merge_Effect.wav"
#define SOUND_EFFECT_FOOTSTEP "content\\sound\\SFX\\FootStep_Effect.wav"
#define SOUND_EFFECT_CLICK "content\\sound\\SFX\\Click_Effect.wav"
#define SOUND_EFFECT_RETRY "content\\sound\\SFX\\Retry_Effect.wav"
#define SOUND_EFFECT_PUZZLE "content\\sound\\SFX\\Puzzle_Effect.wav"

// �ý��� ������ ����
FMOD_SYSTEM* g_System;
void* extradriverdata;

// ����� ������ ��ü ����
FMOD_SOUND* g_Sound[15];
FMOD_SOUND* g_BGMSound[10];

// ä�� �迭 ����
FMOD_CHANNEL* g_Channel[15];
FMOD_CHANNEL* g_BGMChannel;

// ���� ���� ��� ����
FMOD_BOOL g_isSoundMute;
double g_volume;
double g_account;

// ä�� �ε���, bgm ���� �ε���
int Sound_ChannelIndex;
int g_BGMindex;

// sound enum
typedef enum MySound
{
    // BGM 
    SoundType_BGM_MAIN,
    SoundType_BGM_INTRO,
    SoundType_BGM_TUTORIAL,
    //SoundType_BGM_TUTORIAL_CUTSCENE,
    SoundType_BGM_NOTE_CUTSCENE,
    SoundType_BGM_STAGE4,
    SoundType_BGM_INGAME,
    SoundType_BGM_HAPPYEND,
    SoundType_BGM_BADEND,

    // SFX
    SoundType_Effect_PUSH,
    SoundType_Effect_MERGE,
    SoundType_Effect_FOOTSTEP,
    SoundType_Effect_CLICK,
    SoundType_Effect_RETRY,
    SoundType_Effect_PUZZLE

}SoundType;

// ���� ���̵� �ξƿ� ����ü
typedef enum SoundFadeINOUT
{
    Sound_State_Idle,
    Sound_State_FADEIN,
    Sound_State_FADEOUT

}Sound_State;

// ���̵� �ξƿ� ����ü ����
Sound_State SoundState;

// ���� �ý��� ���� �Լ�
void SoundInit();

// ���� �ý��� ���� �Լ�
void SoundSystemCreate();

// ���� ������Ʈ
void Sound_Update();

// ���̵� ��/�ƿ� �Լ�
void FadeIn();
void FadeOut();

// ���� ���� ���� �� ���� �Լ�
void SoundFileInput();

// ���� �÷��� �Լ�
void SoundPlay(int PlayIndex);

// BGM ���� ���� �Լ�
void SoundBGMchange(SoundType sound);

// ���� ���� �Լ�
void SetVolume(float volume);

// BGM�� �ٲٴµ� ���� �Լ� ������
void (*onChangeBGM)();

// �ƾ����� ���������� �ε��ϴµ� BGM ������ �ؾ��ϸ� ȣ���ϴ� �Լ�
void ChangeBGM_CutsceneToStage();

// ������������ �ƾ��� �ε��ϴµ� BGM ������ �ؾ��ϸ� ȣ���ϴ� �Լ�
void ChangeBGM_StageToCutscene();

// �ƾ����� �ƾ����� �Ѿ�µ� BGM ������ �ؾ��ϸ� ȣ���ϴ� �Լ�
void ChangeBGM_CutsceneToCutscene();

// �߰� or ������ ������ ���� Ÿ��Ʋ ȭ������ �Ѿ �� BGM �ٲٴ� �Լ�
void ChangeBGM_GoToMainTitle();

// ���� �б������� ���ư� �� ȣ�� �Ǵ� �Լ�
void ChangBGM_GotoEndPoint();

// ���� ��Ʈ �Լ�
// void SoundPlayer_StopAll();