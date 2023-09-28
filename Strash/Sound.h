#pragma once
#include <windows.h>
#include <stdio.h>
#include "fmod.h"

// BGM 파일 경로
#define SOUND_BGM_MAIN "content\\sound\\BGM\\Main_BGM.wav"
#define SOUND_BGM_INTRO "content\\sound\\BGM\\Main_BGM.wav"
#define SOUND_BGM_TUTORIAL "content\\sound\\BGM\\Tutorial_BGM.wav"
//#define SOUND_BGM_TUTORIAL_CUTSCENE "content\\sound\\BGM\\Title_BGM.wav"
#define SOUND_BGM_NOTE_CUTSCENE "content\\sound\\BGM\\Mother_BGM.wav"
#define SOUND_BGM_STAGE4 "content\\sound\\BGM\\Ending_point_BGM.wav"
#define SOUND_BGM_INGAME "content\\sound\\BGM\\Play_BGM.wav"
#define SOUND_BGM_HAPPYEND "content\\sound\\BGM\\Happy_ending_BGM.wav"
#define SOUND_BGM_BADEND "content\\sound\\BGM\\Bad_ending_BGM.wav"

// SFX 파일 경로
#define SOUND_EFFECT_PUSH "content\\sound\\SFX\\Push_Effect.wav"
#define SOUND_EFFECT_MERGE "content\\sound\\SFX\\Merge_Effect.wav"
#define SOUND_EFFECT_FOOTSTEP "content\\sound\\SFX\\FootStep_Effect.wav"
#define SOUND_EFFECT_CLICK "content\\sound\\SFX\\Click_Effect.wav"
#define SOUND_EFFECT_RETRY "content\\sound\\SFX\\Retry_Effect.wav"
#define SOUND_EFFECT_PUZZLE "content\\sound\\SFX\\Puzzle_Effect.wav"

// 시스템 포인터 선언
FMOD_SYSTEM* g_System;
void* extradriverdata;

// 사용할 사운드의 전체 개수
FMOD_SOUND* g_Sound[15];
FMOD_SOUND* g_BGMSound[10];

// 채널 배열 선언
FMOD_CHANNEL* g_Channel[15];
FMOD_CHANNEL* g_BGMChannel;

// 현재 사운드 재생 여부
FMOD_BOOL g_isSoundMute;
double g_volume;
double g_account;

// 채널 인덱스, bgm 변경 인덱스
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

// 사운드 페이드 인아웃 구조체
typedef enum SoundFadeINOUT
{
    Sound_State_Idle,
    Sound_State_FADEIN,
    Sound_State_FADEOUT

}Sound_State;

// 페이드 인아웃 구조체 선언
Sound_State SoundState;

// 사운드 시스템 선언 함수
void SoundInit();

// 사운드 시스템 생성 함수
void SoundSystemCreate();

// 사운드 업데이트
void Sound_Update();

// 페이드 인/아웃 함수
void FadeIn();
void FadeOut();

// 사운드 파일 선언 및 삽입 함수
void SoundFileInput();

// 사운드 플레이 함수
void SoundPlay(int PlayIndex);

// BGM 사운드 변경 함수
void SoundBGMchange(SoundType sound);

// 볼륨 변경 함수
void SetVolume(float volume);

// BGM을 바꾸는데 쓰일 함수 포인터
void (*onChangeBGM)();

// 컷씬에서 스테이지를 로드하는데 BGM 변경을 해야하면 호출하는 함수
void ChangeBGM_CutsceneToStage();

// 스테이지에서 컷씬을 로드하는데 BGM 변경을 해야하면 호출하는 함수
void ChangeBGM_StageToCutscene();

// 컷씬에서 컷씬으로 넘어가는데 BGM 변경을 해야하면 호출하는 함수
void ChangeBGM_CutsceneToCutscene();

// 중간 or 게임이 끝나서 메인 타이틀 화면으로 넘어갈 때 BGM 바꾸는 함수
void ChangeBGM_GoToMainTitle();

// 엔딩 분기점으로 돌아갈 때 호출 되는 함수
void ChangBGM_GotoEndPoint();

// 사운드 뮤트 함수
// void SoundPlayer_StopAll();