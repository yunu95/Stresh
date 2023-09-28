#include "UIButton.h"
#include "UIWindow.h"
#include "BlackMask.h"
#include "Cutscene.h"
#include "Sound.h"
#include "Stage.h"
#include "Vector2i.h"
#include "GDIEngine.h"
#include "ReloadTransition.h"
#include "Sound.h"

Vector2i buttonHightlightingRect = {0,0};

BOOL g_isButtonHighlighting = FALSE;

void StartButton(UIButton* _button)
{
    CutsceneManager_CurCutscene = &CutsceneArr[0];
    g_isNowCutscene = TRUE;
    g_blackMask.m_state = BlackMask_State_FADEOUT;
    Cutscene_LoadCutscene();
}

void HowToPlayButton(UIButton* _button)
{
    //UIWindow_currentWindow = &UIWindow_Ingame;
    stageInstance.transitionNextStage = L"how to play.txt";
    lstrcpyW(ReloadTransition_Instance.image, L"UI\\\\Reload\\\\endingCredit.bmp");
    ReloadTransition_Instance.onOverlap2 = SetIngameWindowAsCurrentWindow;
    Stage_ReloadStage();
}

void GameExitButton(UIButton* _button)
{

}

void SoundMuteButton(UIButton* _button)
{
    //g_isMute
    if (g_isMute)
    {
        // 사운드 재생 코드 넣기
        //SetVolume(1);
        _button->imageName = soundOnButtonName;
        g_isMute = FALSE;
    }
    else
    {
        // 사운드 끄기 코드 넣기
        //SetVolume(0);
        _button->imageName = muteButtonName;
        g_isMute = TRUE;
    }
    g_isSoundMute = g_isMute;
}

void RetryButton()
{
    Stage_ReloadStage();
}

void MainTitleExit()
{
    DestroyWindow(g_hWnd);
}

void InGameExit()
{
    GoToMainTitle();
}

void SkipButton()
{
    if (!g_isDesolve && !g_blackMask.m_isFade && (stageInstance.state != Stage_State_PLAYING) && (UIWindow_currentWindow != &UIWindow_mainWindow))
    {
        Cutscene_NextCutscene();
    }
}

void ButtonHighlighting(UIButton* _button)
{
	LPCWSTR _text[255];
	wsprintfW(_text, L"ButtonHighlighting\n");
	OutputDebugStringW(_text);

    g_isButtonHighlighting = TRUE;
    buttonHightlightingRect.x = _button->m_areaOrigin.x + 45;
    buttonHightlightingRect.y = _button->m_areaOrigin.y + 45;
    _button->m_drawRect = buttonHightlightingRect;
    _button->m_area = buttonHightlightingRect;

	LPCWSTR _text2[255];
	wsprintfW(_text2, L"%d / %d\n", buttonHightlightingRect.x, buttonHightlightingRect.y);
	OutputDebugStringW(_text2);

	LPCWSTR _text3[255];
	wsprintfW(_text3, L"%d / %d\n", _button->m_drawRect.x, _button->m_drawRect.y);
	OutputDebugStringW(_text3);

	//wchar_t buffer[255] = { NULL };
	//swprintf_s(buffer, 255, L"g_isButtonHighlighting : %d", g_isButtonHighlighting);
	//SetWindowText(g_hWnd, buffer);
}

void ButtonDeHighlighint(UIButton* _button)
{
    g_isButtonHighlighting = FALSE;
    _button->m_drawRect = Vector2i_Zero;
    _button->m_area = _button->m_areaOrigin;

	//wchar_t buffer[255] = { NULL };
	//swprintf_s(buffer, 255, L"g_isButtonHighlighting : %d", g_isButtonHighlighting);
	//SetWindowText(g_hWnd, buffer);
}

void GoToMainTitle()
{
    g_blackMask.m_state = BlackMask_State_FADEOUT;
    g_blackMask.onFadeOutFinish = CallBack_GoToMainTitle;
    onChangeBGM = ChangeBGM_GoToMainTitle;
    SoundState = Sound_State_FADEOUT;
}

void CallBack_GoToMainTitle()
{
    stageInstance.state = Stage_State_NONE;
    UIWindow_currentWindow = &UIWindow_mainWindow;
    g_isSelectEnding = FALSE;
    g_isNowCutscene = FALSE;
    g_isCutsceneActive = FALSE;
    cutNum = 0;
    tempTextIndex = 0;

    for (int i = 0; i < CutsceneIndex_End; ++i)
    {
        if (CutsceneArr[i].m_CutsceneDC == NULL) continue;

        DeleteDC(CutsceneArr[i].m_CutsceneDC);
        CutsceneArr[i].m_CutsceneDC = NULL;
    }
    Cutscene_Init();

    g_isSoundMute = FALSE;
    g_isMute = FALSE;
    SetVolume(1);
    UIWindow_Ingame.uiButtons[0].imageName = soundOnButtonName;
}