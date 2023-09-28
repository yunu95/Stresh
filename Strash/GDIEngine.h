#pragma once
#include <windows.h>
#include "Vector2i.h"


LPCWSTR defaultBitmapPath;
const int GDIEngine_ResolutionW;
const int GDIEngine_ResolutionH;
const double topBarHeight;
RECT GDIEngine_WndRect;
int GDIEngine_WndResolutionW;
int GDIEngine_WndResolutionH;
Vector2i GDIEngine_center;
// ������ �ڵ�
HDC g_hWnd;
// ���� memDc
HDC g_hMemDC;
// ������ dc
HDC g_hScreenDC;
HDC intermediate_DC;

HFONT g_hFont;
double GDI_ScreenFactor;

// (GDI�� �����) �׷��Ƚ� ������ �ʱ�ȭ�Ѵ�.
// �ʿ��� ���� hWnd (���� �ڵ�)
void GDI_Initialize(HWND hWnd);

// GDI�� �̿��ؼ�, �������� ������ �մ� ������ �ϳ� �׸���.
void GDI_DrawLine(int startX, int startY, int endX, int endY);

// ��Ʈ���� �׸���.
//void GDI_DrawBitmap(HDC hBitmapDC, int dstX, int dstY, int sizeX, int sizeY);
// position�� ��ġ�� �߽����� ��濡 ��Ʈ�� �̹����� �׸��ϴ�. 0,0�� �»��, (0.5*resX,0.5*resY)�� ȭ���� �߾��� �˴ϴ�.
void GDI_DrawBitmap(LPCWSTR filename, Vector2i position);
void GDI_DrawResizedBitmap(LPCWSTR filename, Vector2i position, Vector2i drawRect);
void GDI_DrawPartialBitmap(LPCWSTR filename, Vector2i position, Vector2i drawRect, Vector2i cropRect, Vector2i offset);
void GDI_DrawPartialBitmapAlphaWithBf(LPCWSTR filename, Vector2i position, Vector2i drawRect, Vector2i cropRect, Vector2i offset, BLENDFUNCTION bf);
void GDI_DrawPartialBitmapAlpha(LPCWSTR filename, Vector2i position, Vector2i drawRect, Vector2i cropRect, Vector2i offset,double alpha);
// ���ڸ� ����Ѵ�.
void GDI_WriteText(LPCTSTR _text, Vector2i _pos, RECT _Area);

// MemDc�� �ʱ�ȭ�Ѵ�.
void GDI_ResetBuffer();
// MemDc�� ����ȭ�鿡 �����Ѵ�..
void GDI_ApplyBuffer();
// ���Ͽ��� ��Ʈ�� �ϳ��� �ε��ؼ�, �޸� DC����
HDC GDI_LoadBitmapFromFile(LPCWSTR filePath);
Vector2i GDI_screenToStdResolution(Vector2i screenPosition);
Vector2i GDI_StdResolutionToScreen(Vector2i resolPosition);