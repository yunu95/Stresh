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
// 윈도우 핸들
HDC g_hWnd;
// 버퍼 memDc
HDC g_hMemDC;
// 윈도우 dc
HDC g_hScreenDC;
HDC intermediate_DC;

HFONT g_hFont;
double GDI_ScreenFactor;

// (GDI를 사용한) 그래픽스 엔진을 초기화한다.
// 필요한 것은 hWnd (윈도 핸들)
void GDI_Initialize(HWND hWnd);

// GDI를 이용해서, 시작점과 끝점을 잇는 선분을 하나 그린다.
void GDI_DrawLine(int startX, int startY, int endX, int endY);

// 비트맵을 그린다.
//void GDI_DrawBitmap(HDC hBitmapDC, int dstX, int dstY, int sizeX, int sizeY);
// position의 위치를 중심으로 사방에 비트맵 이미지를 그립니다. 0,0은 좌상단, (0.5*resX,0.5*resY)은 화면의 중앙이 됩니다.
void GDI_DrawBitmap(LPCWSTR filename, Vector2i position);
void GDI_DrawResizedBitmap(LPCWSTR filename, Vector2i position, Vector2i drawRect);
void GDI_DrawPartialBitmap(LPCWSTR filename, Vector2i position, Vector2i drawRect, Vector2i cropRect, Vector2i offset);
void GDI_DrawPartialBitmapAlphaWithBf(LPCWSTR filename, Vector2i position, Vector2i drawRect, Vector2i cropRect, Vector2i offset, BLENDFUNCTION bf);
void GDI_DrawPartialBitmapAlpha(LPCWSTR filename, Vector2i position, Vector2i drawRect, Vector2i cropRect, Vector2i offset,double alpha);
// 글자를 출력한다.
void GDI_WriteText(LPCTSTR _text, Vector2i _pos, RECT _Area);

// MemDc를 초기화한다.
void GDI_ResetBuffer();
// MemDc를 게임화면에 적용한다..
void GDI_ApplyBuffer();
// 파일에서 비트맵 하나를 로드해서, 메모리 DC까지
HDC GDI_LoadBitmapFromFile(LPCWSTR filePath);
Vector2i GDI_screenToStdResolution(Vector2i screenPosition);
Vector2i GDI_StdResolutionToScreen(Vector2i resolPosition);