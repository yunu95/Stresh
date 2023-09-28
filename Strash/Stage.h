#pragma once
#include <Windows.h>
#include "StaticTile.h"
#include "DynamicGimmick.h"
#include "Vector2d.h"
#include "Vector2i.h"
#include "Fog.h"

LPCSTR defaultMapPath;
LPCSTR transitionImage;
const double Stage_transitionDuration;

// ��ȣ�ۿ��� ����
typedef enum
{
    Stage_InteractionType_NONE,
    Stage_InteractionType_MOVE,
    Stage_InteractionType_PUSH,
    Stage_InteractionType_PUSH_FUTILE
}Stage_InteractionType;

typedef enum
{
    Stage_State_NONE,
    Stage_State_PLAYING,
    Stage_State_LOADING,
    Stage_State_PAUSE,
}Stage_State;

// Stage�� ���� ��� Ÿ���� ���¿� ���ΰ� ĳ������ ��ġ, ��͵��� ����, ���� Ǯ���� ������ ��� �Ѹ����� �����մϴ�.
// Stage�� ���Ұ� å���� �繵 �����ϰ� ������ �� ������, ������Է��� �޾��� �� �����ų ������ �߷��ϱ� ���ؼ��� �̰��� �������� ��� �ʿ��մϴ�.
typedef struct _Stage
{
    Stage_State state;
    // ���� ���������� ���൵�� ���մϴ�. �� ���൵�� ����� �ذ��Ҷ����� 1�� �����ϸ�, ���� endProgress�� �������� ���������� ����˴ϴ�.
    int currentProgress;
    // �������� Ŭ������� �ʿ��� �������൵�� ���մϴ�.
    int endProgress;
    // ���ΰ��� ���� ��ġ�� ��Ÿ���ϴ�.
    Vector2i protagonistPosition;
    // Ÿ�ϵ��� ���¸� ��Ÿ���� ����Դϴ�.
    StaticTile tileMatrix[100][100];
    // ���������� �����ϴ� ��͵��� ��Ÿ���ϴ�.
    DynamicGimmick gimmicks[100][100];
    DynamicGimmick gimmicksTrespassable[100][100];
    // ���� ���������� �������� �ʰ� ������ �ִ� ��͵��� ��Ÿ���ϴ�.
    DynamicGimmick gimmicksHidden[100][100];

    DynamicGimmick_Type gimmicksDestination[100][100];
    // ���������� �����ϴ� �Ȱ����� ��Ÿ���ϴ�.
    Fog fogs[100][100];
    // Ÿ�� ��ġ���� ���� ����, ���� ���̸� ��Ÿ���ϴ�.
    int  width, height;
    int tileSize;
    // ���ϴ� �������� �ε��� ���Ǵ� Ʈ������ �̹���, ���¿� ���� ����.
    int transitionProgress;
    int bgmIndex;
    Vector2i transitionImagePosition;
    LPCWSTR transitionNextStage;
    WCHAR backgroundFileName[100];
    WCHAR backgroundOuterFileName[100];
    double eventElapsed;
    double eventDuration;
    void(*onStageClear)();
} Stage;

Stage stageInstance;

void Stage_Init();
// ���������� ������Ʈ �Լ��Դϴ�. ���������� �Ҽӵ� StaticTile Ȥ�� DynamicGimmick���� Update �Լ����� ��� ȣ���մϴ�.
void Stage_Update();
// ���������� �������ϸ�, ���������� �����ϰ� �ִ� Ÿ�ϰ� ���ΰ�, ��͵��� ��� �����մϴ�.
void Stage_Render(Vector2i position);

void Stage_ChangeStage(LPCWSTR* filename);
// �����̸��� �Ű������� �ָ� �ش� ������ �ص��Ͽ� ���������� �籸���մϴ�.
void Stage_PlayStageBGM();
void Stage_LoadStage(LPCWSTR filename);
void Stage_ReloadStage();
void Stage_ReloadStageFile();
// �ش� ������ �ص��Ͽ� Ÿ�Ϻ� ������ ���� Ÿ�� Ÿ�� �Ҵ�
void Stage_ProcessTileInfo(char** fileInfo, int row, int col);
BOOL Stage_isAccessible(Vector2i rowCol);
Stage_InteractionType Stage_GetInteractionType(Vector2i interactionTarget, Vector2i inputDirection);
Vector2i Stage_RowColToPosition(Vector2i rowCol);
void Stage_IncrementProgress();
void Stage_OnStageClearDefault();
