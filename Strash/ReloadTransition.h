#include "MovementHandler.h"
#include "Vector2i.h"

typedef enum _ReloadTransitionState
{
    ReloadTransitionState_IDLE,
    ReloadTransitionState_EMERGE,
    ReloadTransitionState_STAY,
    ReloadTransitionState_DISAPPEAR,
}
ReloadTransitionState;

typedef struct _ReloadTransition
{
    ReloadTransitionState state;
    MovementHandler* movementHandler;
    Vector2i position;
    WCHAR image[100];
    void(*onOverlap)();
    void(*onOverlap2)();
}
ReloadTransition;

ReloadTransition ReloadTransition_Instance;

void ReloadTransition_Init();
void ReloadTransition_Update();
void ReloadTransition_Render();
void ReloadTransition_Emerge();
void ReloadTransition_Disappear();
