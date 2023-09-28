#pragma once
// �� ��������� 2���� ���Ϳ� �׿� ���õ� �Լ��� �����մϴ�.
typedef struct
{
    double x, y;
}Vector2d;

struct Vector2i;
// 2���� ���͸� ���������մϴ�. t=0�϶� aVector��, t=1�϶� bVector�� ��ȯ�մϴ�.
Vector2d Vector2d_Lerp(Vector2d aVector,Vector2d bVector,double t);
Vector2d GetVector2d(struct Vector2i vec2i);
