#include "UserInput.h"
#include "GDIEngine.h"
#include "BlackMask.h"
#include "Cutscene.h"

int g_arrKey[UserInput_KEY_LAST] =
{
    VK_LEFT,
    VK_RIGHT,
    VK_UP,
    VK_DOWN,

    VK_SPACE,

    'Q',
    'E',
    'R',
    'W',
    'A',
    'S',
    'D',

    VK_LBUTTON,
    VK_RBUTTON,
    VK_OEM_4,
    VK_ESCAPE,
    VK_TAB,
};

UserInput_Init()
{
    for (int i = 0; i < KEYCOUNT; ++i)
    {
        g_UserInput.m_key[i].m_prevPush = FALSE;
        g_UserInput.m_key[i].m_state = UserInput_KEY_STATE_NONE;
    }
}

UserInput_Update()
{
    HWND hMainWnd = g_hWnd;
    HWND hWnd = GetFocus();

    if (g_blackMask.m_isFade || g_isDesolve) return;

    if (NULL != hWnd)
    {
        for (int i = 0; i < KEYCOUNT; ++i)
        {
            if (GetAsyncKeyState(g_arrKey[i]) & 0x8000)
            {
                if (g_UserInput.m_key[i].m_prevPush)
                {
                    g_UserInput.m_key[i].m_state = UserInput_KEY_STATE_HOLD;
                }
                else
                {
                    g_UserInput.m_key[i].m_state = UserInput_KEY_STATE_TAP;
                }
                g_UserInput.m_key[i].m_prevPush = TRUE;
            }
            else
            {
                if (g_UserInput.m_key[i].m_prevPush)
                {
                    g_UserInput.m_key[i].m_state = UserInput_KEY_STATE_AWAY;
                }
                else
                {
                    g_UserInput.m_key[i].m_state = UserInput_KEY_STATE_NONE;
                }
                g_UserInput.m_key[i].m_prevPush = FALSE;
            }
        }
    }
    else
    {
        for (int i = 0; i < KEYCOUNT; ++i)
        {
            g_UserInput.m_key[i].m_prevPush = FALSE;
            if (UserInput_KEY_STATE_TAP == g_UserInput.m_key[i].m_state || UserInput_KEY_STATE_HOLD == g_UserInput.m_key[i].m_state)
            {
                g_UserInput.m_key[i].m_state = UserInput_KEY_STATE_AWAY;
            }
            else if (UserInput_KEY_STATE_AWAY == g_UserInput.m_key[i].m_state)
            {
                g_UserInput.m_key[i].m_state = UserInput_KEY_STATE_NONE;
            }
        }
    }
    // 마우스 위치 업데이트
	POINT pt = { NULL };
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

    UserInput_mosuePosition_Point;
	UserInput_mosuePosition_Vector;
	
    UserInput_mosuePosition_Vector.x = pt.x;
    UserInput_mosuePosition_Vector.y = pt.y;

    UserInput_mosuePosition_Vector = GDI_screenToStdResolution(UserInput_mosuePosition_Vector);
    UserInput_mosuePosition_Point.x = UserInput_mosuePosition_Vector.x;
    UserInput_mosuePosition_Point.y = UserInput_mosuePosition_Vector.y;
}

UserInput_KEY_STATE UserInput_GetKeyState(KEY _key)
{
    return g_UserInput.m_key[(int)_key].m_state;
}

