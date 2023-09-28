#pragma once
#include <Windows.h>
#include "UIImage.h"
#include "Text.h"
#include "texture.h"

#define DESOLV_DURATION 1
#define TEXT_DESOLV_DURATION 0.5f
#define TEXT_Y_POS 130
#define MONOLOGUE_TEXT_Y_POS 540
#define MONOLOGUE_TEXT_ORIGIN_Y_POS 560

// 본 헤더파일에서는 게임 진행중 플레이 중간 중간에 삽화처럼 등장하는 UI를 어떻게 처리할지를 다룹니다.
int cutNum;
typedef enum _CutsceneIndex
{
    CutsceneIndex_Intro0,
    CutsceneIndex_Intro1,
    CutsceneIndex_Intro2,
    CutsceneIndex_Intro3,
    CutsceneIndex_Intro4,
    CutsceneIndex_Intro5,

    CutsceneIndex_Tutorial,

    CutsceneIndex_Stage0,
    CutsceneIndex_Stage0_Monologue,

    CutsceneIndex_Stage1,
    CutsceneIndex_Stage1_Monologue,

    CutsceneIndex_Stage2,

    CutsceneIndex_HappayEnding0,
    CutsceneIndex_HappayEnding1,
    CutsceneIndex_HappayEnding2,
    CutsceneIndex_HappayEnding3,
    CutsceneIndex_HappayEnding4,
    CutsceneIndex_HappayEnding5,

    CutsceneIndex_Stage2_Monologue,

    CutsceneIndex_HappyEnding_Monologue,
    CutsceneIndex_HappyEnding,

    CutsceneIndex_BadEnding_Monologue,
    CutsceneIndex_BadEnding,

    CutsceneIndex_EndingCredit,

    CutsceneIndex_End,
}CutsceneIndex;

typedef struct
{
    UIImage m_cutsceneImage;
    Text m_text[10];
    int m_textIndex;
    Texture* m_tex;
    BOOL m_doesFadeOut;
    //BOOL m_doesLoadStage;
    LPCSTR loadingStage;
    BOOL m_isMonologue;

    BOOL m_isEndCutscene;

    BOOL m_isBGMChange;

    HDC m_CutsceneDC;
    HBITMAP m_CutsceneBitmap;

    // 디졸브를 위한 변수들
    BLENDFUNCTION blend;
    float m_acc;
    float m_percent;
    BOOL m_isFirst;
}Cutscene;

int cutNum;
int tempTextIndex;
Cutscene* CutsceneManager_CurCutscene;
Cutscene CutsceneArr[CutsceneIndex_End];
Cutscene* DesolveCutscene;

BOOL g_isDesolve;
BOOL g_isCutsceneActive;
BOOL g_isSelectEnding;
BOOL g_isHappyEnd;

// 컷신의 업데이트 함수
void Cutscene_Init();

void Cutscene_Update();
void Cutscene_Render();
// 파일명을 매개변수로 이 함수를 호출하면, 해당 컷신에 대한 정보를 불러옵니다.
void Cutscene_LoadCutscene();

void CallBack_CutsceneLoad();

//=========================================================================
// 해당 컷신의 인스턴스에 대한 동작을 아래에서 정의한다.
void Cutscene_UpdateInstance(Cutscene* instance);
// 해당 컷신의 인스턴스에 대한 동작을 아래에서 정의한다.
void Cutscene_RenderInstance(Cutscene* instance);
//=========================================================================

// 컷씬 초기화 함수들
//====================================================================

// 인트로 컷신 초기화 함수
void Cutscene_Intro_Init(Vector2i _textPos);

// 튜토리얼 컷씬 초기화 함수
void Cutscene_Tutorial_Init(Vector2i _textPos);

// 스테이지1 컷씬 초기화 함수 (독백포함)
void Cutscene_Stage0_Init(Vector2i _textPos);

// 스테이지2 컷씬 초기화 함수 (독백포함)
void Cutscene_Stage1_Init(Vector2i _textPos);

// 스테이지3 컷씬 초기화 함수 (독백포함)
void Cutscene_Stage2_Init(Vector2i _textPos);

// 해피엔딩 컷씬 초기화 함수
void Cutscene_HappyEnd_Init(Vector2i _textPos);

// 나쁜엔딩 컷씬 초기화 함수
void Cutscene_BadEnd_Init(Vector2i _textPos);

// 엔딩 크래딧 컷씬 초기화 함수
void Cutscene_EndingCresdit_Init(Vector2i _textPos);

//====================================================================
// 컷씬에 있는 텍스트를 위한 업데이트 및 렌더함수
void Cutscene_Update_Text(Cutscene* instance);
void Cutscene_Render_Text(Cutscene* instance);

// 컷신을 디졸브하는 함수
void Cutscene_Desolve_Render(Cutscene* instance);
void Cutscene_Desolve_Update(Cutscene* instance);

// 컷씬이 끝나고 스테이지를 부르는 함수 (이 함수는 BlackMask 함수 포인터에 넣어야함)
void Cutscene_CutsceneToStage();
void CallBack_CutsceneToStage();

// 마지막 엔딩이 끝나고 페이드인 아웃 되면서 메인화면으로 돌아가기
void Cutscene_CutsceneToMainTitle();
void CallBack_CutsceneToMainTitle();

//// 다음 컷씬이 독백인 경우 부르는 함수
//void Cutscene_MonologueLoad();
//void CallBack_Cutscene_MonologueLoad();

// 컷씬 진행시키는 함수
void Cutscene_NextCutscene();

// 빈 구조체를 초기화하여 반환한다.
Cutscene emptyCutscene();



void Cutscene_CutsceneToCutscene();


// 엔딩 분기점으로 가는 함수
void Cutscene_GoToEndPoint();
void CallBack_GoToEndPoint();