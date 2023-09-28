#pragma once
#define KEYCOUNT 100

#include <windows.h>
#include "Vector2i.h"

typedef enum _tagUserInput_KEY_STATE
{
    UserInput_KEY_STATE_NONE,
    UserInput_KEY_STATE_TAP,
    UserInput_KEY_STATE_HOLD,
    UserInput_KEY_STATE_AWAY,
}UserInput_KEY_STATE;

typedef enum _tagKEY
{
    UserInput_KEY_LEFT,
    UserInput_KEY_RIGHT,
    UserInput_KEY_UP,
    UserInput_KEY_DOWN,

    UserInput_KEY_SPACE,

    UserInput_KEY_Q,
    UserInput_KEY_E,
    UserInput_KEY_R,
    UserInput_KEY_W,
    UserInput_KEY_A,
    UserInput_KEY_S,
    UserInput_KEY_D,

    UserInput_KEY_LBUTTON,
    UserInput_KEY_RBUTTON,
    UserInput_KEY_BRACKET_LARGE_OPEN,

    UserInput_KEY_ESCAPE,
    UserInput_KEY_TAB,

    UserInput_KEY_LAST,
}KEY;

typedef struct _tagKeyInfo
{
    UserInput_KEY_STATE m_state;
    BOOL m_prevPush;
}KeyInfo;

typedef struct _tagUserInput
{
    KeyInfo m_key[KEYCOUNT];
}UserInput;

UserInput g_UserInput;

// 입력 시스템을 초기화한다.
UserInput_Init();
// 입력을 받는다.
UserInput_Update();

UserInput_KEY_STATE UserInput_GetKeyState(KEY _key);

POINT UserInput_mosuePosition_Point;
Vector2i UserInput_mosuePosition_Vector;