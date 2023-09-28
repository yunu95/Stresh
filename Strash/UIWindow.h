#pragma once
#include "UIImage.h"
#include "UIButton.h"

BOOL g_isMute;
BOOL g_isNowCutscene;

LPCWSTR muteButtonName;
LPCWSTR soundOnButtonName;

// UI 화면 하나를 뜻합니다. 메인 메뉴 화면, 게임 일시정지 메뉴 등이 이에 해당됩니다.
// stage가 그리는 그래픽보다 더 위에 출력됩니다.
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

// 아래 두 함수는 게임 루프에서 호출된다.
void UIWindow_Update();

void UIWindow_Render();


//=============================================================
// 
// 해당 UI 윈도우 인스턴스에 대한 동작을 아래에서 정의한다.
//instance->
void UIWindow_UpdateInstance(UIWindow* instance);

// 해당 UI 윈도우 인스턴스에 대한 동작을 아래에서 정의한다.
//instance->
void UIWindow_RenderInstance(UIWindow* instance);

// 메인 타이틀화면 초기화
void UIMainTitleInit();

// 인게임 UI 초기화
void IngameUI_Init();

// 컷씬 UI 초기화
void CutsceneUI_Init();

void SetIngameWindowAsCurrentWindow();

// 
UIWindow emptyWindow();