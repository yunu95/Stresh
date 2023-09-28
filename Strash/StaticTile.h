#pragma once
#include <Windows.h>
#include "Vector2i.h"

// ���������� �ҷ����� ���ĺ��� ����Ǳ� ������ ���°� ������ �ʴ� Ÿ���Դϴ�.
// �̵��� Update,Render �Լ��� Stage�� ���� ���������� ȣ��˴ϴ�.
typedef enum
{
    TileType_NULL, TileType_PATH, TileType_WALL, TileType_STARTPOINT, TileType_PATH_BUTWALL
}
TileType;

typedef struct
{
    // Ÿ���� Ÿ���� ��Ÿ���� �̳�! �����Դϴ�.
    TileType tileType;
    // �������� ����� �˴ϱ�?.
    BOOL doesRender;
}
StaticTile;

void StaticTile_Render(StaticTile* instance, Vector2i position, Vector2i drawRect);
