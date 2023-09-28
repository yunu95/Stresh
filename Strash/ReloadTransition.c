#include "ReloadTransition.h"
#include "GDIEngine.h"

Vector2i ReloadTransition_emergePoint;
void ReloadTransition_Init()
{
    ReloadTransition_emergePoint = GDIEngine_center;
    ReloadTransition_emergePoint.y -= GDIEngine_ResolutionH * 1.2;
    memset(&ReloadTransition_Instance, 0, sizeof(ReloadTransition));
}
void ReloadTransition_Update()
{
    switch (ReloadTransition_Instance.state)
    {
    case ReloadTransitionState_EMERGE:
        if (!ReloadTransition_Instance.movementHandler->active)
        {
            ReloadTransition_Instance.state = ReloadTransitionState_STAY;
            MovementHandler_DelayedAction(ReloadTransition_Disappear, 1.5);
        }
        break;
    case ReloadTransitionState_DISAPPEAR:
        if (!ReloadTransition_Instance.movementHandler->active)
            ReloadTransition_Instance.state = ReloadTransitionState_IDLE;
        break;
    default:
        break;
    }
}
void ReloadTransition_Render()
{
    if (ReloadTransition_Instance.state == ReloadTransitionState_IDLE)
        return;

    GDI_DrawBitmap(ReloadTransition_Instance.image, ReloadTransition_Instance.position);
}

void ReloadTransition_Emerge()
{
    ReloadTransition_Instance.state = ReloadTransitionState_EMERGE;
    ReloadTransition_Instance.movementHandler = MovementHandler_EaseOutVector(&ReloadTransition_Instance.position, ReloadTransition_emergePoint, GDIEngine_center, 0.3);
}
void ReloadTransition_Disappear()
{
    if (ReloadTransition_Instance.onOverlap)
        ReloadTransition_Instance.onOverlap();
    if (ReloadTransition_Instance.onOverlap2)
        ReloadTransition_Instance.onOverlap2();
    ReloadTransition_Instance.state = ReloadTransitionState_DISAPPEAR;
    ReloadTransition_Instance.movementHandler = MovementHandler_EaseOutVector(&ReloadTransition_Instance.position, GDIEngine_center, ReloadTransition_emergePoint, 0.3);
}
