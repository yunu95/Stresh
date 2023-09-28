#pragma once
#include <Windows.h>
#include "Vector2i.h"

// Letter�� ���ӿ��� ������ �ܼ��� �ؽ�Ʈ�� �����ϴ� ������ �ϴ� UI�� ���մϴ�.
typedef struct
{
    TCHAR text[200];
} Letter;

Letter Letter_instance;

// ǥ���� Letter�� ������ ������ �о�鿩 �����ɴϴ�.
void Letter_Load(const char* filename);
// Letter�� õõ�� �������ų�, ������� �׼��� �����ϰ� �˴ϴ�.
void Letter_Update();
// Letter�� ȭ�鿡 ǥ���մϴ�.
void Letter_Render(Vector2i position);
