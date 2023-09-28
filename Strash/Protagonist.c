#include "Protagonist.h"
#include "Time.h"
#include "GDIEngine.h"
#include "Stage.h"
#include "Sound.h"
#include "UserInput.h"

const double Protagonist_walkDuration = 0.3;
const double Protagonist_pushDuration = 0.4;
int frame = 0;
//int account = 0;
// BOOL isKeyInput = FALSE;

void Protagonist_MoveTo(Vector2i src, Vector2i dst)
{
    if (Protagonist_Instance.state == Protagonist_State_Idle)
    {
        SoundPlay(SoundType_Effect_FOOTSTEP);
        Protagonist_Instance.state = Protagonist_State_Walking;
        Protagonist_Instance.action_elapsed = 0;
        Protagonist_Instance.walk_offset.x = 0;
        Protagonist_Instance.walk_offset.y = 0;
        Protagonist_Instance.walk_src.x = src.x;
        Protagonist_Instance.walk_src.y = src.y;
        Protagonist_Instance.walk_dst.x = dst.x;
        Protagonist_Instance.walk_dst.y = dst.y;

        Protagonist_Instance.walk_current.x = src.x;
        Protagonist_Instance.walk_current.y = src.y;

    }
}
void Protagonist_Push()
{
    if (Protagonist_Instance.state == Protagonist_State_Idle)
    {
        SoundPlay(SoundType_Effect_PUSH);
        Protagonist_Instance.state = Protagonist_State_Pushing;
        Protagonist_Instance.action_elapsed = 0;
    }
}
void Protagonist_Push_Futile()
{
    if (Protagonist_Instance.state == Protagonist_State_Idle)
    {
        Protagonist_Instance.state = Protagonist_State_Pushing;
        Protagonist_Instance.action_elapsed = 0;
    }
}
void Protagonist_Update()
{
    Protagonist_Instance.action_elapsed += Time_Instance.deltaTime;
    // account += Time_Instance.deltaTime;
    switch (Protagonist_Instance.state)
    {
        Vector2d lerp;
    case Protagonist_State_Idle:
		if (Protagonist_Instance.action_elapsed >= 1)
			Protagonist_Instance.action_elapsed = 0;
        frame = Protagonist_Instance.action_elapsed / 0.5;
        Protagonist_Instance.walk_offset.x = 180 * frame;
        break;

    case Protagonist_State_Walking:
        // 출력할 스프라이트의 관리
        // 걷기 시작한 경과 시간이 0.15초를 넘기면 walk_offset의 시작점을 오른쪽으로 180만큼 당긴다.
        frame = Protagonist_Instance.action_elapsed * 2 / Protagonist_walkDuration;
        frame %= 2;
        Protagonist_Instance.walk_offset.x = 180 * frame;


        // 물리적 움직임 처리
        double t = Protagonist_Instance.action_elapsed / Protagonist_walkDuration;
        t = 1 - (1 - t) * (1 - t);
        lerp = Vector2d_Lerp(Protagonist_Instance.walk_src, Protagonist_Instance.walk_dst, t);
        Protagonist_Instance.walk_current.x = lerp.x;
        Protagonist_Instance.walk_current.y = lerp.y;
        if (Protagonist_Instance.action_elapsed > Protagonist_walkDuration)
            Protagonist_Instance.state = Protagonist_State_Idle;
        break;

    case Protagonist_State_Pushing:
        if (Protagonist_Instance.action_elapsed > Protagonist_pushDuration)
            Protagonist_Instance.state = Protagonist_State_Idle;
        break;

    default:

        break;
    }
}
void Protagonist_Render(Vector2i position, Vector2i drawRect)
{
    double sizeFactor = 1.5;
    // 잘라낼 스프라이트의 크기
    Vector2i cropRect = { 180 , 180 };
    // 스프라이트 잘라내는 곳의 시작점
    static Vector2i offset = { 180 , 0 };

    // 다음 애니메이션
    offset.x += 180;
    offset.x %= 180 * 4;

    position.y -= stageInstance.tileSize * (sizeFactor - 1) / 2.0f;
    drawRect.x *= sizeFactor;
    drawRect.y *= sizeFactor;
    switch (Protagonist_Instance.state)
    {
    case Protagonist_State_Idle:
        if (Protagonist_Instance.facingVector.x == 1 && Protagonist_Instance.facingVector.y == 0)
        {
            GDI_DrawPartialBitmap(L"sprite sheet\\Sprite Sheet_Standing_Right.bmp", position, drawRect, cropRect, Protagonist_Instance.walk_offset);
        }
        else if (Protagonist_Instance.facingVector.x == 0 && Protagonist_Instance.facingVector.y == 1
            || (Protagonist_Instance.facingVector.x == 0 && Protagonist_Instance.facingVector.y == 0))
        {
            GDI_DrawPartialBitmap(L"sprite sheet\\Sprite Sheet_Standing_Front.bmp", position, drawRect, cropRect, Protagonist_Instance.walk_offset);
        }
        else if (Protagonist_Instance.facingVector.x == -1 && Protagonist_Instance.facingVector.y == 0)
        {
            GDI_DrawPartialBitmap(L"sprite sheet\\Sprite Sheet_Standing_Left.bmp", position, drawRect, cropRect, Protagonist_Instance.walk_offset);
        }
        else if (Protagonist_Instance.facingVector.x == 0 && Protagonist_Instance.facingVector.y == -1)
        {
            GDI_DrawPartialBitmap(L"sprite sheet\\Sprite Sheet_Standing_Back.bmp", position, drawRect, cropRect, Protagonist_Instance.walk_offset);
        }
        break;
    case Protagonist_State_Walking:
        //GDI_DrawBitmap(L"sprite sheet\\Sprite Sheet_Standing_Front.bmp", Protagonist_Instance.walk_current);
        Protagonist_Instance.walk_current.y -= stageInstance.tileSize * (sizeFactor - 1) / 2.0f;
        if (Protagonist_Instance.facingVector.x == 1 && Protagonist_Instance.facingVector.y == 0)
        {
            GDI_DrawPartialBitmap(L"sprite sheet\\Sprite Sheet_Running_Right.bmp", Protagonist_Instance.walk_current, drawRect, cropRect, Protagonist_Instance.walk_offset);
        }
        else if (Protagonist_Instance.facingVector.x == 0 && Protagonist_Instance.facingVector.y == 1)
        {
            GDI_DrawPartialBitmap(L"sprite sheet\\Sprite Sheet_Running_Front.bmp", Protagonist_Instance.walk_current, drawRect, cropRect, Protagonist_Instance.walk_offset);
        }
        else if (Protagonist_Instance.facingVector.x == -1 && Protagonist_Instance.facingVector.y == 0)
        {
            GDI_DrawPartialBitmap(L"sprite sheet\\Sprite Sheet_Running_left.bmp", Protagonist_Instance.walk_current, drawRect, cropRect, Protagonist_Instance.walk_offset);
        }
        else if (Protagonist_Instance.facingVector.x == 0 && Protagonist_Instance.facingVector.y == -1)
        {
            GDI_DrawPartialBitmap(L"sprite sheet\\Sprite Sheet_Running_Back.bmp", Protagonist_Instance.walk_current, drawRect, cropRect, Protagonist_Instance.walk_offset);
        }
        break;
    case Protagonist_State_Pushing:
        Protagonist_Instance.action_elapsed > Protagonist_pushDuration;

        if (Protagonist_Instance.facingVector.x == 1 && Protagonist_Instance.facingVector.y == 0)
        {
            GDI_DrawPartialBitmap(L"sprite sheet\\Sprite Sheet_Pushing_Right.bmp", position, drawRect, cropRect, Protagonist_Instance.walk_offset);
        }
        else if (Protagonist_Instance.facingVector.x == 0 && Protagonist_Instance.facingVector.y == 1)
        {
            GDI_DrawPartialBitmap(L"sprite sheet\\Sprite Sheet_Pushing_Front.bmp", position, drawRect, cropRect, Protagonist_Instance.walk_offset);
        }
        else if (Protagonist_Instance.facingVector.x == -1 && Protagonist_Instance.facingVector.y == 0)
        {
            GDI_DrawPartialBitmap(L"sprite sheet\\Sprite Sheet_Pushing_Left.bmp", position, drawRect, cropRect, Protagonist_Instance.walk_offset);
        }
        else if (Protagonist_Instance.facingVector.x == 0 && Protagonist_Instance.facingVector.y == -1)
        {
            GDI_DrawPartialBitmap(L"sprite sheet\\Sprite Sheet_Pushing_Back.bmp", position, drawRect, cropRect, Protagonist_Instance.walk_offset);
        }
        break;
    default:
        break;
    }

    //if (Protagonist_Instance.facingVector.x == 0 && Protagonist_Instance.facingVector.y == 0)
    //{	// 정지 상태
    //	Protagonist_Instance.state = Protagonist_State_Idle;
    //	position.y -= stageInstance.tileSize * (sizeFactor - 1) / 2.0f;
    //	GDI_DrawPartialBitmap(L"Sprite Sheet_Standing_Front.bmp", position, drawRect, cropRect, Vector2i_Zero);
    //	//Protagonist_Instance.state = Protagonist_State_Idle;
    //}
    //else if (Protagonist_Instance.facingVector.x == 1 && Protagonist_Instance.facingVector.y == 0)
    //{	// 오른쪽
    //	Protagonist_Instance.state = Protagonist_State_Walking;
    //	Protagonist_Instance.walk_current.y -= stageInstance.tileSize * (sizeFactor - 1) / 2.0f;
    //	GDI_DrawPartialBitmap(L"Sprite Sheet_Running_right.bmp", Protagonist_Instance.walk_current, drawRect, cropRect, Protagonist_Instance.walk_offset);
    //	//Protagonist_Instance.state = Protagonist_State_Idle;
    //}
    //else if (Protagonist_Instance.facingVector.x == 0 && Protagonist_Instance.facingVector.y == 1)
    //{	// 아래
    //	Protagonist_Instance.state = Protagonist_State_Walking;
    //	Protagonist_Instance.walk_current.y -= stageInstance.tileSize * (sizeFactor - 1) / 2.0f;
    //	GDI_DrawPartialBitmap(L"Sprite Sheet_Running_Front.bmp", Protagonist_Instance.walk_current, drawRect, cropRect, Protagonist_Instance.walk_offset);
    //	//Protagonist_Instance.state = Protagonist_State_Idle;
    //}
    //else if (Protagonist_Instance.facingVector.x == 0 && Protagonist_Instance.facingVector.y == -1)
    //{	// 왼쪽
    //	Protagonist_Instance.state = Protagonist_State_Walking;
    //	Protagonist_Instance.walk_current.y -= stageInstance.tileSize * (sizeFactor - 1) / 2.0f;
    //	GDI_DrawPartialBitmap(L"Sprite Sheet_Running_left.bmp", Protagonist_Instance.walk_current, drawRect, cropRect, Protagonist_Instance.walk_offset);
    //}
    //else if (Protagonist_Instance.facingVector.x == -1 && Protagonist_Instance.facingVector.y == 0)
    //{	// 위
    //	Protagonist_Instance.state = Protagonist_State_Walking;
    //	Protagonist_Instance.walk_current.y -= stageInstance.tileSize * (sizeFactor - 1) / 2.0f;
    //	GDI_DrawPartialBitmap(L"Sprite Sheet_Running_Back.bmp", Protagonist_Instance.walk_current, drawRect, cropRect, Protagonist_Instance.walk_offset);
    //}

    //switch (Protagonist_Instance.state)
    //{
    //case Protagonist_State_Idle:
    //	//GDI_DrawBitmap(L"Sprite Sheet_Standing_Front.bmp", position);
    //	position.y -= stageInstance.tileSize * (sizeFactor - 1) / 2.0f;
    //	GDI_DrawPartialBitmap(L"Sprite Sheet_Standing_Front.bmp", position, drawRect, cropRect, Vector2i_Zero);
    //	break;
    //case Protagonist_State_Walking:
    //	//GDI_DrawBitmap(L"Sprite Sheet_Standing_Front.bmp", Protagonist_Instance.walk_current);
    //	Protagonist_Instance.walk_current.y -= stageInstance.tileSize * (sizeFactor - 1) / 2.0f;
    //	GDI_DrawPartialBitmap(L"Sprite Sheet_Running_right.bmp", Protagonist_Instance.walk_current, drawRect, cropRect, Protagonist_Instance.walk_offset);
    //	break;
    //}
}
