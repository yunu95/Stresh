#pragma once
#include "Vector2i.h"

// �Ȱ��� ���¸� ��Ÿ���ϴ�.
// �������� ����, ����, �Ȱ��� ���� ��, �Ȱ��� ������ ���� ���·� �����ϴ�.
typedef enum
{
    FogState_NULL, FogState_EXISTENT, FogState_EMERGING, FogState_DISAPPEARING
}
FogState;

// �Ȱ��Դϴ�. �ſ� �������� ��ü��, Update�� Render�� ��� Stage�� ���� ȣ��˴ϴ�.
typedef struct
{
    // �Ȱ��� ���¸� ��Ÿ���ϴ�.
    FogState state;
    // ���������� progress�� disappearProgress�� �Ѿ�� �Ȱ��� ������ϴ�.
    int disappearProgress;
}
Fog;

void Fog_Update(Fog* target);
void Fog_Render(Fog* target, Vector2i position);
