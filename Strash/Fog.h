#pragma once
#include "Vector2i.h"

// 안개의 상태를 나타냅니다.
// 존재하지 않음, 존재, 안개가 끼는 중, 안개가 걷히는 중의 상태로 나뉩니다.
typedef enum
{
    FogState_NULL, FogState_EXISTENT, FogState_EMERGING, FogState_DISAPPEARING
}
FogState;

// 안개입니다. 매우 수동적인 개체로, Update와 Render가 모두 Stage에 의해 호출됩니다.
typedef struct
{
    // 안개의 상태를 나타냅니다.
    FogState state;
    // 스테이지의 progress가 disappearProgress를 넘어서면 안개가 사라집니다.
    int disappearProgress;
}
Fog;

void Fog_Update(Fog* target);
void Fog_Render(Fog* target, Vector2i position);
