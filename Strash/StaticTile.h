#pragma once
#include <Windows.h>
#include "Vector2i.h"

// 스테이지가 불러와진 이후부터 종료되기 전까지 상태가 변하지 않는 타일입니다.
// 이들의 Update,Render 함수는 Stage에 의해 종속적으로 호출됩니다.
typedef enum
{
    TileType_NULL, TileType_PATH, TileType_WALL, TileType_STARTPOINT, TileType_PATH_BUTWALL
}
TileType;

typedef struct
{
    // 타일의 타입을 나타내는 이넘! 변수입니다.
    TileType tileType;
    // 렌더링의 대상이 됩니까?.
    BOOL doesRender;
}
StaticTile;

void StaticTile_Render(StaticTile* instance, Vector2i position, Vector2i drawRect);
