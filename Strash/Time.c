#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include "GDIEngine.h"
#include "Time.h"
#include "GDIEngine.h"


void Time_Initialize()
{
    Time_Instance.deltaTime = 0;
    Time_Instance.FPS = 0;
}

// 업데이트가 돌며 프레임과 프레임 사이의 시간 간격과 초당 프레임 수를 기록합니다.
// 다른 게임 객체들의 동작이 이 시간간격에 의존할 수 있으므로, 라이프 사이클 중 가장 먼저 호출되어야만 합니다.
void Time_Update()
{
    struct timespec ts;
    static double timeStampQueue[500000] = { 0 };
    static int timeStampQueueFront = 0;
    static int timeStampQueueEnd = 0;
    timespec_get(&ts, TIME_UTC);
    static double lastTime = 0;
    double currentTime = ts.tv_sec + ts.tv_nsec / (double)1000000000;
    Time_Instance.deltaTime = lastTime == 0 ? 0 : currentTime - lastTime;

    timeStampQueue[timeStampQueueEnd] = currentTime;
    timeStampQueueEnd = ++timeStampQueueEnd % 500000;
    while (timeStampQueue[timeStampQueueFront] < currentTime - 1 && timeStampQueueEnd != timeStampQueueFront)
        timeStampQueueFront = ++timeStampQueueFront;

    Time_Instance.FPS = (timeStampQueueEnd < timeStampQueueEnd ? timeStampQueueEnd + 500000 : timeStampQueueEnd) - timeStampQueueFront;
    lastTime = currentTime;

	//wchar_t buffer[255] = { NULL }; 
	//swprintf_s(buffer, 255, L"FPS : %d, DT : %f", Time_Instance.FPS, Time_Instance.deltaTime);
	//SetWindowText(g_hWnd, buffer);
}

void DebugTime()
{

} 