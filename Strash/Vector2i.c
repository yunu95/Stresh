#include "Vector2i.h"
#include "Vector2d.h"
const Vector2i Vector2i_Zero = { 0,0 };
const Vector2i Vector2i_one = { 1,1 };

Vector2i Vector2i_Lerp(Vector2i aVector, Vector2i bVector, double t)
{
    if (t >= 1)
        return bVector;
    else if (t <= 0)
        return aVector;

    aVector.x = aVector.x + (bVector.x - aVector.x) * t;
    aVector.y = aVector.y + (bVector.y - aVector.y) * t;
    return aVector;
}
Vector2i GetVector2i(Vector2d vec2d)
{
    Vector2i ret;
    ret.x = vec2d.x;
    ret.y = vec2d.y;
    return ret;
}
Vector2i GetVector2i_XY(double x, double y)
{
    Vector2i ret;
    ret.x = x;
    ret.y = y;
    return ret;
}
Vector2i Vector2i_Sum(Vector2i a, Vector2i b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}
BOOL Vector2i_Equals(Vector2i a, Vector2i b)
{
    return a.x == b.x && a.y == b.y;
}
