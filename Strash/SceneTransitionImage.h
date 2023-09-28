#pragma once

typedef enum
{
    SceneTransitionImageState_GONE,
    SceneTransitionImageState_EMERGING,
    SceneTransitionImageState_DISAPPEARING
}
SceneTransitionImageState;

// 씬이 전환되는 것을 자연스럽게 보여줄 애니메이션 이미지입니다. 다른 스테이지 내부의 요소들과 마찬가지로, 관련 함수들이 스테이지의 Update에 의해 호출됩니다.
typedef struct
{
    SceneTransitionImageState state;
}
SceneTransitionImage;

SceneTransitionImage SceneTransitionImage_Instance;
void SceneTransitionImage_Update();
