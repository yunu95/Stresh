#pragma once
#include "Res.h"

typedef struct _tagTexture
{
	HDC m_hDC;
	HBITMAP m_hBit;
	BITMAP m_bitInfo;
	Res m_res;
}Texture;

void Texture_Load(Texture* _texture, LPCTSTR _strFilepath);
