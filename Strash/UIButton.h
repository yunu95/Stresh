#pragma once
#include "Vector2i.h"
#include <Windows.h>

Vector2i buttonHightlightingRect;

BOOL g_isButtonHighlighting;

// UI�� �޴� Ž���� ���̴� ��ư�Դϴ�. ���콺 Ŭ������ ������ �� �ֽ��ϴ�.
typedef struct _UIButton
{
	Vector2i position;
	LPCWSTR imageName;
	Vector2i m_area;
	Vector2i m_drawRect;
	Vector2i m_areaOrigin;
	BOOL m_isActive;
	int m_layer;
    void (*onHighlight)(struct _UIButton*);
	void (*onDeHighlight)(struct _UIButton*);
    void (*onClick)(struct _UIButton*);
}
UIButton;

void StartButton(UIButton* _button);
void HowToPlayButton(UIButton* _button);
void GameExitButton(UIButton* _button);
void SoundMuteButton(UIButton* _button);
void RetryButton();
void MainTitleExit();
void InGameExit();
void SkipButton();


void ButtonHighlighting(UIButton* _button);
void ButtonDeHighlighint(UIButton* _button);

void GoToMainTitle();
void CallBack_GoToMainTitle();