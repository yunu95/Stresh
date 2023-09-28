#include "Cutscene.h"
#include "GDIEngine.h"
#include "Vector2i.h"
#include "UserInput.h"
#include "Time.h"
#include "ResManager.h"
#include "BlackMask.h"
#include "UIWindow.h"
#include "MovementHandler.h"
#include "Sound.h"
#include "ReloadTransition.h"
#include <string.h>
#include <wchar.h>
#include <stdio.h>
#include "Stage.h"
#include "Sound.h"

int cutNum = 0;
int tempTextIndex = 0;
Cutscene* CutsceneManager_CurCutscene;
Cutscene CutsceneArr[CutsceneIndex_End];
Cutscene* DesolveCutscene;

BOOL g_isDesolve = FALSE;
BOOL g_isCutsceneActive = FALSE;

BOOL g_isTextShow = TRUE;

BOOL g_isStageToCutscene = FALSE;

BOOL g_isSelectEnding = FALSE;
BOOL g_isHappyEnd = FALSE;

Vector2i textPos;

void Cutscene_Init()
{
	textPos.x = GDIEngine_center.x;
	textPos.y = 150;
	Cutscene_Intro_Init(textPos);
	Cutscene_Tutorial_Init(textPos);
	Cutscene_Stage0_Init(textPos);
	Cutscene_Stage1_Init(textPos);
	Cutscene_Stage2_Init(textPos);
	Cutscene_HappyEnd_Init(textPos);
	Cutscene_BadEnd_Init(textPos);
	Cutscene_EndingCresdit_Init(textPos);
}

void Cutscene_Update()
{
	//wchar_t buffer[255] = { NULL };
	//swprintf_s(buffer, 255, L"CutsceneNumber : %d, %p", cutNum, CutsceneManager_CurCutscene);
	//SetWindowText(g_hWnd, buffer);

	// 엔딩 분기점으로 가는 치트키 
	if (UserInput_GetKeyState(UserInput_KEY_RBUTTON) == UserInput_KEY_STATE_TAP
		 && UserInput_GetKeyState(UserInput_KEY_TAB) == UserInput_KEY_STATE_HOLD && !g_isNowCutscene)
	{
		Cutscene_GoToEndPoint();
	}

	// 마지막 스테이지에서 선택하는 결과에 따른 분기문
	if (cutNum == CutsceneIndex_Stage2_Monologue + (CutsceneIndex_HappyEnding_Monologue - CutsceneIndex_Stage2_Monologue))
	{
		if (g_isSelectEnding)
		{
			// 해피엔딩이라면
			if (g_isHappyEnd)
			{
				cutNum = CutsceneIndex_HappyEnding_Monologue;
			}
			else
			{
				cutNum = CutsceneIndex_BadEnding_Monologue;
			}
		}
	}

	if (UserInput_GetKeyState(UserInput_KEY_SPACE) == UserInput_KEY_STATE_HOLD && !g_isDesolve && !g_blackMask.m_isFade && (stageInstance.state != Stage_State_PLAYING) && (UIWindow_currentWindow != &UIWindow_mainWindow)
		&& ReloadTransition_Instance.state == ReloadTransitionState_IDLE && g_isNowCutscene)
	{
		Cutscene_NextCutscene();
	}

	if (CutsceneManager_CurCutscene)
	{
		Cutscene_UpdateInstance(CutsceneManager_CurCutscene);
		if (CutsceneManager_CurCutscene->m_textIndex != -1)
		{
			Cutscene_Update_Text(CutsceneManager_CurCutscene);
		}
	}
	if (DesolveCutscene)
	{
		Cutscene_Desolve_Update(DesolveCutscene);
	}
}

void Cutscene_Render()
{
	if (!g_isCutsceneActive) return;

	if (CutsceneManager_CurCutscene)
	{
		Cutscene_RenderInstance(CutsceneManager_CurCutscene);
		if (CutsceneManager_CurCutscene->m_textIndex != -1)
		{
			Cutscene_Render_Text(CutsceneManager_CurCutscene);
		}
	}

	if (DesolveCutscene)
	{
		Cutscene_Desolve_Render(DesolveCutscene);
	}
}

// 컷씬 호출할 때 어디서나 이 함수호출하면 된다.
// 스테이지에서 컷씬으로 넘어갈 때 호출
void Cutscene_LoadCutscene()
{
	//g_isCutsceneActive = TRUE;
	g_blackMask.onFadeOutFinish = CallBack_CutsceneLoad;
	g_blackMask.m_state = BlackMask_State_FADEOUT;
	if (CutsceneManager_CurCutscene->m_isBGMChange && cutNum != 12 && cutNum!=0)
	{
		onChangeBGM = ChangeBGM_StageToCutscene;
		SoundState = Sound_State_FADEOUT;
	}
	//SoundBGMchange(SoundType_BGM_INTRO);
}

// 컷씬에서 컷씬으로 넘어갈 때 호출
void CallBack_CutsceneLoad()
{
	if (CutsceneManager_CurCutscene->m_isBGMChange && (cutNum != 0) && (cutNum != 1) && (onChangeBGM != ChangeBGM_StageToCutscene))
	{
		onChangeBGM = ChangeBGM_CutsceneToCutscene;
		SoundState = Sound_State_FADEOUT;
	}
	//g_blackMask.m_state = BlackMask_State_FADEOUT;

	g_isCutsceneActive = TRUE;

	stageInstance.state = Stage_State_NONE;
	g_isTextShow = TRUE;
	CutsceneManager_CurCutscene = &CutsceneArr[cutNum];
	CutsceneManager_CurCutscene->m_cutsceneImage.m_isActive = TRUE;
	g_isNowCutscene = TRUE;
	UIWindow_currentWindow = &UIWindow_Cutscene;
}

void Cutscene_UpdateInstance(Cutscene* instance)
{

}

