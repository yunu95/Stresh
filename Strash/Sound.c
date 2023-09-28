#include "Sound.h"
#include "Time.h"
#include "UserInput.h"
#include "Cutscene.h"

#define SOUND_FADE_DURATION 1

// 시스템 포인터 선언
FMOD_SYSTEM* g_System;
void* extradriverdata = 0;

// 사용할 사운드의 전체 개수
FMOD_SOUND* g_Sound[15];
FMOD_SOUND* g_BGMSound[10];

// 채널 배열 선언
FMOD_CHANNEL* g_Channel[15];
FMOD_CHANNEL* g_BGMChannel;

// 현재 사운드 재생 여부
FMOD_BOOL g_isSoundMute = FALSE;
double g_volume = 1;
double g_account = 0;

// 채널 인덱스, bgm 변경 인덱스
int Sound_ChannelIndex = 0;
int g_BGMindex = 0;


SoundType tempSoundType = SoundType_BGM_MAIN;

// 사운드 시스템 선언 함수
void SoundInit()
{
    SoundSystemCreate();
    SoundFileInput();
    SoundBGMchange(SoundType_BGM_MAIN);
}

// 사운드 시스템 생성
void SoundSystemCreate()
{
    FMOD_System_Create(&g_System);
    FMOD_System_Init(g_System, 32, FMOD_INIT_NORMAL, extradriverdata);
}

// 사운드 파일 삽입
void SoundFileInput()
{
    // BGM
    FMOD_System_CreateSound(g_System, SOUND_BGM_MAIN, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_MAIN]); // 타이틀
    FMOD_System_CreateSound(g_System, SOUND_BGM_INTRO, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_INTRO]); // 타이틀
    FMOD_System_CreateSound(g_System, SOUND_BGM_TUTORIAL, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_TUTORIAL]); // 타이틀
    //FMOD_System_CreateSound(g_System, SOUND_BGM_TUTORIAL_CUTSCENE, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_TUTORIAL_CUTSCENE]); // 타이틀
    FMOD_System_CreateSound(g_System, SOUND_BGM_NOTE_CUTSCENE, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_NOTE_CUTSCENE]); // 타이틀
    FMOD_System_CreateSound(g_System, SOUND_BGM_STAGE4, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_STAGE4]); // 인트로 컷씬
    FMOD_System_CreateSound(g_System, SOUND_BGM_INGAME, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_INGAME]); // 튜토리얼, 인게임 컷씬
    FMOD_System_CreateSound(g_System, SOUND_BGM_HAPPYEND, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_HAPPYEND]); // 해피엔딩
    FMOD_System_CreateSound(g_System, SOUND_BGM_BADEND, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_BADEND]); // 배드엔딩

    // SFX
    FMOD_System_CreateSound(g_System, SOUND_EFFECT_PUSH, FMOD_DEFAULT, 0, &g_Sound[SoundType_Effect_PUSH]); // 오브젝트 PUSH
    FMOD_System_CreateSound(g_System, SOUND_EFFECT_MERGE, FMOD_DEFAULT, 0, &g_Sound[SoundType_Effect_MERGE]); // 오브젝트 MERGE
    FMOD_System_CreateSound(g_System, SOUND_EFFECT_FOOTSTEP, FMOD_DEFAULT, 0, &g_Sound[SoundType_Effect_FOOTSTEP]); // 캐릭터 MOVE
    FMOD_System_CreateSound(g_System, SOUND_EFFECT_CLICK, FMOD_DEFAULT, 0, &g_Sound[SoundType_Effect_CLICK]); // UI CLICK
    FMOD_System_CreateSound(g_System, SOUND_EFFECT_RETRY, FMOD_DEFAULT, 0, &g_Sound[SoundType_Effect_RETRY]); // 게임 RETRY
    FMOD_System_CreateSound(g_System, SOUND_EFFECT_PUZZLE, FMOD_DEFAULT, 0, &g_Sound[SoundType_Effect_PUZZLE]); // 퍼즐 성공
}

