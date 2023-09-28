#pragma once
#include <Windows.h>
#include "Vector2d.h"
#include "Vector2i.h"
#include "MovementHandler.h"

// �ִٰ��� ������ٰ�, ���ٰ��� ���� �� �ִ� ������ ��� ��ü���Դϴ�.
// �� ��ü���� Update, Render �Լ����� Stage ��ü�� ���� ȣ��˴ϴ�.
typedef enum
{
    DynamicGimmick_Type_NONE,
    DynamicGimmick_Type_PIC_HEART,
    DynamicGimmick_Type_PIC_HEART_DST,
    DynamicGimmick_Type_PHONEBOOK,
    DynamicGimmick_Type_PHONE,
    DynamicGimmick_Type_REMOCON,
    DynamicGimmick_Type_TV,
    DynamicGimmick_Type_FLOWER,
    DynamicGimmick_Type_PENCIL,
    DynamicGimmick_Type_TEXTBOOK,
    DynamicGimmick_Type_PIC_FLOWER,
    DynamicGimmick_Type_PIC_FLOWER_DST,
    DynamicGimmick_Type_PIC_BEAR,
    DynamicGimmick_Type_PIC_BEAR_DST,
    DynamicGimmick_Type_TRASH,
    DynamicGimmick_Type_STAR,
    DynamicGimmick_Type_CROSS,
    DynamicGimmick_Type_PIC_TRASH,
    DynamicGimmick_Type_PIC_MOM,
    DynamicGimmick_Type_PIC_CROSS,
    DynamicGimmick_Type_PIC_TRASH_DST,
    DynamicGimmick_Type_PIC_MOM_DST,
    DynamicGimmick_Type_PIC_CROSS_DST,
    DynamicGimmick_Type_PIC_NOTE,
    DynamicGimmick_Type_PIC_STRESH,
    DynamicGimmick_Type_PIC_FAMILY,
    DynamicGimmick_Type_PIC_NOTE_DST,
    DynamicGimmick_Type_PIC_STRESH_DST,
    DynamicGimmick_Type_PIC_FAMILY_DST,
    DynamicGimmick_Type_BRIGHTBOY,
    DynamicGimmick_Type_HEART,
    DynamicGimmick_Type_ALBUM,
    DynamicGimmick_Type_ESCAPE_BAD,
    DynamicGimmick_Type_ESCAPE_GOOD,
    DynamicGimmick_Type_Key_W,
    DynamicGimmick_Type_Key_A,
    DynamicGimmick_Type_Key_S,
    DynamicGimmick_Type_Key_D,
    DynamicGimmick_Type_Key_R,
    DynamicGimmick_Type_Key_ESC,
}DynamicGimmick_Type;

typedef enum
{
    DynamicGimmick_State_IDLE,
    DynamicGimmick_State_MOVING,
    DynamicGimmick_State_Popping_1,
    DynamicGimmick_State_Popping_2,
    DynamicGimmick_State_Popping_in_1,
    DynamicGimmick_State_Popping_in_2,
}DynamicGimmick_State;

typedef struct _DynamicGimmick
{
    // ����� �̹����� ���ϸ�.
    WCHAR imgFileName[100];
    // ��� ������ �� �ֽ��ϱ�? TRUE�� ��ȣ�ۿ��� �Ұ�����.
    BOOL isTrespassable;

    // Ȱ��ȭ�Ǿ� �ֽ��ϱ�? FALSE�� ��ȣ�ۿ��� �Ұ����ϸ�, �׷��̽����� ó���� ��.
    BOOL isActive;
    BOOL showLock;
    BOOL isFloating;

    // �ؼҵǾ��� �� ���������� ������ ������Ʈ�մϱ�?
    BOOL incrementProgressOnQuenched;

    DynamicGimmick_Type gimmickType;
    DynamicGimmick_Type quenchTarget;
    DynamicGimmick_Type quenchEnableTarget[10];
    DynamicGimmick_State gimmickState;

    MovementHandler* poppingHandler;
    MovementHandler* floatingHandler;
    Vector2i poppingSmokeSize;

    Vector2d moving_src;
    Vector2d moving_dst;
    Vector2i moving_current;
    Vector2i floating_deltaVector;
    double moving_elapsed;
    double moving_duration;
    int activeProgress[100];
    void (*onTrigger)(struct _DynamicGimmick* instance, Vector2i position, Vector2i triggeringDirection);
    void (*onQuenched)(struct _DynamicGimmick* instance, Vector2i position);
}
DynamicGimmick;

LPCWSTR lockImage;
DynamicGimmick DynamicGimmick_NONE;
Vector2i floatingDeltaVectorA;
Vector2i floatingDeltaVectorB;

void DynamicGimmick_Init();
void DynamicGimmick_Update(DynamicGimmick* instance);
void DynamicGimmick_MoveTo(DynamicGimmick* instance, Vector2i src, Vector2i dst);
void DynamicGimmick_Render(DynamicGimmick* instance, Vector2i position, Vector2i drawRect);

void DynamicGimmick_Pushed1Block(DynamicGimmick* instance, Vector2i position, Vector2i triggeringDirection);
void DynamicGimmick_FinishStage(DynamicGimmick* instance, Vector2i position, Vector2i triggeringDirection);
void DynamicGimmick_DeactivateOnQuenched(DynamicGimmick* instance, Vector2i position);
void DynamicGimmick_DisappearOnQuenched(DynamicGimmick* instance, Vector2i position);
void DynamicGimmick_PopIn(DynamicGimmick* instance);
DynamicGimmick DynamicGimmick_EmptyGimmick();
DynamicGimmick DynamicGimmick_GetGimmickPreset(DynamicGimmick_Type gimmckType);
