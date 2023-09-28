#include "BlackMask.h"
#include "Time.h"
#include "GDIEngine.h"
#include "UserInput.h"
#include <stdio.h>

BlackMask_Init()
{
	g_blackMask.m_acc = 0.f;
	g_blackMask.m_duration = 1.f;
	g_blackMask.m_state = BlackMask_State_IDLE;
	g_blackMask.m_percent = 0.f;
	g_blackMask.m_isFirst = TRUE;
	g_blackMask.m_isFade = FALSE;
	g_blackMask.m_blend.BlendOp = 0;
	g_blackMask.m_blend.BlendFlags = 0;
	g_blackMask.m_blend.AlphaFormat = AC_SRC_OVER;
	g_blackMask.m_blend.SourceConstantAlpha = 255;
	ResManager_LoadTexture(&g_blackMask.m_blackMaskTexture, L"texture\\Fade.bmp");
	g_blackMask.onFadeOutFinish = NULL;
}

BlackMask_Update()
{
	//char_t buffer[255] = { NULL };
	//wprintf_s(buffer, 255, L"isFADE : %d", g_blackMask.m_isFade);
	//etWindowText(g_hWnd, buffer);
	if (g_blackMask.m_state == BlackMask_State_FADEIN)
	{
		// È­¸éÀÌ Á¡Á¡ ¹à¾ÆÁü
		g_blackMask.m_acc += Time_Instance.deltaTime;
		g_blackMask.m_percent = g_blackMask.m_acc / g_blackMask.m_duration;
		g_blackMask.m_blend.SourceConstantAlpha = g_blackMask.m_percent < 1 ? (int)(255 * (1 - g_blackMask.m_percent)) : 0;
		if (g_blackMask.m_isFirst)
		{
			g_blackMask.m_isFade = TRUE;
			g_blackMask.m_blend.SourceConstantAlpha = 255;
			g_blackMask.m_isFirst = FALSE;
		}

		if (g_blackMask.m_percent >= 1.f)
		{
			g_blackMask.m_acc = 0.f;
			g_blackMask.m_percent = 0.f;

			g_blackMask.m_isFade = FALSE;
			g_blackMask.m_state = BlackMask_State_IDLE;
			g_blackMask.m_isFirst = TRUE;
			
		}
	}
	else if (g_blackMask.m_state == BlackMask_State_FADEOUT )
	{
		// È­¸éÀÌ Á¡Á¡ ¾îµÎ¿öÁü
		g_blackMask.m_acc += Time_Instance.deltaTime;
		g_blackMask.m_percent = g_blackMask.m_acc / g_blackMask.m_duration;
		g_blackMask.m_blend.SourceConstantAlpha = g_blackMask.m_percent < 1 ? (int)(255 * g_blackMask.m_percent) : 255;
		if (g_blackMask.m_isFirst)
		{
			g_blackMask.m_isFade = TRUE;
			g_blackMask.m_blend.SourceConstantAlpha = 0;
			g_blackMask.m_isFirst = FALSE;
		}

		if (g_blackMask.m_percent >= 1.f)
		{
			g_blackMask.m_isFade = FALSE;
			g_blackMask.m_acc = 0.f;
			g_blackMask.m_percent = 0.f;
			g_blackMask.m_isFirst = TRUE;

			if (g_blackMask.onFadeOutFinish != NULL)
			{
				g_blackMask.onFadeOutFinish();
			}
			g_blackMask.m_state = BlackMask_State_FADEIN;
		}
	}
	else
	{
		g_blackMask.m_acc = 0.f;
		g_blackMask.m_percent = 0.f;
		g_blackMask.m_isFade = FALSE;
		g_blackMask.m_isFirst = TRUE;
	}
}
BlackMask_Render()
{
	if (g_blackMask.m_state == BlackMask_State_FADEIN)
	{
		//g_blackMask.m_blend.SourceConstantAlpha = g_blackMask.m_percent <1? (int)(255 * (1-g_blackMask.m_percent)):0;

		AlphaBlend(g_hMemDC, 0, 0, 1920, 1080, g_blackMask.m_blackMaskTexture->m_hDC, 0, 0, 1920, 1080, g_blackMask.m_blend);
	}
	else if (g_blackMask.m_state == BlackMask_State_FADEOUT)
	{
		//g_blackMask.m_blend.SourceConstantAlpha = g_blackMask.m_percent<1?(int)(255 * g_blackMask.m_percent):255;

		AlphaBlend(g_hMemDC, 0, 0, 1920, 1080, g_blackMask.m_blackMaskTexture->m_hDC, 0, 0, 1920, 1080, g_blackMask.m_blend);
	}
}