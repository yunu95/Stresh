#include "UIWindow.h"
#include "GDIEngine.h"
#include "UserInput.h"
#include "ReloadTransition.h"
#include <windows.h>

BOOL g_isMute = FALSE;
BOOL g_isNowCutscene = FALSE;

LPCWSTR muteButtonName = L"mute.bmp";
LPCWSTR soundOnButtonName = L"Sound on.bmp";

UIWindow UIWindow_mainWindow;
UIWindow UIWindow_Ingame;
UIWindow* UIWindow_currentWindow = NULL;

void UIWindow_Init()
{
	UIWindow_currentWindow = &UIWindow_mainWindow;

	muteButtonName = L"mute.bmp";
	soundOnButtonName = L"Sound on.bmp";

	buttonHightlightingRect.x = 0;
	buttonHightlightingRect.y = 0;

	// 메인 타이틀 화면 초기화
	UIMainTitleInit();

	// 인게임 UI 초기화
	IngameUI_Init();

	// 컷씬 UI 초기화
	CutsceneUI_Init();
}

void UIWindow_Update()
{
	if (UIWindow_currentWindow)
		UIWindow_UpdateInstance(UIWindow_currentWindow);
}

void UIWindow_Render()
{
	if (UIWindow_currentWindow)
		UIWindow_RenderInstance(UIWindow_currentWindow);
}

void UIWindow_UpdateInstance(UIWindow* instance)
{
	RECT rt = {0,};
	if (UserInput_GetKeyState(UserInput_KEY_LBUTTON) == UserInput_KEY_STATE_TAP
		&& ReloadTransition_Instance.state == ReloadTransitionState_IDLE)
	{
		POINT pt = UserInput_mosuePosition_Point;

		for (int i = 0; i < instance->buttonImageIndex; i++)
		{
			rt.left = instance->uiButtons[i].position.x - (instance->uiButtons[i].m_area.x / 2);
			rt.top = instance->uiButtons[i].position.y - (instance->uiButtons[i].m_area.y / 2);
			rt.right = instance->uiButtons[i].position.x + (instance->uiButtons[i].m_area.x / 2);
			rt.bottom = instance->uiButtons[i].position.y + (instance->uiButtons[i].m_area.y / 2);
			if (PtInRect(&rt, UserInput_mosuePosition_Point))
			{
				instance->uiButtons[i].onClick(&instance->uiButtons[i]);
			}
		}
	}
	else if (UserInput_GetKeyState(UserInput_KEY_ESCAPE) == UserInput_KEY_STATE_TAP && ReloadTransition_Instance.state == ReloadTransitionState_IDLE &&UIWindow_currentWindow != &UIWindow_mainWindow)
	{
		InGameExit();
	}

	OutputDebugStringW(L"UIWindow_UpdateInstance - 1\n");


	for (int i = 0; i < instance->buttonImageIndex; i++)
	{
		rt.left = instance->uiButtons[i].position.x - (instance->uiButtons[i].m_area.x / 2);
		rt.top = instance->uiButtons[i].position.y - (instance->uiButtons[i].m_area.y / 2);
		rt.right = instance->uiButtons[i].position.x + (instance->uiButtons[i].m_area.x / 2);
		rt.bottom = instance->uiButtons[i].position.y + (instance->uiButtons[i].m_area.y / 2);

		if (PtInRect(&rt, UserInput_mosuePosition_Point))
		{
			instance->uiButtons[i].onHighlight(&instance->uiButtons[i]);

			LPCWSTR _text[255];
			wsprintfW(_text, L"onHighlight - i : %d\n", i);
			OutputDebugStringW(_text);
		}
		else
		{
			instance->uiButtons[i].onDeHighlight(&instance->uiButtons[i]);

			LPCWSTR _text[255];
			wsprintfW(_text, L"onDeHighlight - i : %d\n", i);
			OutputDebugStringW(_text);
		}
	}
}

void UIWindow_RenderInstance(UIWindow* instance)
{
	for (int i = 0; i < instance->uiImageIndex; i++)
	{
		if (instance->uiImages[i].m_isActive)
		{
			GDI_DrawResizedBitmap(instance->uiImages[i].imageName, instance->uiImages[i].position, instance->uiImages[i].m_drawRect);
		}
	}
	for (int i = 0; i < instance->buttonImageIndex; i++)
	{
		if (instance->uiButtons[i].m_isActive)
		{
			GDI_DrawResizedBitmap(instance->uiButtons[i].imageName, instance->uiButtons[i].position, instance->uiButtons[i].m_drawRect);

			LPCWSTR _text[255];
			wsprintfW(_text, L"UIWindow_RenderInstance - i : %d, name : %s, rect : %d/%d\n", i, instance->uiButtons[i].imageName, instance->uiButtons[i].m_drawRect.x, instance->uiButtons[i].m_drawRect.y);
			OutputDebugStringW(_text);
		}
	}
}

