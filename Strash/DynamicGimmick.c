#include "DynamicGimmick.h"
#include "GDIEngine.h"
#include "Stage.h"
#include "Time.h"
#include "Sound.h"
#include "MovementHandler.h"
#include "Cutscene.h"
#include <Windows.h>

LPCWSTR lockImage = L"Locking tile1.bmp";
Vector2i floatingDeltaVectorA = { 0,0 };
Vector2i floatingDeltaVectorB = { 0,-20 };
void DynamicGimmick_Init()
{
    DynamicGimmick_NONE.gimmickType = DynamicGimmick_Type_NONE;
}
void DynamicGimmick_Update(DynamicGimmick* instance)
{
    if (instance->gimmickType == DynamicGimmick_Type_NONE)
        return;

    Vector2i A, B;

    if (!instance->isActive)
        instance->floating_deltaVector = Vector2i_Zero;
    if (instance->floatingHandler)
        instance->floatingHandler->targetVector = &instance->floating_deltaVector;

    if (instance->isFloating && instance->isActive)
        if (!instance->floatingHandler || !instance->floatingHandler->active)
        {
            A = floatingDeltaVectorA;
            B = floatingDeltaVectorB;
            if (instance->floatingHandler)
            {
                A = (Vector2i_Equals(instance->floatingHandler->bVector, floatingDeltaVectorA) ? floatingDeltaVectorA : floatingDeltaVectorB);
                B = (Vector2i_Equals(instance->floatingHandler->bVector, floatingDeltaVectorA) ? floatingDeltaVectorB : floatingDeltaVectorA);
            }
            instance->floatingHandler = MovementHandler_EaseInOutVector(&instance->floating_deltaVector, A, B, 1);
            //instance->floatingHandler = MovementHandler_EaseInVector(&instance->floating_deltaVector, A, B, 1);
        }

    switch (instance->gimmickState)
    {
    case DynamicGimmick_State_IDLE:
        break;
    case DynamicGimmick_State_MOVING:
        instance->moving_elapsed += Time_Instance.deltaTime;
        double t = instance->moving_elapsed / instance->moving_duration;
        t = 1 - (1 - t) * (1 - t);
        Vector2d lerp = Vector2d_Lerp(instance->moving_src, instance->moving_dst, t);
        instance->moving_current.x = lerp.x;
        instance->moving_current.y = lerp.y;
        if (instance->moving_elapsed > instance->moving_duration)
            instance->gimmickState = DynamicGimmick_State_IDLE;
        break;
    case DynamicGimmick_State_Popping_1:
        if (!instance->poppingHandler->active)
        {
            instance->poppingHandler = MovementHandler_EaseInVector(&instance->poppingSmokeSize, GetVector2i_XY(stageInstance.tileSize * 1.1, stageInstance.tileSize * 1.1), Vector2i_one, 0.35);
            instance->gimmickState = DynamicGimmick_State_Popping_2;
        }
        break;
    case DynamicGimmick_State_Popping_2:
        if (!instance->poppingHandler->active)
        {
            instance->gimmickType = DynamicGimmick_Type_NONE;
        }
        break;
    case DynamicGimmick_State_Popping_in_1:
        if (!instance->poppingHandler->active)
        {
            instance->poppingHandler = MovementHandler_EaseInVector(&instance->poppingSmokeSize, GetVector2i_XY(stageInstance.tileSize * 1.1, stageInstance.tileSize * 1.1), Vector2i_one, 0.35);
            instance->gimmickState = DynamicGimmick_State_Popping_in_2;
        }
        break;
    case DynamicGimmick_State_Popping_in_2:
        if (!instance->poppingHandler->active)
        {
            instance->gimmickState = DynamicGimmick_State_IDLE;
        }
        break;
    default:
        break;
    }
}
void DynamicGimmick_Render(DynamicGimmick* instance, Vector2i position, Vector2i drawRect)
{

    position = Vector2i_Sum(position, instance->floating_deltaVector);
    if (instance->gimmickType == DynamicGimmick_Type_NONE)
        return;
    if (instance->imgFileName[0] == 0)
        return;

    switch (instance->gimmickState)
    {
    case DynamicGimmick_State_IDLE:
        GDI_DrawResizedBitmap(instance->imgFileName, position, drawRect);
        if (!instance->isActive && instance->showLock)
            GDI_DrawResizedBitmap(lockImage, position, drawRect);
        break;
    case DynamicGimmick_State_MOVING:
        position = Vector2i_Sum(instance->moving_current, instance->floating_deltaVector);
        GDI_DrawResizedBitmap(instance->imgFileName, position, drawRect);
        break;
    case DynamicGimmick_State_Popping_1:
        GDI_DrawResizedBitmap(instance->imgFileName, position, drawRect);
        GDI_DrawResizedBitmap(L"UI\\\\boom 3.bmp", position, instance->poppingSmokeSize);
        break;
    case DynamicGimmick_State_Popping_2:
        GDI_DrawResizedBitmap(L"UI\\\\boom 3.bmp", position, instance->poppingSmokeSize);
        break;
    case DynamicGimmick_State_Popping_in_1:
        GDI_DrawResizedBitmap(instance->imgFileName, position, drawRect);
        GDI_DrawResizedBitmap(lockImage, position, drawRect);
        GDI_DrawResizedBitmap(L"UI\\\\boom 3.bmp", position, instance->poppingSmokeSize);
        break;
    case DynamicGimmick_State_Popping_in_2:
        GDI_DrawResizedBitmap(instance->imgFileName, position, drawRect);
        GDI_DrawResizedBitmap(L"UI\\\\boom 3.bmp", position, instance->poppingSmokeSize);
        break;
    }
}
void DynamicGimmick_MoveTo(DynamicGimmick* instance, Vector2i src, Vector2i dst)
{
    instance->moving_src.x = src.x;
    instance->moving_src.y = src.y;
    instance->moving_dst.x = dst.x;
    instance->moving_dst.y = dst.y;
    instance->moving_current.x = src.x;
    instance->moving_current.y = src.y;
    instance->moving_elapsed = 0;
    instance->moving_duration = 0.3;
    instance->gimmickState = DynamicGimmick_State_MOVING;
}
void DynamicGimmick_Pushed1Block(DynamicGimmick* instance, Vector2i position, Vector2i triggeringDirection)
{
    Vector2i dst = position;
    dst.x += triggeringDirection.x;
    dst.y += triggeringDirection.y;
    // 다이나믹 기믹이 합쳐지는 로직은 모두 여기서 처리합니다!
    // 먼저, 기믹이 해소되는 경우인가? 그런 경우는 따로 처리해야됨.
    if (instance->quenchTarget == stageInstance.gimmicks[dst.y][dst.x].gimmickType
        || instance->quenchTarget == stageInstance.gimmicksTrespassable[dst.y][dst.x].gimmickType)
    {
        if (instance->onQuenched)
            instance->onQuenched(instance, position);

        Stage_EnableGimmick(*instance);
        if (stageInstance.gimmicks[dst.y][dst.x].onQuenched && instance->quenchTarget == stageInstance.gimmicks[dst.y][dst.x].gimmickType)
            stageInstance.gimmicks[dst.y][dst.x].onQuenched(&stageInstance.gimmicks[dst.y][dst.x], position);


        SoundPlay(SoundType_Effect_MERGE);

        if (instance->quenchTarget == stageInstance.gimmicksTrespassable[dst.y][dst.x].gimmickType
            && stageInstance.gimmicksTrespassable[dst.y][dst.x].incrementProgressOnQuenched)
            Stage_IncrementProgress();

        if (instance->quenchTarget == stageInstance.gimmicksTrespassable[dst.y][dst.x].gimmickType
            && stageInstance.gimmicksTrespassable[dst.y][dst.x].onQuenched)
            stageInstance.gimmicksTrespassable[dst.y][dst.x].onQuenched(&stageInstance.gimmicksTrespassable[dst.y][dst.x], position);
    }

    if (!Stage_isAccessible(dst))
        return;

    stageInstance.gimmicks[dst.y][dst.x] = stageInstance.gimmicks[position.y][position.x];
    stageInstance.gimmicks[position.y][position.x] = DynamicGimmick_NONE;
    DynamicGimmick_MoveTo(&stageInstance.gimmicks[dst.y][dst.x], Stage_RowColToPosition(position), Stage_RowColToPosition(dst));


}
void DynamicGimmick_FinishStage(DynamicGimmick* instance, Vector2i position, Vector2i triggeringDirection)
{
    MovementHandler_DelayedAction(Stage_OnStageClearDefault, 0.5);
}
void DynamicGimmick_FinishStageBadEnding(DynamicGimmick* instance, Vector2i position, Vector2i triggeringDirection)
{
    g_isSelectEnding = TRUE;
    g_isHappyEnd = FALSE;
    MovementHandler_DelayedAction(Stage_OnStageClearDefault, 0.5);
}
void DynamicGimmick_FinishStageGoodEnding(DynamicGimmick* instance, Vector2i position, Vector2i triggeringDirection)
{
    g_isSelectEnding = TRUE;
    g_isHappyEnd = TRUE;
    MovementHandler_DelayedAction(Stage_OnStageClearDefault, 0.5);
}
DynamicGimmick DynamicGimmick_EmptyGimmick()
{
    DynamicGimmick ret;
    memset(&ret, 0, sizeof(DynamicGimmick));
    return ret;
}
void DynamicGimmick_DisappearOnQuenched(DynamicGimmick* instance, Vector2i position)
{
    instance->gimmickState = DynamicGimmick_State_Popping_1;
    instance->poppingHandler = MovementHandler_EaseOutVector(&instance->poppingSmokeSize, Vector2i_one, GetVector2i_XY(stageInstance.tileSize * 1.1, stageInstance.tileSize * 1.1), 0.2);
}
void DynamicGimmick_PopIn(DynamicGimmick* instance)
{
    instance->showLock = FALSE;
    instance->gimmickState = DynamicGimmick_State_Popping_in_1;
    instance->poppingHandler = MovementHandler_EaseOutVector(&instance->poppingSmokeSize, Vector2i_one, GetVector2i_XY(stageInstance.tileSize * 1.1, stageInstance.tileSize * 1.1), 0.2);
}
// 시리얼라이제이션이 안되서 프리펩은 이렇게 저장해야됨.
DynamicGimmick DynamicGimmick_GetGimmickPreset(DynamicGimmick_Type gimmickType)
{
    DynamicGimmick ret = DynamicGimmick_EmptyGimmick();
    ret.gimmickType = gimmickType;
    ret.gimmickState = DynamicGimmick_State_IDLE;
    ret.onTrigger = DynamicGimmick_Pushed1Block;
    ret.imgFileName[0] = 0;
    ret.isFloating = TRUE;


    // 기믹에 맞는 이미지 정해주기.
    switch (gimmickType)
    {
    case DynamicGimmick_Type_CROSS:
        lstrcpyW(ret.imgFileName, L"Object\\\\object__cross_128.bmp");
        break;
    case DynamicGimmick_Type_PIC_CROSS:
        lstrcpyW(ret.imgFileName, L"Object\\\\tile_stage2-2_128.bmp");
        break;
    case DynamicGimmick_Type_PHONE:
        lstrcpyW(ret.imgFileName, L"Object\\\\object_telephone_128.bmp");
        break;
    case DynamicGimmick_Type_PHONEBOOK:
        lstrcpyW(ret.imgFileName, L"Object\\\\object_telephone_directory_128.bmp");
        break;
    case DynamicGimmick_Type_REMOCON:
        lstrcpyW(ret.imgFileName, L"Object\\\\object_remote_control_128.bmp");
        break;
    case DynamicGimmick_Type_TV:
        lstrcpyW(ret.imgFileName, L"Object\\\\object_TV_128.bmp");
        break;
    case DynamicGimmick_Type_ALBUM:
        lstrcpyW(ret.imgFileName, L"Object\\\\object_album_128.bmp");
        break;
    case DynamicGimmick_Type_FLOWER:
        lstrcpyW(ret.imgFileName, L"Object\\\\object_dandelion_seeds_128.bmp");
        break;
    case DynamicGimmick_Type_PENCIL:
        lstrcpyW(ret.imgFileName, L"Object\\\\object_pencil_128.bmp");
        break;
    case DynamicGimmick_Type_PIC_HEART:
        lstrcpyW(ret.imgFileName, L"Object\\\\object_heart_pic_128.bmp");
        break;
    case DynamicGimmick_Type_TEXTBOOK:
        lstrcpyW(ret.imgFileName, L"Object\\\\object_textbook_128.bmp");
        break;
    case DynamicGimmick_Type_PIC_FLOWER:
        lstrcpyW(ret.imgFileName, L"Object\\\\tile_stage1-1_128.bmp");
        break;
    case DynamicGimmick_Type_PIC_BEAR:
        lstrcpyW(ret.imgFileName, L"Object\\\\tile_stage1-2_128.bmp");
        break;
    case DynamicGimmick_Type_TRASH:
        lstrcpyW(ret.imgFileName, L"Object\\\\object_garbage_bag_128.bmp");
        break;
    case DynamicGimmick_Type_STAR:
        lstrcpyW(ret.imgFileName, L"Object\\\\object_star_128.bmp");
        break;
    case DynamicGimmick_Type_PIC_TRASH:
        lstrcpyW(ret.imgFileName, L"Object\\\\tile_stage2-3_128.bmp");
        break;
    case DynamicGimmick_Type_PIC_MOM:
        lstrcpyW(ret.imgFileName, L"Object\\\\tile_stage2-1_128.bmp");
        break;
    case DynamicGimmick_Type_PIC_NOTE:
        lstrcpyW(ret.imgFileName, L"Object\\tile_stage3-1_128.bmp");
        break;
    case DynamicGimmick_Type_PIC_STRESH:
        lstrcpyW(ret.imgFileName, L"Object\\tile_stage3-3_128.bmp");
        break;
    case DynamicGimmick_Type_PIC_FAMILY:
        lstrcpyW(ret.imgFileName, L"Object\\tile_stage3-2_128.bmp");
        break;
    case DynamicGimmick_Type_BRIGHTBOY:
        lstrcpyW(ret.imgFileName, L"Object\\세트리쉬.bmp");
        break;
    case DynamicGimmick_Type_HEART:
        lstrcpyW(ret.imgFileName, L"Object\\object_heart_128.bmp");
        break;
    case DynamicGimmick_Type_Key_W:
        lstrcpyW(ret.imgFileName, L"Object\\w.bmp");
        break;
    case DynamicGimmick_Type_Key_A:
        lstrcpyW(ret.imgFileName, L"Object\\a.bmp");
        break;
    case DynamicGimmick_Type_Key_S:
        lstrcpyW(ret.imgFileName, L"Object\\s.bmp");
        break;
    case DynamicGimmick_Type_Key_D:
        lstrcpyW(ret.imgFileName, L"Object\\d.bmp");
        break;
    case DynamicGimmick_Type_Key_R:
        lstrcpyW(ret.imgFileName, L"Object\\r.bmp");
        break;
    case DynamicGimmick_Type_Key_ESC:
        lstrcpyW(ret.imgFileName, L"Object\\ESC.bmp");
        break;
    case DynamicGimmick_Type_PIC_HEART_DST:
    case DynamicGimmick_Type_PIC_NOTE_DST:
    case DynamicGimmick_Type_PIC_STRESH_DST:
    case DynamicGimmick_Type_PIC_FAMILY_DST:
    case DynamicGimmick_Type_PIC_TRASH_DST:
    case DynamicGimmick_Type_PIC_MOM_DST:
    case DynamicGimmick_Type_PIC_CROSS_DST:
    case DynamicGimmick_Type_PIC_BEAR_DST:
    case DynamicGimmick_Type_PIC_FLOWER_DST:
        lstrcpyW(ret.imgFileName, L"UI\\\\tile_3.bmp");
        break;
    default:
        break;
    }
    // 기믹을 밟을 수 있는가?
    switch (gimmickType)
    {
    case DynamicGimmick_Type_ESCAPE_GOOD:
    case DynamicGimmick_Type_ESCAPE_BAD:
    case DynamicGimmick_Type_ALBUM:
    case DynamicGimmick_Type_PIC_BEAR_DST:
    case DynamicGimmick_Type_PIC_CROSS_DST:
    case DynamicGimmick_Type_PIC_FLOWER_DST:
    case DynamicGimmick_Type_PIC_MOM_DST:
    case DynamicGimmick_Type_PIC_TRASH_DST:
    case DynamicGimmick_Type_PIC_NOTE_DST:
    case DynamicGimmick_Type_PIC_FAMILY_DST:
    case DynamicGimmick_Type_PIC_STRESH_DST:
    case DynamicGimmick_Type_PIC_HEART_DST:
        ret.isTrespassable = TRUE;
        ret.onTrigger = NULL;
        break;
    default:
        ret.isTrespassable = FALSE;
        break;
    }
    // 상호작용시 적용될 특별한 동작이 있는가?
    switch (gimmickType)
    {
    case DynamicGimmick_Type_ESCAPE_GOOD:
        ret.onTrigger = DynamicGimmick_FinishStageGoodEnding;
        break;
    case DynamicGimmick_Type_ESCAPE_BAD:
        ret.onTrigger = DynamicGimmick_FinishStageBadEnding;
        break;
    case DynamicGimmick_Type_ALBUM:
        ret.onTrigger = DynamicGimmick_FinishStage;
        break;
    default:
        break;
    }

    // 기믹이 처음부터 활성화되어 있는가?
    switch (gimmickType)
    {
    case DynamicGimmick_Type_PIC_BEAR:
    case DynamicGimmick_Type_PIC_CROSS:
    case DynamicGimmick_Type_PIC_FLOWER:
    case DynamicGimmick_Type_PIC_MOM:
    case DynamicGimmick_Type_PIC_TRASH:
    case DynamicGimmick_Type_PIC_NOTE:
    case DynamicGimmick_Type_PIC_FAMILY:
    case DynamicGimmick_Type_PIC_STRESH:
    case DynamicGimmick_Type_PIC_HEART:
    case DynamicGimmick_Type_TV:
    case DynamicGimmick_Type_REMOCON:
        ret.isActive = FALSE;
        ret.showLock = TRUE;
        break;
    default:
        ret.isActive = TRUE;
        break;
    }

    // 기믹이 어느 오브젝트와 부딪혀야 해소되는가?
    switch (gimmickType)
    {
    case DynamicGimmick_Type_PIC_BEAR:
        ret.quenchTarget = DynamicGimmick_Type_PIC_BEAR_DST;
        break;
    case DynamicGimmick_Type_PIC_CROSS:
        ret.quenchTarget = DynamicGimmick_Type_PIC_CROSS_DST;
        break;
    case DynamicGimmick_Type_PIC_FLOWER:
        ret.quenchTarget = DynamicGimmick_Type_PIC_FLOWER_DST;
        break;
    case DynamicGimmick_Type_PIC_MOM:
        ret.quenchTarget = DynamicGimmick_Type_PIC_MOM_DST;
        break;
    case DynamicGimmick_Type_PIC_TRASH:
        ret.quenchTarget = DynamicGimmick_Type_PIC_TRASH_DST;
        break;
    case DynamicGimmick_Type_PIC_NOTE:
        ret.quenchTarget = DynamicGimmick_Type_PIC_NOTE_DST;
        break;
    case DynamicGimmick_Type_PIC_STRESH:
        ret.quenchTarget = DynamicGimmick_Type_PIC_STRESH_DST;
        break;
    case DynamicGimmick_Type_PIC_FAMILY:
        ret.quenchTarget = DynamicGimmick_Type_PIC_FAMILY_DST;
        break;
    case DynamicGimmick_Type_PHONEBOOK:
        ret.quenchTarget = DynamicGimmick_Type_PHONE;
        break;
    case DynamicGimmick_Type_REMOCON:
        ret.quenchTarget = DynamicGimmick_Type_TV;
        break;
    case DynamicGimmick_Type_PENCIL:
        ret.quenchTarget = DynamicGimmick_Type_TEXTBOOK;
        break;
    case DynamicGimmick_Type_TEXTBOOK:
        ret.quenchTarget = DynamicGimmick_Type_PENCIL;
        break;
    case DynamicGimmick_Type_PIC_HEART:
        ret.quenchTarget = DynamicGimmick_Type_PIC_HEART_DST;
        break;
    default:
        ret.quenchTarget = ret.gimmickType;
    }

    // 기믹이 해소되면 해당 기믹은 스스로 어떤 동작을 하는가?
    switch (gimmickType)
    {
    case DynamicGimmick_Type_PIC_BEAR:
    case DynamicGimmick_Type_PIC_CROSS:
    case DynamicGimmick_Type_PIC_FLOWER:
    case DynamicGimmick_Type_PIC_MOM:
    case DynamicGimmick_Type_PIC_TRASH:
    case DynamicGimmick_Type_PIC_NOTE:
    case DynamicGimmick_Type_PIC_STRESH:
    case DynamicGimmick_Type_PIC_FAMILY:
    case DynamicGimmick_Type_PIC_HEART:
        ret.onQuenched = DynamicGimmick_DeactivateOnQuenched;
        break;
    case DynamicGimmick_Type_PIC_BEAR_DST:
    case DynamicGimmick_Type_PIC_CROSS_DST:
    case DynamicGimmick_Type_PIC_FLOWER_DST:
    case DynamicGimmick_Type_PIC_MOM_DST:
    case DynamicGimmick_Type_PIC_TRASH_DST:
    case DynamicGimmick_Type_PIC_NOTE_DST:
    case DynamicGimmick_Type_PIC_STRESH_DST:
    case DynamicGimmick_Type_PIC_HEART_DST:
    case DynamicGimmick_Type_PIC_FAMILY_DST:
        ret.onQuenched = NULL;
        ret.incrementProgressOnQuenched = TRUE;
        break;
    default:
        ret.onQuenched = DynamicGimmick_DisappearOnQuenched;
        ret.incrementProgressOnQuenched = FALSE;
        break;
    }

    // 기믹이 해소되면 어떤 기믹을 해금하는가? 
    switch (gimmickType)
    {
    case DynamicGimmick_Type_PHONEBOOK:
        ret.quenchEnableTarget[0] = DynamicGimmick_Type_TV;
        ret.quenchEnableTarget[1] = DynamicGimmick_Type_REMOCON;
        break;
    case DynamicGimmick_Type_CROSS:
        ret.quenchEnableTarget[0] = DynamicGimmick_Type_PIC_CROSS;
        ret.quenchEnableTarget[1] = DynamicGimmick_Type_PIC_CROSS_DST;
        break;
    case DynamicGimmick_Type_FLOWER:
        ret.quenchEnableTarget[0] = DynamicGimmick_Type_PIC_FLOWER;
        ret.quenchEnableTarget[1] = DynamicGimmick_Type_PIC_FLOWER_DST;
        break;
    case DynamicGimmick_Type_PENCIL:
    case DynamicGimmick_Type_TEXTBOOK:
        ret.quenchEnableTarget[0] = DynamicGimmick_Type_PIC_BEAR;
        ret.quenchEnableTarget[1] = DynamicGimmick_Type_PIC_BEAR_DST;
        ret.quenchEnableTarget[1] = DynamicGimmick_Type_PIC_HEART;
        break;
    case DynamicGimmick_Type_STAR:
        ret.quenchEnableTarget[0] = DynamicGimmick_Type_PIC_MOM;
        break;
    case DynamicGimmick_Type_TRASH:
        ret.quenchEnableTarget[0] = DynamicGimmick_Type_PIC_TRASH;
        ret.quenchEnableTarget[1] = DynamicGimmick_Type_PIC_STRESH;
        break;
    case DynamicGimmick_Type_BRIGHTBOY:
        ret.quenchEnableTarget[0] = DynamicGimmick_Type_PIC_FAMILY;
        break;
    case DynamicGimmick_Type_HEART:
        ret.quenchEnableTarget[0] = DynamicGimmick_Type_PIC_NOTE;
        break;
    default:
        //ret.onQuenched = DynamicGimmick_DisappearOnQuenched;
        break;
    }

    return ret;
}
void DynamicGimmick_DeactivateOnQuenched(DynamicGimmick* instance, Vector2i position)
{
    instance->isActive = FALSE;
}
