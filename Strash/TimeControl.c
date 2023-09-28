#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include "TimeControl.h"

double TimeControl_deltaTime = 0;
int FPS = 0;

void TimeControl_Initialize()
{
}

// 업데이트가 돌며 프레임과 프레임 사이의 시간 간격과 초당 프레임 수를 기록합니다.
// 다른 게임 객체들의 동작이 이 시간간격에 의존할 수 있으므로, 라이프 사이클 중 가장 먼저 호출되어야만 합니다.
void TimeControl_Update()
{
    struct timespec ts;
    static double timeStampQueue[500000] = { 0 };
    static int timeStampQueueFront = 0;
    static int timeStampQueueEnd = 0;
    timespec_get(&ts, TIME_UTC);
    static double lastTime = 0;
    double currentTime =ts.tv_sec+ ts.tv_nsec / (double)1000000000;
    TimeControl_deltaTime = currentTime - lastTime;

    timeStampQueue[timeStampQueueEnd] = currentTime;
    timeStampQueueEnd = ++timeStampQueueEnd % 500000;
    while (timeStampQueue[timeStampQueueFront] < currentTime - 1 && timeStampQueueEnd != timeStampQueueFront)
        timeStampQueueFront = ++timeStampQueueFront;

    TimeControl_FPS = (timeStampQueueEnd < timeStampQueueEnd ? timeStampQueueEnd + 500000 : timeStampQueueEnd) - timeStampQueueFront;
    lastTime = currentTime;
}
