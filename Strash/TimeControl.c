#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include "TimeControl.h"

double TimeControl_deltaTime = 0;
int FPS = 0;

void TimeControl_Initialize()
{
}

// ������Ʈ�� ���� �����Ӱ� ������ ������ �ð� ���ݰ� �ʴ� ������ ���� ����մϴ�.
// �ٸ� ���� ��ü���� ������ �� �ð����ݿ� ������ �� �����Ƿ�, ������ ����Ŭ �� ���� ���� ȣ��Ǿ�߸� �մϴ�.
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
