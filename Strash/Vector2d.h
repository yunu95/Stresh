#pragma once
// 본 헤더에서는 2차원 벡터와 그에 관련된 함수를 정의합니다.
typedef struct
{
    double x, y;
}Vector2d;

struct Vector2i;
// 2차원 벡터를 선형보간합니다. t=0일때 aVector를, t=1일때 bVector를 반환합니다.
Vector2d Vector2d_Lerp(Vector2d aVector,Vector2d bVector,double t);
Vector2d GetVector2d(struct Vector2i vec2i);