void UIMainTitleInit()
{
	// 백그라운드 관련 && 타이틀 포함
	UIWindow_mainWindow = emptyWindow();
	Vector2i drawRect = GetVector2i_XY(0,0);
	UIWindow_mainWindow.uiImages[UIWindow_mainWindow.uiImageIndex].m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	UIWindow_mainWindow.uiImages[UIWindow_mainWindow.uiImageIndex].imageName = L"MainTitleBG.bmp";
	UIWindow_mainWindow.uiImages[UIWindow_mainWindow.uiImageIndex].m_isActive = TRUE;
	UIWindow_mainWindow.uiImages[UIWindow_mainWindow.uiImageIndex].m_layer = 0;
	UIWindow_mainWindow.uiImages[UIWindow_mainWindow.uiImageIndex].position = GDIEngine_center;
	UIWindow_mainWindow.uiImageIndex++;

	// 게임 시작 버튼
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_drawRect = drawRect;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].imageName = L"GameStart.bmp";
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_isActive = TRUE;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_layer = 1;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_area.x = 305;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_area.y = 114;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_areaOrigin = UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_area;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].onClick = StartButton;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].onHighlight = ButtonHighlighting;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].onDeHighlight = ButtonDeHighlighint;
	Vector2i Pos = {1708,709};
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].position = Pos;
	UIWindow_mainWindow.buttonImageIndex++;

	// 게임 방법 버튼
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_drawRect = drawRect;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].imageName = L"HowToPlay.bmp";
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_isActive = TRUE;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_area.x = 305;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_area.y = 114;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_areaOrigin = UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_area;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].onClick = HowToPlayButton;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].onHighlight = ButtonHighlighting;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].onDeHighlight = ButtonDeHighlighint;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_layer = 1;
	Pos.y = 848;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].position = Pos;
	UIWindow_mainWindow.buttonImageIndex++;

	// 게임 종료 버튼
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_drawRect = drawRect;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].imageName = L"GameExit.bmp";
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_isActive = TRUE;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_area.x = 305;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_area.y = 114;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_areaOrigin = UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_area;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].onClick = MainTitleExit;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].onHighlight = ButtonHighlighting;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].onDeHighlight = ButtonDeHighlighint;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].m_layer = 1;
	Pos.y = 987;
	UIWindow_mainWindow.uiButtons[UIWindow_mainWindow.buttonImageIndex].position = Pos;
	UIWindow_mainWindow.buttonImageIndex++; 
}

