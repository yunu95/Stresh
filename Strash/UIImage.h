#pragma once
#include "Vector2i.h"
#include "texture.h"
// UI window에 배치될 수 있는 하나의 비트맵 이미지를 뜻합니다.
typedef struct
{
    Vector2i position;
    LPCWSTR imageName;
    Vector2i m_drawRect;
    BOOL m_isActive;
    int m_layer;
}
UIImage;

void UIImage_Update(UIImage* image);
