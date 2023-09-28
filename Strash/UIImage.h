#pragma once
#include "Vector2i.h"
#include "texture.h"
// UI window�� ��ġ�� �� �ִ� �ϳ��� ��Ʈ�� �̹����� ���մϴ�.
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
