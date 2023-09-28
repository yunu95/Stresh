#pragma once
/// 시간을 관리하는 모듈

double TimeControl_deltaTime;
int TimeControl_FPS;
// 시간을 다루는 모듈 초기화
void TimeControl_Initialize();
void TimeControl_Update();


// 2개의 시각 사이의 시간을 재는 함수
/// Elasped Time 

// 이전 프레임과 현재 프레임간의 걸린 시간
/// Elasped Time (DeltaTime)

// 프레임 제어 함수
/// 만약, 60FPS의 경우 deltaTime이 16.7ms보다 작으면
/// 적극적인 대기를 한다.


