#pragma once
#include "Vector2i.h"
#include "Vector2d.h"

typedef enum
{
    Protagonist_State_Idle,
    Protagonist_State_Walking,
    Protagonist_State_Pushing,
}Protagonist_State;

const double Protagonist_pushDuration;
const double Protagonist_walkDuration;
// Protagonist는 주인공이란 뜻입니다.
// 대부분의 게임의 경우, 사용자 입력을 주인공 객체에서 처리하는게 마땅하지만,
// 입력에 대한 동작을 정의하기 위해서는 현재 타일 상태에 대한 정보가 있어야 하기에, 사용자 입력이 여기에서 처리되지는 않습니다.
// 주인공 캐릭터의 애니메이션 재생 로직을 정의합니다.
typedef struct
{
    Protagonist_State state;
    double action_elapsed;
    Vector2d walk_src;
    Vector2d walk_dst;
    Vector2i walk_current;
    Vector2i walk_offset;
    Vector2i facingVector;
} Protagonist;
Protagonist Protagonist_Instance;

void Protagonist_MoveTo(Vector2i src, Vector2i dst);
void Protagonist_Push();
void Protagonist_Push_Futile();
void Protagonist_Update();
void Protagonist_Render(Vector2i position, Vector2i drawRect);
