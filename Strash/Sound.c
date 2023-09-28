#include "Sound.h"
#include "Time.h"
#include "UserInput.h"
#include "Cutscene.h"

#define SOUND_FADE_DURATION 1

// �ý��� ������ ����
FMOD_SYSTEM* g_System;
void* extradriverdata = 0;

// ����� ������ ��ü ����
FMOD_SOUND* g_Sound[15];
FMOD_SOUND* g_BGMSound[10];

// ä�� �迭 ����
FMOD_CHANNEL* g_Channel[15];
FMOD_CHANNEL* g_BGMChannel;

// ���� ���� ��� ����
FMOD_BOOL g_isSoundMute = FALSE;
double g_volume = 1;
double g_account = 0;

// ä�� �ε���, bgm ���� �ε���
int Sound_ChannelIndex = 0;
int g_BGMindex = 0;


SoundType tempSoundType = SoundType_BGM_MAIN;

// ���� �ý��� ���� �Լ�
void SoundInit()
{
    SoundSystemCreate();
    SoundFileInput();
    SoundBGMchange(SoundType_BGM_MAIN);
}

// ���� �ý��� ����
void SoundSystemCreate()
{
    FMOD_System_Create(&g_System);
    FMOD_System_Init(g_System, 32, FMOD_INIT_NORMAL, extradriverdata);
}

// ���� ���� ����
void SoundFileInput()
{
    // BGM
    FMOD_System_CreateSound(g_System, SOUND_BGM_MAIN, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_MAIN]); // Ÿ��Ʋ
    FMOD_System_CreateSound(g_System, SOUND_BGM_INTRO, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_INTRO]); // Ÿ��Ʋ
    FMOD_System_CreateSound(g_System, SOUND_BGM_TUTORIAL, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_TUTORIAL]); // Ÿ��Ʋ
    //FMOD_System_CreateSound(g_System, SOUND_BGM_TUTORIAL_CUTSCENE, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_TUTORIAL_CUTSCENE]); // Ÿ��Ʋ
    FMOD_System_CreateSound(g_System, SOUND_BGM_NOTE_CUTSCENE, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_NOTE_CUTSCENE]); // Ÿ��Ʋ
    FMOD_System_CreateSound(g_System, SOUND_BGM_STAGE4, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_STAGE4]); // ��Ʈ�� �ƾ�
    FMOD_System_CreateSound(g_System, SOUND_BGM_INGAME, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_INGAME]); // Ʃ�丮��, �ΰ��� �ƾ�
    FMOD_System_CreateSound(g_System, SOUND_BGM_HAPPYEND, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_HAPPYEND]); // ���ǿ���
    FMOD_System_CreateSound(g_System, SOUND_BGM_BADEND, FMOD_LOOP_NORMAL, 0, &g_BGMSound[SoundType_BGM_BADEND]); // ��忣��

    // SFX
    FMOD_System_CreateSound(g_System, SOUND_EFFECT_PUSH, FMOD_DEFAULT, 0, &g_Sound[SoundType_Effect_PUSH]); // ������Ʈ PUSH
    FMOD_System_CreateSound(g_System, SOUND_EFFECT_MERGE, FMOD_DEFAULT, 0, &g_Sound[SoundType_Effect_MERGE]); // ������Ʈ MERGE
    FMOD_System_CreateSound(g_System, SOUND_EFFECT_FOOTSTEP, FMOD_DEFAULT, 0, &g_Sound[SoundType_Effect_FOOTSTEP]); // ĳ���� MOVE
    FMOD_System_CreateSound(g_System, SOUND_EFFECT_CLICK, FMOD_DEFAULT, 0, &g_Sound[SoundType_Effect_CLICK]); // UI CLICK
    FMOD_System_CreateSound(g_System, SOUND_EFFECT_RETRY, FMOD_DEFAULT, 0, &g_Sound[SoundType_Effect_RETRY]); // ���� RETRY
    FMOD_System_CreateSound(g_System, SOUND_EFFECT_PUZZLE, FMOD_DEFAULT, 0, &g_Sound[SoundType_Effect_PUZZLE]); // ���� ����
}

// ���� ������Ʈ
void Sound_Update()
{


    if (SoundState == Sound_State_FADEIN)
    {
        // �Ҹ��� ���� Ŀ��
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

        // �Ҹ��� ���� �۾���
        if (g_volume >= 0)
        {
            g_account += Time_Instance.deltaTime;
            g_volume = (1 - (g_account / SOUND_FADE_DURATION));
        }
        else
        {
            // BGM �ٲٴ� �Լ� ���� ��
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

// ���̵� �� �Լ�
void FadeIn()
{

}

// ���̵� �ƿ� �Լ�
void FadeOut()
{

}

// ���� �÷���
void SoundPlay(int index) // int index 
{
    for (int i = 0; i < 15; ++i)
    {
        // ä�� �ε����� ȯ������ ��ȯ�ϸ� ����ִ� ä�� Ž��
        // ����ִ� ä���� �߰��ϸ� �ش� ä�ο� ���带 ���
        Sound_ChannelIndex = ++Sound_ChannelIndex % 15;

        FMOD_BOOL isPlaying;
        FMOD_Channel_IsPlaying(g_Channel[Sound_ChannelIndex], &isPlaying);

        // �ϴ� ���ǹ��� ������ ä�� �ε����� ä���� �޸������ �� ���̴�.
        if (!isPlaying || g_Channel[Sound_ChannelIndex] == 0)
        {
            FMOD_System_PlaySound(g_System, g_Sound[index], 0, FALSE, &g_Channel[Sound_ChannelIndex]);
            FMOD_Channel_SetVolume(g_Channel[Sound_ChannelIndex], g_isSoundMute ? 0 : g_volume);
            break;
        }
    }
}

// ���� ��� �Լ�
void SoundBGMchange(SoundType sound)
{
    FMOD_Channel_Stop(g_BGMChannel);
    FMOD_System_PlaySound(g_System, g_BGMSound[sound], 0, FALSE, &g_BGMChannel);
}

// ��Ʈ �Լ� (���� �ּ�ȭ / �ִ�ȭ)
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
        // ��Ʈ�ο��� Ʃ�丮��� �Ѿ ��
    case CutsceneIndex_Tutorial:
    {
        tempSoundType = SoundType_BGM_TUTORIAL;
        SoundBGMchange(tempSoundType);
        break;
    }
    // ��������3(����)���� ��������4�� �Ѿ ��
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
        // ����Ÿ��Ʋȭ�鿡�� ��Ʈ���ƾ����� �Ѿ ��
        //case CutsceneIndex_Intro0:
        //{
        //	tempSoundType = SoundType_BGM_INTRO;
        //	SoundBGMchange(tempSoundType);
        //	break;
        //}

        // Ʃ�丮�󿡼� Ʃ�丮�� �ƾ����� �Ѿ ��
    case CutsceneIndex_Tutorial:
    {
        tempSoundType = SoundType_BGM_INGAME;
        SoundBGMchange(tempSoundType);
        break;
    }
    // ��������4���� ���� �ƾ����� �Ѿ ��
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
        // ��������3 �ƾ����� ����ø �ƾ����� �Ѿ ��
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