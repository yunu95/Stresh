#include "StaticTile.h"
#include "GDIEngine.h"

void StaticTile_Render(StaticTile* instance, Vector2i position, Vector2i drawRect)
{
    if (!instance->doesRender)
        return;

    switch (instance->tileType)
    {
    case TileType_PATH_BUTWALL:
    case TileType_PATH:
        GDI_DrawResizedBitmap(L"UI\\tile_1_frame.bmp", position, drawRect);
        break;
    case TileType_WALL:
        GDI_DrawResizedBitmap(L"UI\\tile_2.bmp", position, drawRect);
        break;
    case TileType_NULL:
        break;
    default:
        break;
    }
}