void Cutscene_RenderInstance(Cutscene* instance)
{
	if (instance->m_cutsceneImage.m_isActive)
	{
		//GDI_DrawBitmap(instance->m_cutsceneImage.imageName, instance->m_cutsceneImage.position);
		//wchar_t buffer[255] = { NULL };
		//swprintf_s(buffer, 255, L"g_isDesolve : %d", g_isDesolve);
		//SetWindowText(g_hWnd, buffer);
		if (DesolveCutscene)
		{
			BitBlt(DesolveCutscene->m_CutsceneDC, 0, 0, 1920, 1080, instance->m_tex->m_hDC, 0, 0, SRCCOPY);

		}
		else
		{
			BitBlt(instance->m_CutsceneDC, 0, 0, 1920, 1080, instance->m_tex->m_hDC, 0, 0, SRCCOPY);


			BitBlt(g_hMemDC, 0, 0, 1920, 1080, instance->m_CutsceneDC, 0, 0, SRCCOPY);

		}
	}
}

void Cutscene_Update_Text(Cutscene* instance)
{
	if (tempTextIndex == 0)
	{
		instance->m_text[tempTextIndex].m_isActive = TRUE;
	}

	if (instance->m_text[tempTextIndex].m_isActive)
	{
		g_isDesolve = TRUE;

		if (instance->m_text[tempTextIndex].m_percent >= 1.f)
		{
			g_isDesolve = FALSE;
			return;
		}

		instance->m_text[tempTextIndex].m_acc += Time_Instance.deltaTime;
		instance->m_text[tempTextIndex].m_percent = instance->m_text[tempTextIndex].m_acc / TEXT_DESOLV_DURATION;
		if (!instance->m_isMonologue && instance->m_text[tempTextIndex].position.y > TEXT_Y_POS)
		{
			instance->m_text[tempTextIndex].position.y = (instance->m_text[tempTextIndex].position.y - (Time_Instance.deltaTime));
		}
		else if (instance->m_isMonologue && instance->m_text[tempTextIndex].position.y > MONOLOGUE_TEXT_Y_POS)
		{
			instance->m_text[tempTextIndex].position.y = (instance->m_text[tempTextIndex].position.y - (Time_Instance.deltaTime));
		}

		instance->m_text[tempTextIndex].blend.SourceConstantAlpha = instance->m_text[tempTextIndex].m_percent < 1 ? (int)(255 * (instance->m_text[tempTextIndex].m_percent)) : 255;
	}
}

void Cutscene_Render_Text(Cutscene* instance)
{
	if (!g_isTextShow) return;

	if (instance->m_text[tempTextIndex].m_isActive)
	{
		if (!instance->m_text[tempTextIndex].m_isFirst)
		{
			instance->m_text[tempTextIndex].m_isFirst = TRUE;
			instance->m_text[tempTextIndex].m_acc = 0.f;
			instance->m_text[tempTextIndex].m_percent = 0.f;
			instance->m_text[tempTextIndex].blend.BlendOp = 0;
			instance->m_text[tempTextIndex].blend.BlendFlags = 0;
			instance->m_text[tempTextIndex].blend.AlphaFormat = AC_SRC_OVER;
			instance->m_text[tempTextIndex].blend.SourceConstantAlpha = 0;
		}

		int width = instance->m_text[tempTextIndex].m_tex->m_bitInfo.bmWidth;
		int height = instance->m_text[tempTextIndex].m_tex->m_bitInfo.bmHeight;

		if (g_isDesolve)
		{
			if (DesolveCutscene == NULL)
			{
				TransparentBlt(instance->m_CutsceneDC
					, (int)instance->m_text[tempTextIndex].position.x - width / 2
					, (int)(instance->m_text[tempTextIndex].position.y - height / 2)
					, width, height
					, instance->m_text[tempTextIndex].m_tex->m_hDC
					, 0, 0, width, height
					, RGB(255, 0, 255));

				AlphaBlend(g_hMemDC,
					(int)instance->m_text[tempTextIndex].position.x - width / 2,
					(int)instance->m_text[tempTextIndex].position.y - height / 2,
					width, height,
					instance->m_CutsceneDC,
					(int)instance->m_text[tempTextIndex].position.x - width / 2,
					(int)instance->m_text[tempTextIndex].position.y - height / 2,
					width,
					height,
					instance->m_text[tempTextIndex].blend);
			}
			else
			{
				TransparentBlt(DesolveCutscene->m_CutsceneDC
					, (int)instance->m_text[tempTextIndex].position.x - width / 2
					, (int)(instance->m_text[tempTextIndex].position.y - height / 2)
					, width, height
					, instance->m_text[tempTextIndex].m_tex->m_hDC
					, 0, 0, width, height
					, RGB(255, 0, 255));

				AlphaBlend(g_hMemDC,
					(int)instance->m_text[tempTextIndex].position.x - width / 2,
					(int)instance->m_text[tempTextIndex].position.y - height / 2,
					width, height,
					DesolveCutscene->m_CutsceneDC,
					(int)instance->m_text[tempTextIndex].position.x - width / 2,
					(int)instance->m_text[tempTextIndex].position.y - height / 2,
					width,
					height,
					instance->m_text[tempTextIndex].blend);
			}
		}
		else
		{
			TransparentBlt(g_hMemDC
				, (int)instance->m_text[tempTextIndex].position.x - width / 2
				, (int)instance->m_text[tempTextIndex].position.y - height / 2
				, width, height
				, instance->m_text[tempTextIndex].m_tex->m_hDC
				, 0, 0, width, height
				, RGB(255, 0, 255));
		}
	}
}

