#pragma once

#include "GDIEngine.h"
#include "ResManager.h"
#include "texture.h"

/// 모듈 내에서 쓸 전역 변수


double GDI_ScreenFactor = 1;
LPCWSTR defaultBitmapPath = L".\\texture\\";
const int GDIEngine_ResolutionW = 1920;
const int GDIEngine_ResolutionH = 1080;
const double topBarHeight = 36;
RECT GDIEngine_WndRect = { 0,0,0,0 };
int GDIEngine_WndResolutionW = 0;
int GDIEngine_WndResolutionH = 0;
Vector2i GDIEngine_center;
// 윈도우 핸들
HDC g_hWnd;
// 버퍼 memDc
HDC g_hMemDC;
// 윈도우 dc
HDC g_hScreenDC;
HBRUSH g_hmazentaBrush;
HDC intermediate_DC;
HDC intermediate_DC2;
HBITMAP hBit;

HFONT g_hFont;

/// <summary>
/// GDI를 이용한 2D 그래픽스 엔진을 초기화한다.
/// </summary>
/// <param name="hWnd">그래픽스 엔진을 사용할 대상이 되는 윈도의 핸들</param>
void GDI_Initialize(HWND hWnd)
{
	g_hScreenDC = GetDC(hWnd);
	g_hMemDC = CreateCompatibleDC(g_hScreenDC);
	GDIEngine_center.x = GDIEngine_ResolutionW * 0.5;
	GDIEngine_center.y = GDIEngine_ResolutionH * 0.5;
	HBITMAP hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	DeleteObject((HBITMAP)SelectObject(g_hMemDC, hBit));
	intermediate_DC = CreateCompatibleDC(g_hScreenDC);
	intermediate_DC2 = CreateCompatibleDC(g_hScreenDC);
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW*2, GDIEngine_ResolutionH*2);
	DeleteObject((HBITMAP)SelectObject(intermediate_DC, hBit));
	hBit = CreateCompatibleBitmap(g_hScreenDC, GDIEngine_ResolutionW, GDIEngine_ResolutionH);
	g_hmazentaBrush = CreateSolidBrush(RGB(255, 0, 255));
	DeleteObject((HBITMAP)SelectObject(intermediate_DC2, hBit));
	g_hWnd = hWnd;

	//g_Core.m_hBit = CreateCompatibleBitmap(g_Core.m_hDC, g_Core.m_Resolution.x, g_Core.m_Resolution.y);
	//g_hMemDC = Memdc GetDC(hWnd);
}

/// <summary>
/// GDI를 이용해서, 시작점과 끝점을 잇는 선분을 하나 그린다.
/// </summary>
void GDI_DrawLine(int startX, int startY, int endX, int endY)
{
	// 특정 지점으로 이동한다.
	MoveToEx(g_hMemDC, startX, startY, NULL);

	// 거기에서부터 특정 지점까지 선을 만든다.
	LineTo(g_hMemDC, endX, endY);
}

