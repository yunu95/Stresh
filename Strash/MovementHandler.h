#pragma once
#include<Windows.h>
#include "Vector2d.h"
#include "Vector2i.h"


typedef struct _MovementHandler
{
    BOOL active;
    Vector2i* targetVector;
    Vector2i aVector;
    Vector2i bVector;
    double t;
    double duration;

    Vector2i(*expression)(struct _MovementHandler*);
    void(*onFinish)();
}
MovementHandler;

MovementHandler MovementHandler_instances[1000];
int MovementHandler_index;

void MovementHandler_Init();
void MovementHandler_Update();

Vector2i MovementHandler_Expression_EaseOut(struct _MovementHandler*);
Vector2i MovementHandler_Expression_EaseIn(struct _MovementHandler* instance);
Vector2i MovementHandler_Expression_EaseInOut(struct _MovementHandler* instance);
MovementHandler* MovementHandler_EaseInVector(Vector2i* target, Vector2i aVector, Vector2i bVector, double duration);
MovementHandler* MovementHandler_EaseOutVector(Vector2i* target, Vector2i aVector, Vector2i bVector, double duration);
MovementHandler* MovementHandler_EaseInOutVector(Vector2i* target, Vector2i aVector, Vector2i bVector, double duration);

MovementHandler* MovementHandler_EaseVectorWithFunc(Vector2i* target, Vector2i aVector, Vector2i bVector, double duration, Vector2i(*expression)(struct _MovementHandler*));
void MovementHandler_DelayedAction(void(*onFinish)(), double duration);