// 사운드 업데이트
void Sound_Update()
{


    if (SoundState == Sound_State_FADEIN)
    {
        // 소리가 점점 커짐
        if (g_volume < 1)
        {
            g_account += Time_Instance.deltaTime;
            g_volume = g_account / SOUND_FADE_DURATION;
        }
        else
        {
            onChangeBGM = NULL;
            g_account = 0.f;
            SoundState = Sound_State_Idle;
        }
    }
    else if (SoundState == Sound_State_FADEOUT)
    {
        //if (!g_isSoundMute)
        //{
        //    SetVolume(g_volume);
        //    return;
        //}

        // 소리가 점점 작아짐
        if (g_volume >= 0)
        {
            g_account += Time_Instance.deltaTime;
            g_volume = (1 - (g_account / SOUND_FADE_DURATION));
        }
        else
        {
            // BGM 바꾸는 함수 넣을 곳
            if (onChangeBGM != NULL)
            {
                onChangeBGM();
            }

            g_account = 0.f;
            SoundState = Sound_State_FADEIN;
        }
    }
    BOOL isplayingMusic = FMOD_Channel_IsPlaying(g_BGMChannel, &isplayingMusic);
    SetVolume(g_isSoundMute ? 0 : g_volume);
}

// 페이드 인 함수
void FadeIn()
{

}

// 페이드 아웃 함수
void FadeOut()
{

}

// 사운드 플레이
void SoundPlay(int index) // int index 
{
    for (int i = 0; i < 15; ++i)
    {
        // 채널 인덱스를 환형으로 순환하며 비어있는 채널 탐색
        // 비어있는 채널을 발견하면 해당 채널에 사운드를 재생
        Sound_ChannelIndex = ++Sound_ChannelIndex % 15;

        FMOD_BOOL isPlaying;
        FMOD_Channel_IsPlaying(g_Channel[Sound_ChannelIndex], &isPlaying);

        // 하단 조건문의 조건은 채널 인덱스의 채널이 휴면상태일 때 참이다.
        if (!isPlaying || g_Channel[Sound_ChannelIndex] == 0)
        {
            FMOD_System_PlaySound(g_System, g_Sound[index], 0, FALSE, &g_Channel[Sound_ChannelIndex]);
            FMOD_Channel_SetVolume(g_Channel[Sound_ChannelIndex], g_isSoundMute ? 0 : g_volume);
            break;
        }
    }
}

// 음악 재생 함수
void SoundBGMchange(SoundType sound)
{
    FMOD_Channel_Stop(g_BGMChannel);
    FMOD_System_PlaySound(g_System, g_BGMSound[sound], 0, FALSE, &g_BGMChannel);
}

// 뮤트 함수 (볼륨 최소화 / 최대화)
void SetVolume(float volume)
{
    //if (g_isSoundMute) return;
    FMOD_Channel_SetVolume(g_BGMChannel, volume);
    //g_volume = volume;
}

void ChangeBGM_CutsceneToStage()
{
    switch (cutNum)
    {
        // 인트로에서 튜토리얼로 넘어갈 때
    case CutsceneIndex_Tutorial:
    {
        tempSoundType = SoundType_BGM_TUTORIAL;
        SoundBGMchange(tempSoundType);
        break;
    }
    // 스테이지3(독백)에서 스테이지4로 넘어갈 때
    case CutsceneIndex_HappyEnding_Monologue:
    {
        tempSoundType = SoundType_BGM_STAGE4;
        SoundBGMchange(tempSoundType);
        break;
    }
    }
}

void ChangeBGM_StageToCutscene()
{
    switch (cutNum)
    {
        // 메인타이틀화면에서 인트로컷씬으로 넘어갈 때
        //case CutsceneIndex_Intro0:
        //{
        //	tempSoundType = SoundType_BGM_INTRO;
        //	SoundBGMchange(tempSoundType);
        //	break;
        //}

        // 튜토리얼에서 튜토리얼 컷씬으로 넘어갈 때
    case CutsceneIndex_Tutorial:
    {
        tempSoundType = SoundType_BGM_INGAME;
        SoundBGMchange(tempSoundType);
        break;
    }
    // 스테이지4에서 엔딩 컷씬으로 넘어갈 때
    case CutsceneIndex_HappyEnding_Monologue:
    {
        tempSoundType = SoundType_BGM_HAPPYEND;
        SoundBGMchange(tempSoundType);
        break;
    }
    case CutsceneIndex_BadEnding_Monologue:
    {
        tempSoundType = SoundType_BGM_BADEND;
        SoundBGMchange(tempSoundType);
        break;
    }
    }
}

void ChangeBGM_CutsceneToCutscene()
{
    switch (cutNum)
    {
        // 스테이지3 컷씬에서 산모수첩 컷씬으로 넘어갈 때
    case CutsceneIndex_HappayEnding0:
    {
        tempSoundType = SoundType_BGM_NOTE_CUTSCENE;
        SoundBGMchange(tempSoundType);
    }
    break;
    }
}

void ChangeBGM_GoToMainTitle()
{
    SoundBGMchange(SoundType_BGM_MAIN);
}

void ChangBGM_GotoEndPoint()
{
    SoundBGMchange(SoundType_BGM_STAGE4);
}