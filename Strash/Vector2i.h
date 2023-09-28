#pragma once
#include <Windows.h>

// ������ 2���� �����Դϴ�. ��� ��ǥ�� ǥ���Ҷ� ���Դϴ�.
typedef struct
{
    int x, y;
}
Vector2i;
const Vector2i Vector2i_Zero;
const Vector2i Vector2i_one;
Vector2i Vector2i_Lerp(Vector2i aVector,Vector2i bVector,double t);
Vector2i GetVector2i(struct Vector2d);
Vector2i GetVector2i_XY(double x,double y);
Vector2i Vector2i_Sum(Vector2i a,Vector2i b);
BOOL Vector2i_Equals(Vector2i a,Vector2i b);
