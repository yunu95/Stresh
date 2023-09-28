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

// �� ������Ͽ����� ���� ������ �÷��� �߰� �߰��� ��ȭó�� �����ϴ� UI�� ��� ó�������� �ٷ�ϴ�.
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

    // �����긦 ���� ������
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

// �ƽ��� ������Ʈ �Լ�
void Cutscene_Init();

void Cutscene_Update();
void Cutscene_Render();
// ���ϸ��� �Ű������� �� �Լ��� ȣ���ϸ�, �ش� �ƽſ� ���� ������ �ҷ��ɴϴ�.
void Cutscene_LoadCutscene();

void CallBack_CutsceneLoad();

//=========================================================================
// �ش� �ƽ��� �ν��Ͻ��� ���� ������ �Ʒ����� �����Ѵ�.
void Cutscene_UpdateInstance(Cutscene* instance);
// �ش� �ƽ��� �ν��Ͻ��� ���� ������ �Ʒ����� �����Ѵ�.
void Cutscene_RenderInstance(Cutscene* instance);
//=========================================================================

// �ƾ� �ʱ�ȭ �Լ���
//====================================================================

// ��Ʈ�� �ƽ� �ʱ�ȭ �Լ�
void Cutscene_Intro_Init(Vector2i _textPos);

// Ʃ�丮�� �ƾ� �ʱ�ȭ �Լ�
void Cutscene_Tutorial_Init(Vector2i _textPos);

// ��������1 �ƾ� �ʱ�ȭ �Լ� (��������)
void Cutscene_Stage0_Init(Vector2i _textPos);

// ��������2 �ƾ� �ʱ�ȭ �Լ� (��������)
void Cutscene_Stage1_Init(Vector2i _textPos);

// ��������3 �ƾ� �ʱ�ȭ �Լ� (��������)
void Cutscene_Stage2_Init(Vector2i _textPos);

// ���ǿ��� �ƾ� �ʱ�ȭ �Լ�
void Cutscene_HappyEnd_Init(Vector2i _textPos);

// ���ۿ��� �ƾ� �ʱ�ȭ �Լ�
void Cutscene_BadEnd_Init(Vector2i _textPos);

// ���� ũ���� �ƾ� �ʱ�ȭ �Լ�
void Cutscene_EndingCresdit_Init(Vector2i _textPos);

//====================================================================
// �ƾ��� �ִ� �ؽ�Ʈ�� ���� ������Ʈ �� �����Լ�
void Cutscene_Update_Text(Cutscene* instance);
void Cutscene_Render_Text(Cutscene* instance);

// �ƽ��� �������ϴ� �Լ�
void Cutscene_Desolve_Render(Cutscene* instance);
void Cutscene_Desolve_Update(Cutscene* instance);

// �ƾ��� ������ ���������� �θ��� �Լ� (�� �Լ��� BlackMask �Լ� �����Ϳ� �־����)
void Cutscene_CutsceneToStage();
void CallBack_CutsceneToStage();

// ������ ������ ������ ���̵��� �ƿ� �Ǹ鼭 ����ȭ������ ���ư���
void Cutscene_CutsceneToMainTitle();
void CallBack_CutsceneToMainTitle();

//// ���� �ƾ��� ������ ��� �θ��� �Լ�
//void Cutscene_MonologueLoad();
//void CallBack_Cutscene_MonologueLoad();

// �ƾ� �����Ű�� �Լ�
void Cutscene_NextCutscene();

// �� ����ü�� �ʱ�ȭ�Ͽ� ��ȯ�Ѵ�.
Cutscene emptyCutscene();



void Cutscene_CutsceneToCutscene();


// ���� �б������� ���� �Լ�
void Cutscene_GoToEndPoint();
void CallBack_GoToEndPoint();