void Cutscene_Desolve_Render(Cutscene* instance)
{
	if (DesolveCutscene)
	{
		//GDI_DrawPartialBitmapAlphaWithBf(instance->m_cutsceneImage.imageName, GDIEngine_center, GetVector2i_XY(1920, 1080), Vector2i_Zero, Vector2i_Zero, instance->blend);
		if (instance->m_isFirst)
		{
			instance->m_isFirst = FALSE;
			instance->m_acc = 0.f;
			instance->m_percent = 0.f;
			instance->blend.BlendOp = 0;
			instance->blend.BlendFlags = 0;
			instance->blend.AlphaFormat = AC_SRC_OVER;
			instance->blend.SourceConstantAlpha = 255;
		}
		//GdiAlphaBlend(intermediate_DC, 0, 0, 1920, 1080, instance->m_tex->m_hDC, 0, 0, 1920, 1080, instance->blend);
		AlphaBlend(instance->m_CutsceneDC, 0, 0, 1920, 1080, instance->m_tex->m_hDC, 0, 0, 1920, 1080, instance->blend);
		BitBlt(g_hMemDC, 0, 0, 1920, 1080, instance->m_CutsceneDC, 0, 0, SRCCOPY);
		//TransparentBlt(g_hMemDC
		//	, 0
		//	, 0
		//	, 1920, 1080
		//	, instance->m_CutsceneDC
		//	, 0, 0, 1920, 1080
		//	, RGB(255, 0, 255));
	}
}

void Cutscene_Desolve_Update(Cutscene* instance)
{
	if (DesolveCutscene)
	{
		if (instance->m_percent >= 1.f)
		{
			g_isDesolve = FALSE;
			instance->m_cutsceneImage.m_isActive = FALSE;
			DesolveCutscene = NULL;
			return;
		}

		g_isDesolve = TRUE;

		instance->m_acc += Time_Instance.deltaTime;
		instance->m_percent = instance->m_acc / DESOLV_DURATION;
		instance->blend.SourceConstantAlpha = instance->m_percent < 1 ? (int)(255 * (1 - instance->m_percent)) : 0;
	}
}

// 컷씬에서 스테이지로 넘어갈 때 호출
void Cutscene_CutsceneToStage()
{
	g_blackMask.onFadeOutFinish = CallBack_CutsceneToStage;
	if (CutsceneManager_CurCutscene->m_isBGMChange)
	{
		onChangeBGM = ChangeBGM_CutsceneToStage;
		SoundState = Sound_State_FADEOUT;
	}
	g_blackMask.m_state = BlackMask_State_FADEOUT;
	g_blackMask.m_acc = 0;
}

void CallBack_CutsceneToStage()
{
	stageInstance.state = Stage_State_PLAYING;
	UIWindow_currentWindow = &UIWindow_Ingame;
	g_isNowCutscene = FALSE;
	g_isCutsceneActive = FALSE;
	Stage_LoadStage(CutsceneManager_CurCutscene->loadingStage);
	//Stage_PlayStageBGM();
}



// 독백 없음 (DC 예외처리함)
void Cutscene_Intro_Init(Vector2i _textPos)
{
	HBITMAP hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	// 인트로 컷신0 초기화
	// (컷신 배경 초기화)
	CutsceneArr[CutsceneIndex_Intro0] = emptyCutscene();
	Vector2i drawRect = GetVector2i_XY(0, 0);
	CutsceneArr[CutsceneIndex_Intro0].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Intro0].m_cutsceneImage.imageName = L"IntroCutscene0.bmp";
	CutsceneArr[CutsceneIndex_Intro0].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_Intro0].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_Intro0].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Intro0].m_tex, L"texture\\IntroCutscene0.bmp");
	CutsceneArr[CutsceneIndex_Intro0].m_cutsceneImage.m_layer = 0;
	CutsceneArr[CutsceneIndex_Intro0].m_cutsceneImage.position = GDIEngine_center;
	CutsceneArr[CutsceneIndex_Intro0].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	CutsceneArr[CutsceneIndex_Intro0].m_isBGMChange = TRUE;
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_Intro0].m_CutsceneDC, hBit));

	// 인트로 컷신1 초기화
	// (컷신 배경 초기화)
	CutsceneArr[CutsceneIndex_Intro1] = emptyCutscene();
	CutsceneArr[CutsceneIndex_Intro1].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Intro1].m_cutsceneImage.imageName = L"IntroCutscene1.bmp";
	CutsceneArr[CutsceneIndex_Intro1].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_Intro1].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_Intro1].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Intro1].m_tex, L"texture\\IntroCutscene1.bmp");
	CutsceneArr[CutsceneIndex_Intro1].m_cutsceneImage.m_layer = 1;
	CutsceneArr[CutsceneIndex_Intro1].m_cutsceneImage.position = GDIEngine_center;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Intro1].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_Intro1].m_CutsceneDC, hBit));


	// 인트로 컷신2 초기화
	// (컷신 배경 초기화)
	CutsceneArr[CutsceneIndex_Intro2] = emptyCutscene();
	CutsceneArr[CutsceneIndex_Intro2].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Intro2].m_cutsceneImage.imageName = L"IntroCutscene2.bmp";
	CutsceneArr[CutsceneIndex_Intro2].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_Intro2].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_Intro2].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Intro2].m_tex, L"texture\\IntroCutscene2.bmp");
	CutsceneArr[CutsceneIndex_Intro2].m_cutsceneImage.m_layer = 2;
	CutsceneArr[CutsceneIndex_Intro2].m_cutsceneImage.position = GDIEngine_center;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Intro2].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_Intro2].m_CutsceneDC, hBit));

	// 인트로 컷신3 초기화
	// (컷신 배경 초기화)
	CutsceneArr[CutsceneIndex_Intro3] = emptyCutscene();
	CutsceneArr[CutsceneIndex_Intro3].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Intro3].m_cutsceneImage.imageName = L"IntroCutscene3.bmp";
	CutsceneArr[CutsceneIndex_Intro3].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_Intro3].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_Intro3].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Intro3].m_tex, L"texture\\IntroCutscene3.bmp");
	CutsceneArr[CutsceneIndex_Intro3].m_cutsceneImage.m_layer = 3;
	CutsceneArr[CutsceneIndex_Intro3].m_cutsceneImage.position = GDIEngine_center;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Intro3].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_Intro3].m_CutsceneDC, hBit));


	// 인트로 컷신4 초기화
	// (컷신 배경 초기화)
	CutsceneArr[CutsceneIndex_Intro4] = emptyCutscene();
	CutsceneArr[CutsceneIndex_Intro4].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Intro4].m_cutsceneImage.imageName = L"IntroCutscene4.bmp";
	CutsceneArr[CutsceneIndex_Intro4].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_Intro4].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_Intro4].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Intro4].m_tex, L"texture\\IntroCutscene4.bmp");
	CutsceneArr[CutsceneIndex_Intro4].m_cutsceneImage.m_layer = 4;
	CutsceneArr[CutsceneIndex_Intro4].m_cutsceneImage.position = GDIEngine_center;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Intro4].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_Intro4].m_CutsceneDC, hBit));


	// 인트로 컷신5 초기화
	// (컷신 배경 초기화)
	CutsceneArr[CutsceneIndex_Intro5] = emptyCutscene();
	CutsceneArr[CutsceneIndex_Intro5].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Intro5].m_cutsceneImage.imageName = L"IntroCutscene5.bmp";
	CutsceneArr[CutsceneIndex_Intro5].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_Intro5].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_Intro5].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Intro5].m_tex, L"texture\\IntroCutscene5.bmp");
	CutsceneArr[CutsceneIndex_Intro5].m_cutsceneImage.m_layer = 5;
	CutsceneArr[CutsceneIndex_Intro5].m_cutsceneImage.position = GDIEngine_center;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Intro5].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_Intro5].m_CutsceneDC, hBit));

	CutsceneArr[CutsceneIndex_Intro5].m_doesFadeOut = TRUE;
	CutsceneArr[CutsceneIndex_Intro5].loadingStage = L"tutorial.txt";
	CutsceneArr[CutsceneIndex_Intro5].m_isBGMChange = TRUE;
}