void IngameUI_Init()
{
	UIWindow_Ingame = emptyWindow();
	Vector2i drawRect = GetVector2i_XY(0, 0);
	// 사운드 버튼 (ON)
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_drawRect = drawRect;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].imageName = L"Sound on.bmp";
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_isActive = TRUE;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_layer = 1;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_area.x = 150;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_area.y = 150;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_areaOrigin = UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_area;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].onClick = SoundMuteButton;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].onHighlight = ButtonHighlighting;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].onDeHighlight = ButtonDeHighlighint;
	Vector2i Pos = { 120,118 };
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].position = Pos;
	UIWindow_Ingame.buttonImageIndex++;

	// 다시하기 버튼
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_drawRect = drawRect;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].imageName = L"reset key.bmp";
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_isActive = TRUE;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_layer = 1;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_area.x = 150;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_area.y = 150;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_areaOrigin = UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_area;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].onClick = RetryButton;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].onHighlight = ButtonHighlighting;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].onDeHighlight = ButtonDeHighlighint;
	Pos.x = 1600;
	Pos.y = 118;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].position = Pos;
	UIWindow_Ingame.buttonImageIndex++;
	
	// 나가기 버튼
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_drawRect = drawRect;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].imageName = L"exit ingame.bmp";
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_isActive = TRUE;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_layer = 1;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_area.x = 150;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_area.y = 150;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_areaOrigin = UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].m_area;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].onClick = InGameExit;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].onHighlight = ButtonHighlighting;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].onDeHighlight = ButtonDeHighlighint;
	Pos.x = 1800;
	Pos.y = 118;
	UIWindow_Ingame.uiButtons[UIWindow_Ingame.buttonImageIndex].position = Pos;
	UIWindow_Ingame.buttonImageIndex++;
	
	// 다시하기 이미지
	//UIWindow_Ingame.uiImages[UIWindow_Ingame.uiImageIndex].m_drawRect = drawRect;
	//UIWindow_Ingame.uiImages[UIWindow_Ingame.uiImageIndex].imageName = L"reset key.bmp";
	//UIWindow_Ingame.uiImages[UIWindow_Ingame.uiImageIndex].m_isActive = TRUE;
	//UIWindow_Ingame.uiImages[UIWindow_Ingame.uiImageIndex].m_layer = 0;
	//Pos.x = 1600;
	//Pos.y = 118;
	//UIWindow_Ingame.uiImages[UIWindow_Ingame.uiImageIndex].position = Pos;
	//UIWindow_Ingame.uiImageIndex++;

	//// 나가기 이미지
	//UIWindow_Ingame.uiImages[UIWindow_Ingame.uiImageIndex].m_drawRect = drawRect;
	//UIWindow_Ingame.uiImages[UIWindow_Ingame.uiImageIndex].imageName = L"exit ingame.bmp";
	//UIWindow_Ingame.uiImages[UIWindow_Ingame.uiImageIndex].m_isActive = TRUE;
	//UIWindow_Ingame.uiImages[UIWindow_Ingame.uiImageIndex].m_layer = 0;
	//Pos.x = 1800;
	//Pos.y = 118;
	//UIWindow_Ingame.uiImages[UIWindow_Ingame.uiImageIndex].position = Pos;
	//UIWindow_Ingame.uiImageIndex++;
}

void CutsceneUI_Init()
{
	UIWindow_Cutscene = emptyWindow();
	Vector2i drawRect = GetVector2i_XY(0, 0);
	Vector2i Pos = { 1800,960};

	// 스킵 버튼
	UIWindow_Cutscene.uiButtons[UIWindow_Cutscene.buttonImageIndex].m_drawRect = drawRect;
	UIWindow_Cutscene.uiButtons[UIWindow_Cutscene.buttonImageIndex].imageName = L"skip button.bmp";
	UIWindow_Cutscene.uiButtons[UIWindow_Cutscene.buttonImageIndex].m_isActive = TRUE;
	UIWindow_Cutscene.uiButtons[UIWindow_Cutscene.buttonImageIndex].m_layer = 1;
	UIWindow_Cutscene.uiButtons[UIWindow_Cutscene.buttonImageIndex].m_area.x = 150;
	UIWindow_Cutscene.uiButtons[UIWindow_Cutscene.buttonImageIndex].m_area.y = 150;
	UIWindow_Cutscene.uiButtons[UIWindow_Cutscene.buttonImageIndex].m_areaOrigin = UIWindow_Cutscene.uiButtons[UIWindow_Cutscene.buttonImageIndex].m_area;
	UIWindow_Cutscene.uiButtons[UIWindow_Cutscene.buttonImageIndex].onClick = SkipButton;
	UIWindow_Cutscene.uiButtons[UIWindow_Cutscene.buttonImageIndex].onHighlight = ButtonHighlighting;
	UIWindow_Cutscene.uiButtons[UIWindow_Cutscene.buttonImageIndex].onDeHighlight = ButtonDeHighlighint;
	UIWindow_Cutscene.uiButtons[UIWindow_Cutscene.buttonImageIndex].position = Pos;
	UIWindow_Cutscene.buttonImageIndex++;

	// 스킵 이미지
	//UIWindow_Cutscene.uiImages[UIWindow_Cutscene.uiImageIndex].m_drawRect = drawRect;
	//UIWindow_Cutscene.uiImages[UIWindow_Cutscene.uiImageIndex].imageName = L"skip button.bmp";
	//UIWindow_Cutscene.uiImages[UIWindow_Cutscene.uiImageIndex].m_isActive = TRUE;
	//UIWindow_Cutscene.uiImages[UIWindow_Cutscene.uiImageIndex].m_layer = 0;
	//UIWindow_Cutscene.uiImages[UIWindow_Cutscene.uiImageIndex].position = Pos;
	//UIWindow_Cutscene.uiImageIndex++;
}
void SetIngameWindowAsCurrentWindow()
{
    UIWindow_currentWindow = &UIWindow_Ingame;
}

UIWindow emptyWindow()
{
	UIWindow ret;
	memset(&ret, 0, sizeof(UIWindow));
	return ret;
}