//void GDI_DrawBitmap(HDC hBitmapDC, int dstX, int dstY, int sizeX, int sizeY)
//{
//    BitBlt(g_hMemDC, dstX, dstY, sizeX, sizeY, hBitmapDC, 0, 0, SRCCOPY);
//}
void GDI_DrawBitmap(LPCWSTR filename, Vector2i position)
{
	GDI_DrawPartialBitmap(filename, position, Vector2i_Zero, Vector2i_Zero, Vector2i_Zero);
}
void GDI_DrawResizedBitmap(LPCWSTR filename, Vector2i position, Vector2i drawRect)
{
	GDI_DrawPartialBitmap(filename, position, drawRect, Vector2i_Zero, Vector2i_Zero);
}
void GDI_DrawPartialBitmap(LPCWSTR filename, Vector2i position, Vector2i drawRect, Vector2i cropRect, Vector2i offset)
{
	GDI_DrawPartialBitmapAlpha(filename, position, drawRect, cropRect, offset, 1);
}
void GDI_DrawPartialBitmapAlpha(LPCWSTR filename, Vector2i position, Vector2i drawRect, Vector2i cropRect, Vector2i offset, double alpha)
{
	BLENDFUNCTION blend;
	blend.BlendOp = 0;
	blend.BlendFlags = 0;
	blend.AlphaFormat = AC_SRC_OVER;
	blend.SourceConstantAlpha = alpha * 255;
	GDI_DrawPartialBitmapAlphaWithBf(filename, position, drawRect, cropRect, offset, blend);
}
void GDI_DrawPartialBitmapAlphaWithBf(LPCWSTR filename, Vector2i position, Vector2i drawRect, Vector2i cropRect, Vector2i offset, BLENDFUNCTION bf)
{
	if (filename == 0)
		return;

	Texture* _pTex = NULL;
	WCHAR fpath[200] = { 0 };
	lstrcatW(fpath, defaultBitmapPath);
	lstrcatW(fpath, filename);
	_pTex = ResManager_FindTexture(fpath);
	if (_pTex == NULL)
	{
		ResManager_LoadTexture(&_pTex, fpath);
		_pTex = ResManager_FindTexture(fpath);
	}
	//SetTimer(g_hWnd, 1, 200, NULL);
	int frame = 0;
	int width = _pTex->m_bitInfo.bmWidth;
	int height = _pTex->m_bitInfo.bmHeight;



	if (drawRect.x == 0 && drawRect.y == 0)
	{
		drawRect.x = width;
		drawRect.y = height;
	}
	if (cropRect.x == 0)
		cropRect.x = width;
	if (cropRect.y == 0)
		cropRect.y = height;

	if (drawRect.x == 0 && drawRect.y == 0)
	{
		drawRect.x = width;
		drawRect.y = height;
	}
	if (cropRect.x == 0)
		cropRect.x = width;
	if (cropRect.y == 0)
		cropRect.y = height;


	StretchBlt(intermediate_DC,
		0, 0,
		drawRect.x, drawRect.y,
		_pTex->m_hDC,
		offset.x, offset.y,
		cropRect.x, cropRect.y, SRCCOPY);

	if (bf.SourceConstantAlpha == 255)
	{
		TransparentBlt(g_hMemDC
			, (int)(position.x - (float)(drawRect.x / 2))
			, (int)(position.y - (float)(drawRect.y / 2))
			, drawRect.x, drawRect.y
			, intermediate_DC
			, 0, 0, drawRect.x, drawRect.y
			, RGB(255, 0, 255));
	}
	else
	{
		//255면 그냥 보여주는거
		//0이면 안보여줌
		RECT rect;
   		rect.left = -1;
		rect.right = GDIEngine_ResolutionW + 1;
		rect.top = -1;
		rect.bottom = GDIEngine_ResolutionH + 1;
		double factor = bf.SourceConstantAlpha/ 255.0;
		//FillRect(intermediate_DC2, &rect, g_hmazentaBrush);
		//TransparentBlt(intermediate_DC2
		//	, 0
		//	, 0
		//	, drawRect.x, drawRect.y
		//	, intermediate_DC
		//	, 0, 0, drawRect.x, drawRect.y
		//	, RGB(255, 0, 255));

		//FillRect(intermediate_DC2, &rect, g_hmazentaBrush);
		//bf.AlphaFormat
		GdiAlphaBlend(g_hMemDC,
			(int)(position.x - (float)(drawRect.x / 2)),
			(int)(position.y - (float)(drawRect.y / 2)),
			drawRect.x, drawRect.y,
			intermediate_DC,
			0,
			0,
			drawRect.x,
			drawRect.y,
			bf);
		//GdiAlphaBlend(g_hScreenDC,
		//	0,
		//	0,
		//	width, height,
		//	intermediate_DC,
		//	0,
		//	0,
		//	drawRect.x,
		//	drawRect.y,
		//	bf);
		TransparentBlt(g_hMemDC,
			(int)(position.x - (float)(drawRect.x / 2))
			, (int)(position.y - (float)(drawRect.y / 2))
			, drawRect.x, drawRect.y
			, intermediate_DC2
			, 0, 0, drawRect.x, drawRect.y
			, RGB(255*factor, 0, 255 * factor)); 
	}

}