// 독백 없음
void Cutscene_Tutorial_Init(Vector2i _textPos)
{
	// SoundBGMchange(SoundType_BGM_INGAME);
	HBITMAP hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	Vector2i drawRect = GetVector2i_XY(0, 0);
	// 튜토리얼 초기화
	// (컷신 배경 초기화)
	CutsceneArr[CutsceneIndex_Tutorial] = emptyCutscene();
	CutsceneArr[CutsceneIndex_Tutorial].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Tutorial].m_cutsceneImage.imageName = L"TutorialCutscene.bmp";
	CutsceneArr[CutsceneIndex_Tutorial].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_Tutorial].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_Tutorial].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Tutorial].m_tex, L"texture\\TutorialCutscene.bmp");
	CutsceneArr[CutsceneIndex_Tutorial].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_Tutorial].m_cutsceneImage.position = GDIEngine_center;
	CutsceneArr[CutsceneIndex_Tutorial].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_Tutorial].m_CutsceneDC, hBit));

	CutsceneArr[CutsceneIndex_Tutorial].loadingStage = L"stage1.txt";
	// (컷신 텍스트 초기화)
	CutsceneArr[CutsceneIndex_Tutorial].m_textIndex++;
	CutsceneArr[CutsceneIndex_Tutorial].m_text[CutsceneArr[CutsceneIndex_Tutorial].m_textIndex].imageName = L"TutorialText0.bmp";
	CutsceneArr[CutsceneIndex_Tutorial].m_text[CutsceneArr[CutsceneIndex_Tutorial].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Tutorial].m_text[CutsceneArr[CutsceneIndex_Tutorial].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Tutorial].m_text[CutsceneArr[CutsceneIndex_Tutorial].m_textIndex].m_tex, L"texture\\TutorialText0.bmp");
	CutsceneArr[CutsceneIndex_Tutorial].m_text[CutsceneArr[CutsceneIndex_Tutorial].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Tutorial].m_text[CutsceneArr[CutsceneIndex_Tutorial].m_textIndex].blend.SourceConstantAlpha = 0;

	CutsceneArr[CutsceneIndex_Tutorial].m_textIndex++;
	CutsceneArr[CutsceneIndex_Tutorial].m_text[CutsceneArr[CutsceneIndex_Tutorial].m_textIndex].imageName = L"TutorialText1.bmp";
	CutsceneArr[CutsceneIndex_Tutorial].m_text[CutsceneArr[CutsceneIndex_Tutorial].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Tutorial].m_text[CutsceneArr[CutsceneIndex_Tutorial].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Tutorial].m_text[CutsceneArr[CutsceneIndex_Tutorial].m_textIndex].m_tex, L"texture\\TutorialText1.bmp");
	CutsceneArr[CutsceneIndex_Tutorial].m_text[CutsceneArr[CutsceneIndex_Tutorial].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Tutorial].m_text[CutsceneArr[CutsceneIndex_Tutorial].m_textIndex].blend.SourceConstantAlpha = 0;
}

