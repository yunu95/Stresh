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
/// ���� 1.
/// ���� �⺻���� ������ Windows ���α׷� (�����ڵ� ����)
/// WinMain ����Ʈ

// ���漱��
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/// <summary>
/// Win32API�� �̿��� ����ũ�� ���ø����̼�
/// ���α׷��� ��Ʈ�� ����Ʈ
/// </summary>
/// <param name="hInstance">�ν��Ͻ��� �ڵ� (��� �����) </param>
/// <param name="hPrevInstance">���� �ν��Ͻ��� �ڵ� (Win16�� ����)</param>
/// <param name="lpCmdLine">���α׷� �ڿ� ���̴� �̳�</param>
/// <param name="nCmdShow"></param>
/// <returns></returns>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    SoundBGMchange(SoundType_BGM_MAIN);
    //SoundPlay(0);
    /// (���� �� ������) ���
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;		/// ���� ���ν���
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

    /// (�� ������) ����
    HWND hWnd = CreateWindowW(L"COLORZZLEWIN32E", L"Stresh", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    //HWND hWnd = CreateWindowW(L"COLORZZLEWIN32E", L"Stresh", WS_POPUP,
    //    0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);

    /*HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW, L"COLORZZLEWIN32E", L"Stresh", WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);*/



    // ���� �ڵ��� ���� �߿�
    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    /// GDI �׷��Ƚ� ������ �ʱ�ȭ�Ѵ�.
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

    /// ���ѷ��� (�ȿ� �޽��� ������ ���� ���� ���� ���Ե�)

    /// �޽��� ����
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
            //GDI_WriteText(L"�����ٶ󸶹ٻ������īŸ����ABCDEF", pos, area);

            Vector2i drawRect = { 256,256 };
            //GDI_DrawBitmap(L"bloodlust.bmp", tempVec, drawRect);

            Stage_Update();
            Letter_Update();


            // �Ʒ��� �����Լ���


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

/// ���� �޽����� ó�� �� �޽��� ���ν���
// �����ϸ�, �޽��� ���ν����� ���� �۵��ϰ� �Ѵ�.
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
