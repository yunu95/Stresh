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

// 상호작용의 종류
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

// Stage는 현재 모든 타일의 상태와 주인공 캐릭터의 위치, 기믹들의 상태, 퍼즐 풀이의 진도를 모두 총망라해 저장합니다.
// Stage의 역할과 책임이 사뭇 과중하게 느껴질 수 있지만, 사용자입력을 받았을 때 실행시킬 동작을 추론하기 위해서는 이같은 정보들이 모두 필요합니다.
typedef struct _Stage
{
    Stage_State state;
    // 현재 스테이지의 진행도를 뜻합니다. 이 진행도는 기믹을 해결할때마다 1씩 증가하며, 값이 endProgress와 같아지면 스테이지가 종료됩니다.
    int currentProgress;
    // 스테이지 클리어까지 필요한 누적진행도를 뜻합니다.
    int endProgress;
    // 주인공의 현재 위치를 나타냅니다.
    Vector2i protagonistPosition;
    // 타일들의 상태를 나타내는 행렬입니다.
    StaticTile tileMatrix[100][100];
    // 스테이지에 존재하는 기믹들을 나타냅니다.
    DynamicGimmick gimmicks[100][100];
    DynamicGimmick gimmicksTrespassable[100][100];
    // 아직 스테이지에 존재하지 않고 숨겨져 있는 기믹들을 나타냅니다.
    DynamicGimmick gimmicksHidden[100][100];

    DynamicGimmick_Type gimmicksDestination[100][100];
    // 스테이지에 존재하는 안개들을 나타냅니다.
    Fog fogs[100][100];
    // 타일 배치도의 가로 길이, 세로 길이를 나타냅니다.
    int  width, height;
    int tileSize;
    // 이하는 스테이지 로딩에 사용되는 트랜지션 이미지, 상태에 대한 정보.
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
// 스테이지의 업데이트 함수입니다. 스테이지에 소속된 StaticTile 혹은 DynamicGimmick들의 Update 함수들을 모두 호출합니다.
void Stage_Update();
// 스테이지를 렌더링하면, 스테이지가 포함하고 있는 타일과 주인공, 기믹들을 모두 렌더합니다.
void Stage_Render(Vector2i position);

void Stage_ChangeStage(LPCWSTR* filename);
// 파일이름을 매개변수로 주면 해당 파일을 해독하여 스테이지를 재구성합니다.
void Stage_PlayStageBGM();
void Stage_LoadStage(LPCWSTR filename);
void Stage_ReloadStage();
void Stage_ReloadStageFile();
// 해당 파일을 해독하여 타일별 정보에 따라 타일 타입 할당
void Stage_ProcessTileInfo(char** fileInfo, int row, int col);
BOOL Stage_isAccessible(Vector2i rowCol);
Stage_InteractionType Stage_GetInteractionType(Vector2i interactionTarget, Vector2i inputDirection);
Vector2i Stage_RowColToPosition(Vector2i rowCol);
void Stage_IncrementProgress();
void Stage_OnStageClearDefault();
