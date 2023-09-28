#pragma once

typedef enum
{
    SceneTransitionImageState_GONE,
    SceneTransitionImageState_EMERGING,
    SceneTransitionImageState_DISAPPEARING
}
SceneTransitionImageState;

// ���� ��ȯ�Ǵ� ���� �ڿ������� ������ �ִϸ��̼� �̹����Դϴ�. �ٸ� �������� ������ ��ҵ�� ����������, ���� �Լ����� ���������� Update�� ���� ȣ��˴ϴ�.
typedef struct
{
    SceneTransitionImageState state;
}
SceneTransitionImage;

SceneTransitionImage SceneTransitionImage_Instance;
void SceneTransitionImage_Update();