// 독백 있음
void Cutscene_Stage0_Init(Vector2i _textPos)
{
	HBITMAP hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	Vector2i drawRect = GetVector2i_XY(0, 0);
	// 스테이지1 초기화
	// (컷신 배경 초기화)
	CutsceneArr[CutsceneIndex_Stage0] = emptyCutscene();
	CutsceneArr[CutsceneIndex_Stage0].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Stage0].m_cutsceneImage.imageName = L"Stage1Cutscene.bmp";
	CutsceneArr[CutsceneIndex_Stage0].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_Stage0].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_Stage0].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage0].m_tex, L"texture\\Stage1Cutscene.bmp");
	CutsceneArr[CutsceneIndex_Stage0].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_Stage0].m_cutsceneImage.position = GDIEngine_center;
	CutsceneArr[CutsceneIndex_Stage0].m_doesFadeOut = TRUE;
	CutsceneArr[CutsceneIndex_Stage0].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_Stage0].m_CutsceneDC, hBit));

	// (컷신 텍스트 초기화)
	CutsceneArr[CutsceneIndex_Stage0].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].imageName = L"Stage0Text0.bmp";
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_tex, L"texture\\Stage0Text0.bmp");
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].blend.SourceConstantAlpha = 0;

	CutsceneArr[CutsceneIndex_Stage0].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].imageName = L"Stage0Text1.bmp";
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_tex, L"texture\\Stage0Text1.bmp");
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].blend.SourceConstantAlpha = 0;

	CutsceneArr[CutsceneIndex_Stage0].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].imageName = L"Stage0Text2.bmp";
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_tex, L"texture\\Stage0Text2.bmp");
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].blend.SourceConstantAlpha = 0;

	CutsceneArr[CutsceneIndex_Stage0].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].imageName = L"Stage0Text3.bmp";
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_tex, L"texture\\Stage0Text3.bmp");
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].blend.SourceConstantAlpha = 0;

	CutsceneArr[CutsceneIndex_Stage0].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].imageName = L"Stage0Text4.bmp";
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_tex, L"texture\\Stage0Text4.bmp");
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].blend.SourceConstantAlpha = 0;

	CutsceneArr[CutsceneIndex_Stage0].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].imageName = L"Stage0Text5.bmp";
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].m_tex, L"texture\\Stage0Text5.bmp");
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage0].m_text[CutsceneArr[CutsceneIndex_Stage0].m_textIndex].blend.SourceConstantAlpha = 0;

	// 스테이지1 독백
	CutsceneArr[CutsceneIndex_Stage0_Monologue] = emptyCutscene();
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_cutsceneImage.imageName = L"Fade.bmp";
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage0_Monologue].m_tex, L"texture\\Fade.bmp");
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_cutsceneImage.position = GDIEngine_center;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_Stage0_Monologue].m_CutsceneDC, hBit));
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_isMonologue = TRUE;
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_doesFadeOut = TRUE;
	// (스테이지1 독백 텍스트 초기화)
	_textPos.y = MONOLOGUE_TEXT_ORIGIN_Y_POS;
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage0_Monologue].m_textIndex].imageName = L"Stage0Monologue0.bmp";
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage0_Monologue].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage0_Monologue].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage0_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage0_Monologue].m_textIndex].m_tex, L"texture\\Stage0Monologue0.bmp");
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage0_Monologue].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage0_Monologue].m_textIndex].blend.SourceConstantAlpha = 0;

	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage0_Monologue].m_textIndex].imageName = L"Stage0Monologue1.bmp";
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage0_Monologue].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage0_Monologue].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage0_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage0_Monologue].m_textIndex].m_tex, L"texture\\Stage0Monologue1.bmp");
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage0_Monologue].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage0_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage0_Monologue].m_textIndex].blend.SourceConstantAlpha = 0;
	CutsceneArr[CutsceneIndex_Stage0_Monologue].loadingStage = L"stage2.txt";
}

// 독백 있음
void Cutscene_Stage1_Init(Vector2i _textPos)
{
	HBITMAP hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	Vector2i drawRect = GetVector2i_XY(0, 0);
	// 스테이지2 초기화
	// (컷신 배경 초기화)
	CutsceneArr[CutsceneIndex_Stage1] = emptyCutscene();
	CutsceneArr[CutsceneIndex_Stage1].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Stage1].m_cutsceneImage.imageName = L"Stage2Cutscene.bmp";
	CutsceneArr[CutsceneIndex_Stage1].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_Stage1].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_Stage1].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage1].m_tex, L"texture\\Stage2Cutscene.bmp");
	CutsceneArr[CutsceneIndex_Stage1].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_Stage1].m_cutsceneImage.position = GDIEngine_center;
	CutsceneArr[CutsceneIndex_Stage1].m_doesFadeOut = TRUE;
	CutsceneArr[CutsceneIndex_Stage1].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_Stage1].m_CutsceneDC, hBit));

	// (컷신 텍스트 초기화)
	CutsceneArr[CutsceneIndex_Stage1].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].imageName = L"Stage1Text0.bmp";
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].m_tex, L"texture\\Stage1Text0.bmp");
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].blend.SourceConstantAlpha = 0;

	CutsceneArr[CutsceneIndex_Stage1].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].imageName = L"Stage1Text1.bmp";
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].m_tex, L"texture\\Stage1Text1.bmp");
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].blend.SourceConstantAlpha = 0;

	CutsceneArr[CutsceneIndex_Stage1].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].imageName = L"Stage1Text2.bmp";
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].m_tex, L"texture\\Stage1Text2.bmp");
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].blend.SourceConstantAlpha = 0;

	CutsceneArr[CutsceneIndex_Stage1].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].imageName = L"Stage1Text3.bmp";
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].m_tex, L"texture\\Stage1Text3.bmp");
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].blend.SourceConstantAlpha = 0;

	CutsceneArr[CutsceneIndex_Stage1].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].imageName = L"Stage1Text4.bmp";
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].m_tex, L"texture\\Stage1Text4.bmp");
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage1].m_text[CutsceneArr[CutsceneIndex_Stage1].m_textIndex].blend.SourceConstantAlpha = 0;

	// 스테이지2 컷씬 독백 초기화
	CutsceneArr[CutsceneIndex_Stage1_Monologue] = emptyCutscene();
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_cutsceneImage.imageName = L"Fade.bmp";
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage1_Monologue].m_tex, L"texture\\Fade.bmp");
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_cutsceneImage.position = GDIEngine_center;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_Stage1_Monologue].m_CutsceneDC, hBit));
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_isMonologue = TRUE;
	// (컷신 텍스트 초기화)
	_textPos.y = MONOLOGUE_TEXT_ORIGIN_Y_POS;
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage1_Monologue].m_textIndex].imageName = L"Stage1Monologue0.bmp";
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage1_Monologue].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage1_Monologue].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage1_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage1_Monologue].m_textIndex].m_tex, L"texture\\Stage1Monologue0.bmp");
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage1_Monologue].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage1_Monologue].m_textIndex].blend.SourceConstantAlpha = 0;

	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage1_Monologue].m_textIndex].imageName = L"Stage1Monologue1.bmp";
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage1_Monologue].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage1_Monologue].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage1_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage1_Monologue].m_textIndex].m_tex, L"texture\\Stage1Monologue1.bmp");
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage1_Monologue].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage1_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage1_Monologue].m_textIndex].blend.SourceConstantAlpha = 0;
	CutsceneArr[CutsceneIndex_Stage1_Monologue].loadingStage = L"stage3.txt";
}

