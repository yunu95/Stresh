#pragma once
#include <Windows.h>
#include "Vector2i.h"

// Letter는 게임에서 퍼즐의 단서를 텍스트로 제공하는 역할을 하는 UI를 말합니다.
typedef struct
{
    TCHAR text[200];
} Letter;

Letter Letter_instance;

// 표시할 Letter의 정보를 파일을 읽어들여 가져옵니다.
void Letter_Load(const char* filename);
// Letter가 천천히 펼쳐지거나, 사라지는 액션을 수행하게 됩니다.
void Letter_Update();
// Letter를 화면에 표시합니다.
void Letter_Render(Vector2i position);
