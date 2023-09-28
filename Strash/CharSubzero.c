//#include "CharSubzero.h"
//#include "TimeControl.h"
//#include "GDIEngine.h"
//
//void CharSubzero_Start()
//{
//    CharSubzero_position.x = 100;
//    CharSubzero_position.y = 100;
//    CharSubzero_spriteIndex = 0;
//    CharSubzero_bitmapDC = GDI_LoadBitmapFromFile(L"subzero.bmp");
//    CharSubzero_State = CharSubzero_IDLE;
//}
//void CharSubzero_Update()
//{
//    static double accumulatedTime = 0;
//    const double fdeltaTime = 0.2f;
//    accumulatedTime += TimeControl_deltaTime;
//    int xSrc = 0, ySrc = 0;
//    int wSrc = 0, hSrc = 0;
//    switch (CharSubzero_State)
//    {
//    case CharSubzero_IDLE:
//        if (accumulatedTime > fdeltaTime)
//        {
//            accumulatedTime -= fdeltaTime;
//            CharSubzero_spriteIndex = ++CharSubzero_spriteIndex % 23;
//        }
//
//        xSrc = 1 + CharSubzero_spriteIndex * 48;
//        ySrc = 14;
//        wSrc = 46;
//        hSrc = 98;
//
//
//        break;
//    case CharSubzero_KICK:
//        break;
//
//    default:
//        break;
//    }
//    //GDI_DrawBitmapTransparent(CharSubzero_bitmapDC, CharSubzero_position.x, CharSubzero_position.y, wSrc, hSrc, xSrc, ySrc, wSrc, hSrc, RGB(0, 66, 66));
//}