// 독백 있음
void Cutscene_Stage2_Init(Vector2i _textPos)
{
	HBITMAP hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	Vector2i drawRect = GetVector2i_XY(0, 0);
	// 스테이지3 초기화
	// (컷신 배경 초기화)
	CutsceneArr[CutsceneIndex_Stage2] = emptyCutscene();
	CutsceneArr[CutsceneIndex_Stage2].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Stage2].m_cutsceneImage.imageName = L"Stage3Cutscene.bmp";
	CutsceneArr[CutsceneIndex_Stage2].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_Stage2].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_Stage2].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage2].m_tex, L"texture\\Stage3Cutscene.bmp");
	CutsceneArr[CutsceneIndex_Stage2].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_Stage2].m_cutsceneImage.position = GDIEngine_center;
	CutsceneArr[CutsceneIndex_Stage2].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_Stage2].m_CutsceneDC, hBit));
	CutsceneArr[CutsceneIndex_Stage2].m_doesFadeOut = TRUE;
	CutsceneArr[CutsceneIndex_Stage2].m_isBGMChange = TRUE;

	// 산모수첩 엔딩 컷씬 초기화
	CutsceneArr[CutsceneIndex_HappayEnding0] = emptyCutscene();
	CutsceneArr[CutsceneIndex_HappayEnding0].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_HappayEnding0].m_cutsceneImage.imageName = L"NoteCutscene0.bmp";
	CutsceneArr[CutsceneIndex_HappayEnding0].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_HappayEnding0].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_HappayEnding0].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_HappayEnding0].m_tex, L"texture\\NoteCutscene0.bmp");
	CutsceneArr[CutsceneIndex_HappayEnding0].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_HappayEnding0].m_cutsceneImage.position = GDIEngine_center;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_HappayEnding0].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_HappayEnding0].m_CutsceneDC, hBit));

	CutsceneArr[CutsceneIndex_HappayEnding1] = emptyCutscene();
	CutsceneArr[CutsceneIndex_HappayEnding1].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_HappayEnding1].m_cutsceneImage.imageName = L"NoteCutscene1.bmp";
	CutsceneArr[CutsceneIndex_HappayEnding1].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_HappayEnding1].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_HappayEnding1].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_HappayEnding1].m_tex, L"texture\\NoteCutscene1.bmp");
	CutsceneArr[CutsceneIndex_HappayEnding1].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_HappayEnding1].m_cutsceneImage.position = GDIEngine_center;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_HappayEnding1].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_HappayEnding1].m_CutsceneDC, hBit));

	CutsceneArr[CutsceneIndex_HappayEnding2] = emptyCutscene();
	CutsceneArr[CutsceneIndex_HappayEnding2].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_HappayEnding2].m_cutsceneImage.imageName = L"NoteCutscene2.bmp";
	CutsceneArr[CutsceneIndex_HappayEnding2].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_HappayEnding2].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_HappayEnding2].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_HappayEnding2].m_tex, L"texture\\NoteCutscene2.bmp");
	CutsceneArr[CutsceneIndex_HappayEnding2].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_HappayEnding2].m_cutsceneImage.position = GDIEngine_center;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_HappayEnding2].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_HappayEnding2].m_CutsceneDC, hBit));

	CutsceneArr[CutsceneIndex_HappayEnding3] = emptyCutscene();
	CutsceneArr[CutsceneIndex_HappayEnding3].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_HappayEnding3].m_cutsceneImage.imageName = L"NoteCutscene3.bmp";
	CutsceneArr[CutsceneIndex_HappayEnding3].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_HappayEnding3].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_HappayEnding3].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_HappayEnding3].m_tex, L"texture\\NoteCutscene3.bmp");
	CutsceneArr[CutsceneIndex_HappayEnding3].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_HappayEnding3].m_cutsceneImage.position = GDIEngine_center;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_HappayEnding3].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_HappayEnding3].m_CutsceneDC, hBit));

	CutsceneArr[CutsceneIndex_HappayEnding4] = emptyCutscene();
	CutsceneArr[CutsceneIndex_HappayEnding4].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_HappayEnding4].m_cutsceneImage.imageName = L"NoteCutscene4.bmp";
	CutsceneArr[CutsceneIndex_HappayEnding4].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_HappayEnding4].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_HappayEnding4].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_HappayEnding4].m_tex, L"texture\\NoteCutscene4.bmp");
	CutsceneArr[CutsceneIndex_HappayEnding4].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_HappayEnding4].m_cutsceneImage.position = GDIEngine_center;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_HappayEnding4].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_HappayEnding4].m_CutsceneDC, hBit));

	CutsceneArr[CutsceneIndex_HappayEnding5] = emptyCutscene();
	CutsceneArr[CutsceneIndex_HappayEnding5].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_HappayEnding5].m_cutsceneImage.imageName = L"NoteCutscene5.bmp";
	CutsceneArr[CutsceneIndex_HappayEnding5].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_HappayEnding5].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_HappayEnding5].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_HappayEnding5].m_tex, L"texture\\NoteCutscene5.bmp");
	CutsceneArr[CutsceneIndex_HappayEnding5].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_HappayEnding5].m_cutsceneImage.position = GDIEngine_center;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_HappayEnding5].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_HappayEnding5].m_CutsceneDC, hBit));
	CutsceneArr[CutsceneIndex_HappayEnding5].m_doesFadeOut = TRUE;

	// 스테이지3 독백 컷씬 초기화
	CutsceneArr[CutsceneIndex_Stage2_Monologue] = emptyCutscene();
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_cutsceneImage.imageName = L"Fade.bmp";
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_isBGMChange = TRUE;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage2_Monologue].m_tex, L"texture\\Fade.bmp");
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_cutsceneImage.position = GDIEngine_center;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_Stage2_Monologue].m_CutsceneDC, hBit));
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_isMonologue = TRUE;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].loadingStage = L"stage4.txt";
	// 스테이지3 독백 텍스트 초기화
	_textPos.y = MONOLOGUE_TEXT_ORIGIN_Y_POS;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].imageName = L"Stage2Monologue0.bmp";
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].m_tex, L"texture\\Stage2Monologue0.bmp");
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].blend.SourceConstantAlpha = 0;

	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].imageName = L"Stage2Monologue1.bmp";
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].m_tex, L"texture\\Stage2Monologue1.bmp");
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].blend.SourceConstantAlpha = 0;

	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].imageName = L"Stage2Monologue2.bmp";
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].m_tex, L"texture\\Stage2Monologue2.bmp");
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].blend.SourceConstantAlpha = 0;

	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex++;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].imageName = L"Stage2Monologue3.bmp";
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].m_tex, L"texture\\Stage2Monologue3.bmp");
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_Stage2_Monologue].m_text[CutsceneArr[CutsceneIndex_Stage2_Monologue].m_textIndex].blend.SourceConstantAlpha = 0;
}

