#include "Vector2d.h"
#include "Vector2i.h"

Vector2d Vector2d_Lerp(Vector2d aVector, Vector2d bVector, double t)
{
    aVector.x = aVector.x + (bVector.x - aVector.x) * t;
    aVector.y = aVector.y + (bVector.y - aVector.y) * t;
    return aVector;
}
Vector2d GetVector2d(Vector2i vec2i)
{
    Vector2d ret;
    ret.x = vec2i.x;
    ret.y = vec2i.y;
    return ret;
}