void GDI_WriteText(LPCTSTR _text, Vector2i _pos, RECT _Area)
{
	g_hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("밀양영남루체"));
	SelectObject(g_hMemDC, g_hFont);
	DrawText(g_hMemDC, _text, -1, &_Area, DT_WORDBREAK);
}
void GDI_ResetBuffer()
{
	Rectangle(g_hMemDC, -1, -1, GDIEngine_ResolutionW + 1, GDIEngine_ResolutionH + 1);
}
void GDI_ApplyBuffer()
{
	double factorW;
	double factorH;
	GetWindowRect(g_hWnd, &GDIEngine_WndRect);
	factorW = (double)(GDIEngine_WndRect.right - GDIEngine_WndRect.left) / GDIEngine_ResolutionW;
	factorH = (double)(GDIEngine_WndRect.bottom - GDIEngine_WndRect.top - topBarHeight) / GDIEngine_ResolutionH;
	GDI_ScreenFactor = factorH < factorW ? factorH : factorW;
	GDIEngine_WndResolutionW = (GDIEngine_WndRect.right - GDIEngine_WndRect.left);
	GDIEngine_WndResolutionH = (GDIEngine_WndRect.bottom - GDIEngine_WndRect.top - topBarHeight);
	StretchBlt(g_hScreenDC,
		(0.5 * (GDIEngine_WndRect.right - GDIEngine_WndRect.left) - 0.5 * GDIEngine_ResolutionW * GDI_ScreenFactor),
		(0.5 * (GDIEngine_WndRect.bottom - GDIEngine_WndRect.top - topBarHeight) - 0.5 * GDIEngine_ResolutionH * GDI_ScreenFactor),
		GDIEngine_ResolutionW * GDI_ScreenFactor, GDIEngine_ResolutionH * GDI_ScreenFactor,
		g_hMemDC,
		0, 0,
		GDIEngine_ResolutionW, GDIEngine_ResolutionH, SRCCOPY);
}
Vector2i GDI_screenToStdResolution(Vector2i screenPosition)
{
	screenPosition.x -= (0.5 * GDIEngine_WndResolutionW - 0.5 * GDIEngine_ResolutionW * GDI_ScreenFactor);
	screenPosition.y -= (0.5 * GDIEngine_WndResolutionH - 0.5 * GDIEngine_ResolutionH * GDI_ScreenFactor);

	screenPosition.x /= GDI_ScreenFactor;
	screenPosition.y /= GDI_ScreenFactor;
	return screenPosition;
}
Vector2i GDI_StdResolutionToScreen(Vector2i resolPosition)
{
	Vector2i ret;
	ret.x = (0.5 * GDIEngine_WndResolutionW - 0.5 * GDIEngine_ResolutionW * GDI_ScreenFactor);
	ret.y = (0.5 * GDIEngine_WndResolutionH - 0.5 * GDIEngine_ResolutionH * GDI_ScreenFactor);

	ret.x += resolPosition.x * GDI_ScreenFactor;
	ret.y += resolPosition.y * GDI_ScreenFactor;
	return ret;
}
HDC GDI_LoadBitmapFromFile(LPCWSTR filePath)
{
	HDC hBitmapDC = CreateCompatibleDC(g_hMemDC);
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, filePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// 메모리 DC에 비트맵을 장착
	SelectObject(hBitmapDC, hBitmap);

	// 부가정보는 없다.

	// 비트맵 정보를 삭제
	DeleteObject(hBitmap);


	return hBitmapDC;
}