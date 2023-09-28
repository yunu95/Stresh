#pragma once
#include "texture.h"
typedef enum
{
	BlackMask_State_IDLE,
	BlackMask_State_FADEIN,
	BlackMask_State_FADEOUT,
}BlackMask_State;

typedef struct 
{
	float m_acc;
	float m_duration;
	float m_percent;
	BLENDFUNCTION m_blend;
	BOOL m_isFirst;
	BOOL m_isFade;
	BOOL m_isOnlyFadeOut;
	BlackMask_State m_state;
	Texture* m_blackMaskTexture;
	void (*onFadeOutFinish)();
}BlackMask;

BlackMask g_blackMask;

BlackMask_Init();
BlackMask_Update();
BlackMask_Render();