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

// ������Ʈ�� ���� �����Ӱ� ������ ������ �ð� ���ݰ� �ʴ� ������ ���� ����մϴ�.
// �ٸ� ���� ��ü���� ������ �� �ð����ݿ� ������ �� �����Ƿ�, ������ ����Ŭ �� ���� ���� ȣ��Ǿ�߸� �մϴ�.
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