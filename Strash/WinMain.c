#include <windows.h>
#include "Time.h"
#include "GDIEngine.h"
#include "Stage.h"
#include "Cutscene.h"
#include "Letter.h"
#include "UserInput.h"
#include "PathManager.h"
#include "BlackMask.h"
#include "MovementHandler.h"
#include "ReloadTransition.h"
#include "Sound.h"
//#include "GDIEngine.h"
//#include "CGDIEngine.h"

///
/// 예제 1.
/// 가장 기본적인 형태의 Windows 프로그램 (유니코드 버전)
/// WinMain 디폴트

// 전방선언
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/// <summary>
/// Win32API를 이용한 데스크톱 어플리케이션
/// 프로그램의 엔트리 포인트
/// </summary>
/// <param name="hInstance">인스턴스의 핸들 (어디에 쓸까요) </param>
/// <param name="hPrevInstance">이전 인스턴스의 핸들 (Win16의 잔재)</param>
/// <param name="lpCmdLine">프로그램 뒤에 붙이는 이낮</param>
/// <param name="nCmdShow"></param>
/// <returns></returns>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    SoundBGMchange(SoundType_BGM_MAIN);
    //SoundPlay(0);
    /// (내가 쓸 윈도를) 등록
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;		/// 윈도 프로시저
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"COLORZZLEWIN32E";
    wcex.hIconSm = NULL;

    RegisterClassExW(&wcex);

    /// (그 윈도를) 생성
    HWND hWnd = CreateWindowW(L"COLORZZLEWIN32E", L"Stresh", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    //HWND hWnd = CreateWindowW(L"COLORZZLEWIN32E", L"Stresh", WS_POPUP,
    //    0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);

    /*HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW, L"COLORZZLEWIN32E", L"Stresh", WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);*/



    // 윈도 핸들이 가장 중요
    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    /// GDI 그래픽스 엔진을 초기화한다.
    GDI_Initialize(hWnd);
    //GDI_Font_Init();

    PathManager_Init();
    MovementHandler_Init();
    ReloadTransition_Init();
    SoundInit();

    //tempVec = Vector2i_Zero;
    Vector2i v1 = Vector2i_Zero;
    Vector2i v2 = Vector2i_Zero;
    v1.y = 0;
    v1.x = 1000;
    v2.x = 00;
    v2.y = 1000;
    //MovementHandler_EaseOutVector(&tempVec, v1, v2, 10);
    Time_Initialize();
    //ResManager_Init();
    Cutscene_Init();
    BlackMask_Init();
    UIWindow_Init();
    //CGDIEngine _GDIGraphicsEngine;
    //_GDIGraphicsEngine.Initialize(hWnd);

    //Stage_ChangeStage(L"stage1.txt");
    //Stage_LoadStage(L"temp.txt");

    /// 무한루프 (안에 메시지 루프와 게임 루프 등이 포함됨)

    /// 메시지 루프
    MSG msg;
    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            DispatchMessage(&msg);
        }
        else
        {
            GDI_ResetBuffer();

            Time_Update();

            UserInput_Update();
            MovementHandler_Update();
            Cutscene_Update();
            UIWindow_Update();
            ReloadTransition_Update();
            BlackMask_Update();
            Sound_Update();


            Vector2i pos = { 20,20 };
            RECT area = { 500,500,800,800 };
            //GDI_WriteText(L"가나다라마바사아자차카타파하ABCDEF", pos, area);

            Vector2i drawRect = { 256,256 };
            //GDI_DrawBitmap(L"bloodlust.bmp", tempVec, drawRect);

            Stage_Update();
            Letter_Update();


            // 아래는 렌더함수들


            Stage_Render(GDIEngine_center);
            Cutscene_Render();
            UIWindow_Render();
            ReloadTransition_Render();
            BlackMask_Render();

            GDI_ApplyBuffer();

            // manager..
            int sleepOVer = 16 - (int)(0.5 + Time_Instance.deltaTime * 1000);
            if (sleepOVer > 0)
                Sleep(sleepOVer);
        }
    }

    return 0;
}

/// 윈도 메시지를 처리 할 메시지 프로시저
// 가능하면, 메시지 프로시저를 만들어서 작동하게 한다.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    switch (message)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    break;

    default:
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    }
    return 0;
}
