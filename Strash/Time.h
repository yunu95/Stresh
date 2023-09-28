#pragma once
/// 시간을 관리하는 모듈

// 시간은 델타 시간을 저장하여 타 객체들의 업데이트 함수에서 유용히 쓰일 수 있도록 합니다.
typedef struct
{
    double deltaTime;
    int FPS;
}
Time;

Time Time_Instance;

// 시간을 다루는 모듈 초기화
void Time_Initialize();
// 시간 관리에 쓰이는 업데이트 주기 함수입니다. 많은 동작들이 Time의 상태에 의존적이므로, 이 함수는 다른 모든 업데이트 함수보다 더 우선적으로 호출되어야 합니다.
void Time_Update();


// 2개의 시각 사이의 시간을 재는 함수
/// Elasped Time 

// 이전 프레임과 현재 프레임간의 걸린 시간
/// Elasped Time (DeltaTime)

// 프레임 제어 함수
/// 만약, 60FPS의 경우 deltaTime이 16.7ms보다 작으면
/// 적극적인 대기를 한다.

void DebugTime();