// 독백 있음
void Cutscene_HappyEnd_Init(Vector2i _textPos)
{
	HBITMAP hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	Vector2i drawRect = GetVector2i_XY(0, 0);


	// 해피엔딩 독백 초기화 (배경)
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue] = emptyCutscene();
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_cutsceneImage.imageName = L"Fade.bmp";
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_tex, L"texture\\Fade.bmp");
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_cutsceneImage.position = GDIEngine_center;
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_CutsceneDC, hBit));
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_doesFadeOut = TRUE;
	// 해피엔딩 독백 초기화
	_textPos.y = MONOLOGUE_TEXT_ORIGIN_Y_POS;
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_textIndex++;
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_text[CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_textIndex].imageName = L"HappyEnd_Monologue0.bmp";
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_text[CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_text[CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_text[CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_textIndex].m_tex, L"texture\\HappyEnd_Monologue0.bmp");
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_text[CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_text[CutsceneArr[CutsceneIndex_HappyEnding_Monologue].m_textIndex].blend.SourceConstantAlpha = 0;

	// 해피엔딩 초기화
	// (컷신 배경 초기화)
	CutsceneArr[CutsceneIndex_HappyEnding] = emptyCutscene();
	CutsceneArr[CutsceneIndex_HappyEnding].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_HappyEnding].m_cutsceneImage.imageName = L"happy end.bmp";
	CutsceneArr[CutsceneIndex_HappyEnding].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_HappyEnding].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_HappyEnding].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_HappyEnding].m_tex, L"texture\\happy end.bmp");
	CutsceneArr[CutsceneIndex_HappyEnding].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_HappyEnding].m_cutsceneImage.position = GDIEngine_center;
	//CutsceneArr[CutsceneIndex_HappyEnding].m_isEndCutscene = TRUE;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_HappyEnding].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_HappyEnding].m_CutsceneDC, hBit));
	CutsceneArr[CutsceneIndex_HappyEnding].m_doesFadeOut = TRUE;
}

// 독백 있음
void Cutscene_BadEnd_Init(Vector2i _textPos)
{
	HBITMAP hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	Vector2i drawRect = GetVector2i_XY(0, 0);


	// 배드엔딩 독백 배경 초기화
	CutsceneArr[CutsceneIndex_BadEnding_Monologue] = emptyCutscene();
	CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_cutsceneImage.imageName = L"Fade.bmp";
	CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_tex, L"texture\\Fade.bmp");
	CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_cutsceneImage.position = GDIEngine_center;
	CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_CutsceneDC, hBit));
	CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_doesFadeOut = TRUE;
	// 배드엔딩 독백 텍스트 초기화
	_textPos.y = MONOLOGUE_TEXT_ORIGIN_Y_POS;
	CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_textIndex++;
	CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_text[CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_textIndex].imageName = L"badEnd_Monologue0.bmp";
	CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_text[CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_textIndex].m_drawRect = drawRect;
	CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_text[CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_textIndex].m_isActive = FALSE;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_text[CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_textIndex].m_tex, L"texture\\badEnd_Monologue0.bmp");
	CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_text[CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_textIndex].position = _textPos;
	CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_text[CutsceneArr[CutsceneIndex_BadEnding_Monologue].m_textIndex].blend.SourceConstantAlpha = 0;

	// 배드엔딩 초기화
	// (컷신 배경 초기화)
	CutsceneArr[CutsceneIndex_BadEnding] = emptyCutscene();
	CutsceneArr[CutsceneIndex_BadEnding].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_BadEnding].m_cutsceneImage.imageName = L"bad end.bmp";
	CutsceneArr[CutsceneIndex_BadEnding].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_BadEnding].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_BadEnding].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_BadEnding].m_tex, L"texture\\bad end.bmp");
	CutsceneArr[CutsceneIndex_BadEnding].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_BadEnding].m_cutsceneImage.position = GDIEngine_center;
	//CutsceneArr[CutsceneIndex_BadEnding].m_isEndCutscene = TRUE;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_BadEnding].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_BadEnding].m_CutsceneDC, hBit));

	CutsceneArr[CutsceneIndex_BadEnding].m_doesFadeOut = TRUE;
}

void Cutscene_EndingCresdit_Init(Vector2i _textPos)
{
	HBITMAP hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	Vector2i drawRect = GetVector2i_XY(0, 0);

	CutsceneArr[CutsceneIndex_EndingCredit] = emptyCutscene();
	CutsceneArr[CutsceneIndex_EndingCredit].m_cutsceneImage.m_drawRect = GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_EndingCredit].m_cutsceneImage.imageName = L"endingCredit.bmp";
	CutsceneArr[CutsceneIndex_EndingCredit].m_cutsceneImage.m_isActive = FALSE;
	CutsceneArr[CutsceneIndex_EndingCredit].m_isFirst = TRUE;
	CutsceneArr[CutsceneIndex_EndingCredit].m_textIndex = -1;
	ResManager_LoadTexture(&CutsceneArr[CutsceneIndex_EndingCredit].m_tex, L"texture\\endingCredit.bmp");
	CutsceneArr[CutsceneIndex_EndingCredit].m_cutsceneImage.m_layer = 6;
	CutsceneArr[CutsceneIndex_EndingCredit].m_cutsceneImage.position = GDIEngine_center;
	CutsceneArr[CutsceneIndex_EndingCredit].m_isEndCutscene = TRUE;
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	CutsceneArr[CutsceneIndex_EndingCredit].m_CutsceneDC = CreateCompatibleDC(g_hScreenDC);
	DeleteObject((HBITMAP)SelectObject(CutsceneArr[CutsceneIndex_EndingCredit].m_CutsceneDC, hBit));
}

