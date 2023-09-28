#pragma once
#include <Windows.h>
#include "Vector2i.h"
#include "texture.h"

typedef struct _Text
{
	Vector2i position;
	LPCWSTR imageName;
	Texture* m_tex;
	Vector2i m_drawRect;
	BOOL m_isActive;
	int m_layer;

	// 점점 진해지게 만들기 위한 변수
	BLENDFUNCTION blend;
	float m_acc;
	float m_percent;
	BOOL m_isFirst;
}Text;