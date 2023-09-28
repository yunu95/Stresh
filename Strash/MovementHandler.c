#include "MovementHandler.h"
#include "Time.h"

void MovementHandler_Init()
{
    for (int i = 0; i < 1000; i++)
        MovementHandler_instances[i].active = FALSE;
    MovementHandler_index = 0;
}
void MovementHandler_Update()
{
    for (int i = 0; i < 1000; i++)
        if (MovementHandler_instances[i].active)
        {
            MovementHandler_instances[i].t += Time_Instance.deltaTime;
            if (MovementHandler_instances[i].expression)
                *MovementHandler_instances[i].targetVector = MovementHandler_instances[i].expression(&MovementHandler_instances[i]);

            if (MovementHandler_instances[i].t > MovementHandler_instances[i].duration)
            {
                MovementHandler_instances[i].active = FALSE;
                if (MovementHandler_instances[i].onFinish)
                    MovementHandler_instances[i].onFinish();
            }
        }
}

Vector2i MovementHandler_Expression_EaseOut(struct _MovementHandler* instance)
{
    double normT = instance->t / instance->duration;
    return Vector2i_Lerp(instance->aVector, instance->bVector, 1 - (1 - normT) * (1 - normT));
}
Vector2i MovementHandler_Expression_EaseIn(struct _MovementHandler* instance)
{
    double normT = instance->t / instance->duration;
    return Vector2i_Lerp(instance->aVector, instance->bVector, normT * normT);
}
Vector2i MovementHandler_Expression_EaseInOut(struct _MovementHandler* instance)
{
    double normT = instance->t / instance->duration;
    return Vector2i_Lerp(instance->aVector, instance->bVector, -normT * normT * normT + normT * normT + normT);
}

MovementHandler* MovementHandler_FindIdleOne()
{
    for (int i = 0; i < 1000; i++)
    {
        MovementHandler_index = ++MovementHandler_index % 1000;
        if (!MovementHandler_instances[MovementHandler_index].active)
        {
            memset(&MovementHandler_instances[MovementHandler_index], 0, sizeof(MovementHandler));
            return &MovementHandler_instances[MovementHandler_index];
        }
    }

    return NULL;
}
MovementHandler* MovementHandler_EaseOutVector(Vector2i* target, Vector2i aVector, Vector2i bVector, double duration)
{
    return MovementHandler_EaseVectorWithFunc(target, aVector, bVector, duration, MovementHandler_Expression_EaseOut);
}
MovementHandler* MovementHandler_EaseInVector(Vector2i* target, Vector2i aVector, Vector2i bVector, double duration)
{
    return MovementHandler_EaseVectorWithFunc(target, aVector, bVector, duration, MovementHandler_Expression_EaseIn);
}
MovementHandler* MovementHandler_EaseInOutVector(Vector2i* target, Vector2i aVector, Vector2i bVector, double duration)
{
    return MovementHandler_EaseVectorWithFunc(target, aVector, bVector, duration, MovementHandler_Expression_EaseInOut);
}
MovementHandler* MovementHandler_EaseVectorWithFunc(Vector2i* target, Vector2i aVector, Vector2i bVector, double duration, Vector2i(*expression)(struct _MovementHandler*))
{
    MovementHandler* handler = MovementHandler_FindIdleOne();
    if (!handler)
        return NULL;
    *target = aVector;
    handler->targetVector = target;
    handler->active = TRUE;
    handler->aVector = aVector;
    handler->bVector = bVector;
    handler->expression = expression;
    handler->t = 0;
    handler->duration = duration;
    return handler;
}
void MovementHandler_DelayedAction(void(*onFinish)(), double duration)
{
    MovementHandler* handler = MovementHandler_FindIdleOne();

    handler->t = 0;
    handler->active = TRUE;
    handler->duration = duration;
    handler->onFinish = onFinish;
}