void Cutscene_CutsceneToMainTitle()
{
	g_blackMask.onFadeOutFinish = CallBack_CutsceneToMainTitle;
	g_blackMask.m_state = BlackMask_State_FADEOUT;
	onChangeBGM = ChangeBGM_GoToMainTitle;
	SoundState = Sound_State_FADEOUT;


	g_blackMask.m_acc = 0;
}

void CallBack_CutsceneToMainTitle()
{
	//stageInstance.state = Stage_State_NONE;
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

	// 게임이 끝난것이기 때문에 사운드 키고 이미지도 바꾸자
	g_isSoundMute = FALSE;
	g_isMute = FALSE;
	SetVolume(1);
	UIWindow_Ingame.uiButtons[0].imageName = soundOnButtonName;
}

void Cutscene_NextCutscene()
{
	// 현재 컷씬에 텍스트가 없다면
  	if (CutsceneManager_CurCutscene->m_textIndex == -1)
	{
		DesolveCutscene = &CutsceneArr[cutNum];

		++cutNum;

		if (cutNum > CutsceneIndex_EndingCredit)
		{
			cutNum = CutsceneIndex_EndingCredit;
			CutsceneManager_CurCutscene = &CutsceneArr[cutNum];
			DesolveCutscene = NULL;
		}
		if (g_isSelectEnding)
		{
			if (g_isHappyEnd)
			{
				while (cutNum != CutsceneIndex_EndingCredit)
				{
					++cutNum;
				}
			}
		}

		// 만약 현재 컷씬 다음이 스테이지를 불러와야한다면
		if (CutsceneManager_CurCutscene->m_isEndCutscene)
		{
			Cutscene_CutsceneToMainTitle();
		}
		else if (CutsceneManager_CurCutscene->loadingStage)
		{
			Cutscene_CutsceneToStage();
		}
		else if (CutsceneManager_CurCutscene->m_doesFadeOut)
		{
			// 스테이지 -> 컷신
			g_isStageToCutscene = TRUE;
			Cutscene_LoadCutscene();
		}
		else
		{
			// 컷신 -> 컷신
			g_isStageToCutscene = FALSE;
			CallBack_CutsceneLoad();
			//Cutscene_CutsceneToCutscene();
		}
	}
	else
	{
		// 두번째 텍스트부터는 스페이스바를 눌러야 올라옴
		CutsceneManager_CurCutscene->m_text[tempTextIndex].m_isActive = FALSE;

		++tempTextIndex;
		CutsceneManager_CurCutscene->m_text[tempTextIndex].m_isActive = TRUE;

		// 현재 컷씬의 텍스트가 모두 보여지고 나서의 후처리
		if (tempTextIndex > CutsceneManager_CurCutscene->m_textIndex)
		{
			tempTextIndex = 0;
			++cutNum;
			g_isTextShow = FALSE;

			// 아래는 테스트용임 실제로는 위의 함수 써야함.
			if (CutsceneManager_CurCutscene->loadingStage)
			{
				Cutscene_CutsceneToStage();
			}
			else if (CutsceneManager_CurCutscene->m_doesFadeOut)
			{
				g_isStageToCutscene = TRUE;
				Cutscene_LoadCutscene();
			}
			else
			{
				g_isStageToCutscene = FALSE;
				//Cutscene_CutsceneToCutscene();
				CallBack_CutsceneLoad();
			}
		}
	}
}

Cutscene emptyCutscene()
{
	Cutscene cutscene;
	memset(&cutscene, 0, sizeof(Cutscene));
	return cutscene;
}

void Cutscene_CutsceneToCutscene()
{
	//if (CutsceneManager_CurCutscene->m_isBGMChange && (onChangeBGM != ChangeBGM_StageToCutscene))
	//{
	//    onChangeBGM = ChangeBGM_CutsceneToCutscene;
	//    SoundState = Sound_State_FADEOUT;
	//}
	if (CutsceneManager_CurCutscene->m_isBGMChange && (cutNum != 1))
	{
		onChangeBGM = ChangeBGM_CutsceneToCutscene;
		SoundState = Sound_State_FADEOUT;
	}

	//g_blackMask.m_state = BlackMask_State_FADEOUT;

	g_isCutsceneActive = TRUE;

	stageInstance.state = Stage_State_NONE;
	g_isTextShow = TRUE;
	CutsceneManager_CurCutscene = &CutsceneArr[cutNum];
	CutsceneManager_CurCutscene->m_cutsceneImage.m_isActive = TRUE;
	g_isNowCutscene = TRUE;
	UIWindow_currentWindow = &UIWindow_Cutscene;
}

void Cutscene_GoToEndPoint()
{
	g_blackMask.m_state = BlackMask_State_FADEOUT;
	g_blackMask.onFadeOutFinish = CallBack_GoToEndPoint;
	cutNum = CutsceneIndex_Stage2_Monologue;
	SoundState = Sound_State_FADEOUT;
	onChangeBGM = ChangBGM_GotoEndPoint;
	CutsceneManager_CurCutscene = &CutsceneArr[cutNum];
}
void CallBack_GoToEndPoint()
{
	g_isNowCutscene = FALSE;
	stageInstance.state = Stage_State_PLAYING;
	cutNum++;
	UIWindow_currentWindow = &UIWindow_Ingame;
	Stage_LoadStage(CutsceneManager_CurCutscene->loadingStage);
}