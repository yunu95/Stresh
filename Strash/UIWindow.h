#pragma once
#include "UIImage.h"
#include "UIButton.h"

BOOL g_isMute;
BOOL g_isNowCutscene;

LPCWSTR muteButtonName;
LPCWSTR soundOnButtonName;

// UI ȭ�� �ϳ��� ���մϴ�. ���� �޴� ȭ��, ���� �Ͻ����� �޴� ���� �̿� �ش�˴ϴ�.
// stage�� �׸��� �׷��Ⱥ��� �� ���� ��µ˴ϴ�.
typedef struct 
{
    UIImage uiImages[10];
    UIButton uiButtons[10];
    int uiImageIndex;
    int buttonImageIndex;
}
UIWindow;

UIWindow UIWindow_mainWindow;
UIWindow UIWindow_Ingame;
UIWindow UIWindow_Cutscene;
UIWindow* UIWindow_currentWindow;

void UIWindow_Init();

// �Ʒ� �� �Լ��� ���� �������� ȣ��ȴ�.
void UIWindow_Update();

void UIWindow_Render();


//=============================================================
// 
// �ش� UI ������ �ν��Ͻ��� ���� ������ �Ʒ����� �����Ѵ�.
//instance->
void UIWindow_UpdateInstance(UIWindow* instance);

// �ش� UI ������ �ν��Ͻ��� ���� ������ �Ʒ����� �����Ѵ�.
//instance->
void UIWindow_RenderInstance(UIWindow* instance);

// ���� Ÿ��Ʋȭ�� �ʱ�ȭ
void UIMainTitleInit();

// �ΰ��� UI �ʱ�ȭ
void IngameUI_Init();

// �ƾ� UI �ʱ�ȭ
void CutsceneUI_Init();

void SetIngameWindowAsCurrentWindow();

// 
UIWindow emptyWindow();