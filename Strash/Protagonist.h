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
// Protagonist�� ���ΰ��̶� ���Դϴ�.
// ��κ��� ������ ���, ����� �Է��� ���ΰ� ��ü���� ó���ϴ°� ����������,
// �Է¿� ���� ������ �����ϱ� ���ؼ��� ���� Ÿ�� ���¿� ���� ������ �־�� �ϱ⿡, ����� �Է��� ���⿡�� ó�������� �ʽ��ϴ�.
// ���ΰ� ĳ������ �ִϸ��̼� ��� ������ �����մϴ�.
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
