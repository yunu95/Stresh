#include <stdio.h>
#include "Stage.h"
#include "GDIEngine.h"
#include "UserInput.h"
#include "StaticTile.h"
#include "Vector2d.h"
#include "Protagonist.h"
#include "PathManager.h"
#include "Time.h"
#include "MovementHandler.h"
#include "Cutscene.h"
#include "Sound.h"
#include "UIWindow.h"
#include "ReloadTransition.h"
#include "BlackMask.h"
//#include <cstdlib>

LPCSTR defaultMapPath = L"maps\\";
LPCSTR transitionImage = L"bloodlust.bmp";
const double Stage_transitionDuration = 2;

void Stage_Init()
{
    DynamicGimmick_Init();
}
void Stage_Update()
{
    switch (stageInstance.state)
    {
    case Stage_State_PLAYING:
    {
        for (int row = 0; row < stageInstance.height; row++)
            for (int col = 0; col < stageInstance.width; col++)
            {
                DynamicGimmick_Update(&stageInstance.gimmicks[row][col]);
            }
        Protagonist_Update();

        //스킵하기 버튼
        if (UserInput_GetKeyState(UserInput_KEY_BRACKET_LARGE_OPEN) == UserInput_KEY_STATE_HOLD)
            stageInstance.onStageClear();

        // 사용자 입력 해석 부분
        Vector2i input = Vector2i_Zero;

        if (UserInput_GetKeyState(UserInput_KEY_LEFT) == UserInput_KEY_STATE_HOLD || UserInput_GetKeyState(UserInput_KEY_A) == UserInput_KEY_STATE_HOLD)
            input.x -= 1;
        else if (UserInput_GetKeyState(UserInput_KEY_RIGHT) == UserInput_KEY_STATE_HOLD || UserInput_GetKeyState(UserInput_KEY_D) == UserInput_KEY_STATE_HOLD)
            input.x += 1;
        else if (UserInput_GetKeyState(UserInput_KEY_UP) == UserInput_KEY_STATE_HOLD || UserInput_GetKeyState(UserInput_KEY_W) == UserInput_KEY_STATE_HOLD)
            input.y -= 1;
        else if (UserInput_GetKeyState(UserInput_KEY_DOWN) == UserInput_KEY_STATE_HOLD || UserInput_GetKeyState(UserInput_KEY_S) == UserInput_KEY_STATE_HOLD)
            input.y += 1;
        else if (UserInput_GetKeyState(UserInput_KEY_R) == UserInput_KEY_STATE_TAP &&
            Protagonist_Instance.state == Protagonist_State_Idle && !g_blackMask.m_isFade)
        {
            Stage_ReloadStage();
            return;
        }

        if ((input.x != 0 || input.y != 0) && Protagonist_Instance.state == Protagonist_State_Idle && stageInstance.state == Stage_State_PLAYING)
        {
            Vector2i interactionPoint = stageInstance.protagonistPosition;
            Protagonist_Instance.facingVector = input;

            interactionPoint.x += input.x;
            interactionPoint.y += input.y;
            if (interactionPoint.y >= stageInstance.height || interactionPoint.y < 0
                || interactionPoint.x >= stageInstance.width || interactionPoint.x < 0)
                return;

            Stage_InteractionType interactionType = Stage_GetInteractionType(interactionPoint, input);


            // 모든 판정이 끝났을 때, 아래에서 이벤트를 발생시킨다.
            switch (interactionType)
            {
            case Stage_InteractionType_MOVE:
                Protagonist_MoveTo(Stage_RowColToPosition(stageInstance.protagonistPosition), Stage_RowColToPosition(interactionPoint));
                stageInstance.protagonistPosition = interactionPoint;
                if (stageInstance.gimmicksTrespassable[interactionPoint.y][interactionPoint.x].gimmickType != DynamicGimmick_Type_NONE &&
                    stageInstance.gimmicksTrespassable[interactionPoint.y][interactionPoint.x].onTrigger)
                    stageInstance.gimmicksTrespassable[interactionPoint.y][interactionPoint.x].onTrigger(&stageInstance.gimmicksTrespassable[interactionPoint.y][interactionPoint.x], interactionPoint, input);
                break;
            case Stage_InteractionType_PUSH:
                Protagonist_Push();
                if (stageInstance.gimmicks[interactionPoint.y][interactionPoint.x].onTrigger)
                    stageInstance.gimmicks[interactionPoint.y][interactionPoint.x].onTrigger(&stageInstance.gimmicks[interactionPoint.y][interactionPoint.x], interactionPoint, input);
                break;
            case Stage_InteractionType_PUSH_FUTILE:
                Protagonist_Push_Futile();
                break;
            default:
                break;
            }
        }
        break;
    }
    case Stage_State_LOADING:
    {
        stageInstance.eventElapsed += Time_Instance.deltaTime;
        switch (stageInstance.transitionProgress)
        {
        case 0:
            MovementHandler_EaseOutVector(&stageInstance.transitionImagePosition,
                GetVector2i_XY(GDIEngine_ResolutionW * 0.5, GDIEngine_ResolutionH * -2.5),
                GetVector2i_XY(GDIEngine_ResolutionW * 0.5, GDIEngine_ResolutionH * 0.5), Stage_transitionDuration * 0.4);

            stageInstance.eventElapsed = 0;
            stageInstance.transitionProgress++;
            break;
        case 1:
            if (stageInstance.eventElapsed > Stage_transitionDuration * 0.6)
            {
                MovementHandler_EaseOutVector(&stageInstance.transitionImagePosition,
                    GetVector2i_XY(GDIEngine_ResolutionW * 0.5, GDIEngine_ResolutionH * 0.5),
                    GetVector2i_XY(GDIEngine_ResolutionW * 0.5, GDIEngine_ResolutionH * -2.5), Stage_transitionDuration * 0.4);

                Stage_LoadStage(stageInstance.transitionNextStage);
                stageInstance.eventElapsed = 0;
                stageInstance.transitionProgress++;
            }
            break;
        case 2:
            if (stageInstance.eventElapsed > Stage_transitionDuration * 0.4)
            {
                stageInstance.state = Stage_State_PLAYING;
            }
            break;
        default:
            break;
        }
    }
    }
}

Stage_InteractionType Stage_GetInteractionType(Vector2i interactionPoint, Vector2i input)
{
    // 목표지점이 벽일때
    if (stageInstance.tileMatrix[interactionPoint.y][interactionPoint.x].tileType == TileType_WALL
        || stageInstance.tileMatrix[interactionPoint.y][interactionPoint.x].tileType == TileType_PATH_BUTWALL)
        return Stage_InteractionType_PUSH_FUTILE;

    // 목표지점이 블록일 때
    if (stageInstance.gimmicks[interactionPoint.y][interactionPoint.x].gimmickType != DynamicGimmick_Type_NONE)
    {
        if (stageInstance.gimmicks[interactionPoint.y][interactionPoint.x].isTrespassable)
            return Stage_InteractionType_MOVE;
        else if (stageInstance.gimmicks[interactionPoint.y][interactionPoint.x].gimmickState == DynamicGimmick_State_IDLE && stageInstance.gimmicks[interactionPoint.y][interactionPoint.x].isActive)
            return Stage_InteractionType_PUSH;
        else
            return Stage_InteractionType_PUSH_FUTILE;
    }
    return Stage_InteractionType_MOVE;
}
void Stage_Render(Vector2i position)
{
    if (g_isNowCutscene) return;

    if (stageInstance.backgroundFileName[0])
        GDI_DrawResizedBitmap(stageInstance.backgroundFileName, position, GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH));

    Vector2i drawRect = { stageInstance.tileSize,stageInstance.tileSize };
    for (int row = 0; row < stageInstance.height; ++row)
    {
        for (int col = 0; col < stageInstance.width; ++col)
        {
            Vector2i rowcol = { col,row };
            StaticTile_Render(&stageInstance.tileMatrix[row][col], Stage_RowColToPosition(rowcol), drawRect);
        }
        for (int col = 0; col < stageInstance.width; ++col)
        {
            Vector2i rowcol = { col,row };
            DynamicGimmick_Render(&stageInstance.gimmicksTrespassable[row][col], Stage_RowColToPosition(rowcol), drawRect);
        }
    }
    for (int row = 0; row < stageInstance.height; ++row)
    {
        for (int col = 0; col < stageInstance.width; ++col)
        {
            Vector2i rowcol = { col,row };
            DynamicGimmick_Render(&stageInstance.gimmicks[row][col], Stage_RowColToPosition(rowcol), drawRect);
        }
        for (int col = 0; col < stageInstance.width; ++col)
        {
            Vector2i rowcol = { col,row };
            if (Vector2i_Equals(stageInstance.protagonistPosition, rowcol))
                Protagonist_Render(Stage_RowColToPosition(stageInstance.protagonistPosition), drawRect);
        }
    }

    if (stageInstance.backgroundOuterFileName[0])
        GDI_DrawResizedBitmap(stageInstance.backgroundOuterFileName, position, GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH));

    if (stageInstance.state == Stage_State_LOADING)
        GDI_DrawResizedBitmap(transitionImage, stageInstance.transitionImagePosition, GetVector2i_XY(GDIEngine_ResolutionW, GDIEngine_ResolutionH));
}
//void Stage_ChangeToNextStage()
//{
//    Stage_ChangeStage(stageInstance.transitionNextStage);
//}
//void Stage_ChangeStage(LPCWSTR* filename)
//{
//    stageInstance.state = Stage_State_LOADING;
//    stageInstance.transitionProgress = 0;
//    stageInstance.transitionImagePosition = GetVector2i_XY(-99999999, 0);
//    stageInstance.transitionNextStage = filename;
//}

void Stage_ReloadStage()
{
    SoundPlay(SoundType_Effect_RETRY);
    stageInstance.state = Stage_State_PAUSE;
    ReloadTransition_Instance.onOverlap = Stage_ReloadStageFile;
    ReloadTransition_Emerge();
}
void Stage_ReloadStageFile()
{
    Stage_LoadStage(stageInstance.transitionNextStage);
}
void Stage_PlayStageBGM()
{
    SoundBGMchange(stageInstance.bgmIndex);
}
void Stage_LoadStage(LPCWSTR* filename)
{
    memset(&stageInstance, 0, sizeof(Stage));

    stageInstance.currentProgress = 0;
    stageInstance.protagonistPosition = Vector2i_Zero;
    Protagonist_Instance.state = Protagonist_State_Idle;
    Protagonist_Instance.facingVector = GetVector2i_XY(0, 1);
    stageInstance.transitionNextStage = filename;
    stageInstance.onStageClear = Stage_OnStageClearDefault;
    stageInstance.state = Stage_State_PLAYING;
    stageInstance.backgroundFileName[0] = 0;
    lstrcpyW(ReloadTransition_Instance.image, L"UI\\\\Reload\\\\Stage 1 Re.bmp");

    char FileInfo[100][100] = { 0 };
    FILE* fp = NULL;
    WCHAR filepath[100] = L"";
    lstrcatW(filepath, g_pathManager.m_contentPath);
    lstrcatW(filepath, defaultMapPath);
    lstrcatW(filepath, filename);
    fp = _wfopen(filepath, L"r");
    char c = 1;

    stageInstance.width = 0;
    stageInstance.height = 0;
    int row = 0;
    int col = 0;

    while (c != EOF)
    {
        c = fgetc(fp);
        // 목표 progress는 몇인지 입력을 받아온다.
        if (c == '$')
        {
            c = fgetc(fp);
            stageInstance.endProgress = c - '0';
            while ((c = fgetc(fp)) != '\n');
            continue;
        }
        // 재생할 배경음악의 인덱스
        if (c == '&')
        {
            c = fgetc(fp);
            stageInstance.bgmIndex = c - '0';
            while ((c = fgetc(fp)) != '\n');
            continue;
        }
        // 배경화면으로 쓸 파일
        if (c == '*')
        {
            int i = 0;
            while ((c = fgetc(fp)) != '\n')
                stageInstance.backgroundFileName[i++] = c;

            stageInstance.backgroundFileName[i] = 0;
            continue;
        }
        // 배경화면중 바깥 레이어로 쓸 파일
        if (c == '#')
        {
            int i = 0;
            while ((c = fgetc(fp)) != '\n')
                stageInstance.backgroundOuterFileName[i++] = c;

            stageInstance.backgroundOuterFileName[i] = 0;
            continue;
        }
        // 리로드 이미지로 쓸 파일
        if (c == '~')
        {
            int i = 0;
            while ((c = fgetc(fp)) != '\n')
                ReloadTransition_Instance.image[i++] = c;

            ReloadTransition_Instance.image[i] = 0;
            continue;
        }

        FileInfo[row][col] = c;

        if (c == '\n')
        {
            row++;
            stageInstance.height++;
            col = 0;
            continue;
        }

        col++;
        if (stageInstance.width < col)
            stageInstance.width = col;


    }
    ++row;
    fclose(fp);

    for (int row = 0; row < stageInstance.height; row += 2)
    {
        for (int col = 0; col < stageInstance.width; col += 2)
        {
        }
    }
    stageInstance.width /= 2;
    stageInstance.height++;
    stageInstance.height /= 2;
    // 여기서부턴 스테이지 구성의 영역

    for (int row = 0; row < stageInstance.height; row++)
        for (int col = 0; col < stageInstance.width; col++)
        {
            stageInstance.gimmicks[row][col].gimmickType = DynamicGimmick_Type_NONE;
            stageInstance.gimmicks[row][col].onTrigger = NULL;
            stageInstance.tileMatrix[row][col].tileType = TileType_NULL;
        }
    // 스테이지 정보 해석구간
    for (int row = 0; row < stageInstance.height; row++)
    {
        for (int col = 0; col < stageInstance.width; col++)
        {
            Stage_ProcessTileInfo(FileInfo, row, col);
        }
    }

    stageInstance.tileSize = 128;
    //stageInstance.tileSize = GDIEngine_ResolutionW * 0.8 / stageInstance.width;
    //if (stageInstance.tileSize > GDIEngine_ResolutionH * 0.8 / stageInstance.height)
    //    stageInstance.tileSize = GDIEngine_ResolutionH * 0.8 / stageInstance.height;

    // 여기까지 코드가 다 진행됐다면
    // stageInstance.tileMatrix에 타일의 정보가 모두 들어가야 하고,
    // stageInstance.width, height에 읽은 타일맵의 가로길이 세로길이가 들어가 있어야 한다.
}
Vector2i Stage_RowColToPosition(Vector2i rowCol)
{
    Vector2d deltaPosition;
    Vector2d centerRowCol;
    centerRowCol.x = (stageInstance.width - 1) / 2.0f;
    centerRowCol.y = (stageInstance.height - 1) / 2.0f;
    deltaPosition.x = (rowCol.x - centerRowCol.x) * stageInstance.tileSize;
    deltaPosition.y = (rowCol.y - centerRowCol.y) * stageInstance.tileSize;

    rowCol.x = deltaPosition.x + GDIEngine_center.x;
    rowCol.y = deltaPosition.y + GDIEngine_center.y;

    return rowCol;
}
BOOL Stage_isAccessible(Vector2i rowCol)
{
    if (rowCol.y >= stageInstance.height || rowCol.y < 0
        || rowCol.x >= stageInstance.width || rowCol.x < 0)
        return FALSE;

    if (stageInstance.tileMatrix[rowCol.y][rowCol.x].tileType == TileType_WALL ||
        stageInstance.tileMatrix[rowCol.y][rowCol.x].tileType == TileType_PATH_BUTWALL)
        return FALSE;

    if (stageInstance.gimmicks[rowCol.y][rowCol.x].gimmickType != DynamicGimmick_Type_NONE)
        return  stageInstance.gimmicks[rowCol.y][rowCol.x].isTrespassable;

    return TRUE;
}
void Stage_EnableGimmick(DynamicGimmick enabler)
{
    for (int i = 0; i < 10; i++)
    {
        if (enabler.quenchEnableTarget[i] == DynamicGimmick_Type_NONE)
            return;

        for (int row = 0; row < stageInstance.height; row++)
            for (int col = 0; col < stageInstance.width; col++)
                if (stageInstance.gimmicksHidden[row][col].gimmickType == enabler.quenchEnableTarget[i])
                {
                    // 해금되는 곳에 이미 기믹이 위치하고 있는 경우
                    if (stageInstance.gimmicks[row][col].gimmickType != DynamicGimmick_Type_NONE)
                    {
                        Vector2i delta[] = { {0,1},{0,-1},{1,0},{-1,0} };
                        for (int i = 0; i < 4; i++)
                        {
                            Vector2i dst = { col,row };
                            dst.x += delta->x;
                            dst.y += delta->y;
                            if (Stage_isAccessible(dst))
                            {
                                DynamicGimmick_Pushed1Block(&stageInstance.gimmicks[row][col], GetVector2i_XY(col, row), delta[i]);
                                break;
                            }
                        }
                    }
                    stageInstance.gimmicks[row][col] = stageInstance.gimmicksHidden[row][col];
                }
        for (int row = 0; row < stageInstance.height; row++)
            for (int col = 0; col < stageInstance.width; col++)
                if (stageInstance.gimmicks[row][col].gimmickType == enabler.quenchEnableTarget[i])
                {
                    stageInstance.gimmicks[row][col].isActive = TRUE;
                    DynamicGimmick_PopIn(&stageInstance.gimmicks[row][col]);
                }
    }
}
void Stage_ProcessTileInfo(char FileInfo[100][100], int row, int col)
{
    int fileRow = row * 2;
    int fileCol = col * 2;
    DynamicGimmick_Type gimmickType = DynamicGimmick_Type_NONE;
    stageInstance.tileMatrix[row][col].doesRender = TRUE;

    if (FileInfo[fileRow][fileCol] == ' ' && FileInfo[fileRow][fileCol + 1] == ' ' &&
        FileInfo[fileRow + 1][fileCol] == ' ' && FileInfo[fileRow + 1][fileCol + 1] == ' ')
    {
        stageInstance.tileMatrix[row][col].tileType = TileType_NULL;
    }

    if (FileInfo[fileRow][fileCol] == 'p' && FileInfo[fileRow][fileCol + 1] == 'p' &&
        FileInfo[fileRow + 1][fileCol] == 'p' && FileInfo[fileRow + 1][fileCol + 1] == 'p')
    {
        stageInstance.tileMatrix[row][col].tileType = TileType_PATH;
        if (!(FileInfo[fileRow][fileCol] == 'p' && FileInfo[fileRow][fileCol + 1] == 'p' &&
            FileInfo[fileRow + 1][fileCol] == 'p' && FileInfo[fileRow + 1][fileCol + 1] == 'p'))
            stageInstance.tileMatrix[row][col].doesRender = FALSE;
    }

    if (FileInfo[fileRow][fileCol] == 'w')
    {
        stageInstance.tileMatrix[row][col].tileType = TileType_WALL;
        if (!(FileInfo[fileRow][fileCol] == 'w' && FileInfo[fileRow][fileCol + 1] == 'w' &&
            FileInfo[fileRow + 1][fileCol] == 'w' && FileInfo[fileRow + 1][fileCol + 1] == 'w'))
            stageInstance.tileMatrix[row][col].doesRender = FALSE;

        if ((FileInfo[fileRow][fileCol] == 'w' && FileInfo[fileRow][fileCol + 1] == 'p' &&
            FileInfo[fileRow + 1][fileCol] == 'w' && FileInfo[fileRow + 1][fileCol + 1] == 'p'))
            stageInstance.tileMatrix[row][col].tileType = TileType_PATH_BUTWALL;
    }

    if (FileInfo[fileRow][fileCol] == 's')
    {
        stageInstance.tileMatrix[row][col].tileType = TileType_PATH;
        stageInstance.protagonistPosition.x = col;
        stageInstance.protagonistPosition.y = row;
        if (!(FileInfo[fileRow][fileCol] == 's' && FileInfo[fileRow][fileCol + 1] == 's' &&
            FileInfo[fileRow + 1][fileCol] == 's' && FileInfo[fileRow + 1][fileCol + 1] == 's'))
            stageInstance.tileMatrix[row][col].doesRender = FALSE;
    }

    // 기믹이 위치하고 있는가? 
    if (FileInfo[fileRow][fileCol] == 'D')
    {
        stageInstance.tileMatrix[row][col].tileType = TileType_PATH;

        if (FileInfo[fileRow + 1][fileCol] == 'g' && FileInfo[fileRow + 1][fileCol + 1] == 'e')
        {
            gimmickType = DynamicGimmick_Type_ESCAPE_GOOD;
            stageInstance.tileMatrix[row][col].doesRender = FALSE;
        }
        if (FileInfo[fileRow + 1][fileCol] == 'b' && FileInfo[fileRow + 1][fileCol + 1] == 'e')
        {
            gimmickType = DynamicGimmick_Type_ESCAPE_BAD;
            stageInstance.tileMatrix[row][col].doesRender = FALSE;
        }
        if (FileInfo[fileRow + 1][fileCol] == 'D' && FileInfo[fileRow + 1][fileCol + 1] == 'h')
            gimmickType = DynamicGimmick_Type_PIC_HEART_DST;
        if (FileInfo[fileRow + 1][fileCol] == 'P' && FileInfo[fileRow + 1][fileCol + 1] == 'h')
            gimmickType = DynamicGimmick_Type_PIC_HEART;

        if (FileInfo[fileRow + 1][fileCol] == 'K' && FileInfo[fileRow + 1][fileCol + 1] == 'w')
            gimmickType = DynamicGimmick_Type_Key_W;
        if (FileInfo[fileRow + 1][fileCol] == 'K' && FileInfo[fileRow + 1][fileCol + 1] == 'a')
            gimmickType = DynamicGimmick_Type_Key_A;
        if (FileInfo[fileRow + 1][fileCol] == 'K' && FileInfo[fileRow + 1][fileCol + 1] == 's')
            gimmickType = DynamicGimmick_Type_Key_S;
        if (FileInfo[fileRow + 1][fileCol] == 'K' && FileInfo[fileRow + 1][fileCol + 1] == 'd')
            gimmickType = DynamicGimmick_Type_Key_D;
        if (FileInfo[fileRow + 1][fileCol] == 'K' && FileInfo[fileRow + 1][fileCol + 1] == 'r')
            gimmickType = DynamicGimmick_Type_Key_R;
        if (FileInfo[fileRow + 1][fileCol] == 'K' && FileInfo[fileRow + 1][fileCol + 1] == 'E')
            gimmickType = DynamicGimmick_Type_Key_ESC;

        if (FileInfo[fileRow + 1][fileCol] == 'P' && FileInfo[fileRow + 1][fileCol + 1] == 'n')
            gimmickType = DynamicGimmick_Type_PIC_NOTE;
        if (FileInfo[fileRow + 1][fileCol] == 'P' && FileInfo[fileRow + 1][fileCol + 1] == 's')
            gimmickType = DynamicGimmick_Type_PIC_STRESH;
        if (FileInfo[fileRow + 1][fileCol] == 'P' && FileInfo[fileRow + 1][fileCol + 1] == 'p')
            gimmickType = DynamicGimmick_Type_PIC_FAMILY;
        if (FileInfo[fileRow + 1][fileCol] == 'D' && FileInfo[fileRow + 1][fileCol + 1] == 'n')
            gimmickType = DynamicGimmick_Type_PIC_NOTE_DST;
        if (FileInfo[fileRow + 1][fileCol] == 'D' && FileInfo[fileRow + 1][fileCol + 1] == 's')
            gimmickType = DynamicGimmick_Type_PIC_STRESH_DST;
        if (FileInfo[fileRow + 1][fileCol] == 'D' && FileInfo[fileRow + 1][fileCol + 1] == 'p')
            gimmickType = DynamicGimmick_Type_PIC_FAMILY_DST;
        if (FileInfo[fileRow + 1][fileCol] == 'h' && FileInfo[fileRow + 1][fileCol + 1] == 'h')
            gimmickType = DynamicGimmick_Type_HEART;
        if (FileInfo[fileRow + 1][fileCol] == 'b' && FileInfo[fileRow + 1][fileCol + 1] == 'b')
            gimmickType = DynamicGimmick_Type_BRIGHTBOY;

        if (FileInfo[fileRow + 1][fileCol] == 'c' && FileInfo[fileRow + 1][fileCol + 1] == 'c')
            gimmickType = DynamicGimmick_Type_CROSS;
        if (FileInfo[fileRow + 1][fileCol] == 's' && FileInfo[fileRow + 1][fileCol + 1] == 's')
            gimmickType = DynamicGimmick_Type_STAR;
        if (FileInfo[fileRow + 1][fileCol] == 't' && FileInfo[fileRow + 1][fileCol + 1] == 't')
            gimmickType = DynamicGimmick_Type_TRASH;
        if (FileInfo[fileRow + 1][fileCol] == 'P' && FileInfo[fileRow + 1][fileCol + 1] == 'm')
            gimmickType = DynamicGimmick_Type_PIC_MOM;
        if (FileInfo[fileRow + 1][fileCol] == 'P' && FileInfo[fileRow + 1][fileCol + 1] == 't')
            gimmickType = DynamicGimmick_Type_PIC_TRASH;
        if (FileInfo[fileRow + 1][fileCol] == 'P' && FileInfo[fileRow + 1][fileCol + 1] == 'c')
            gimmickType = DynamicGimmick_Type_PIC_CROSS;
        if (FileInfo[fileRow + 1][fileCol] == 'D' && FileInfo[fileRow + 1][fileCol + 1] == 'm')
            gimmickType = DynamicGimmick_Type_PIC_MOM_DST;
        if (FileInfo[fileRow + 1][fileCol] == 'D' && FileInfo[fileRow + 1][fileCol + 1] == 't')
            gimmickType = DynamicGimmick_Type_PIC_TRASH_DST;
        if (FileInfo[fileRow + 1][fileCol] == 'D' && FileInfo[fileRow + 1][fileCol + 1] == 'c')
            gimmickType = DynamicGimmick_Type_PIC_CROSS_DST;

        if (FileInfo[fileRow + 1][fileCol] == 'c' && FileInfo[fileRow + 1][fileCol + 1] == 'c')
            gimmickType = DynamicGimmick_Type_CROSS;
        if (FileInfo[fileRow + 1][fileCol] == 'P' && FileInfo[fileRow + 1][fileCol + 1] == 'c')
            gimmickType = DynamicGimmick_Type_PIC_CROSS;
        if (FileInfo[fileRow + 1][fileCol] == 'p' && FileInfo[fileRow + 1][fileCol + 1] == 'b')
            gimmickType = DynamicGimmick_Type_PHONEBOOK;
        if (FileInfo[fileRow + 1][fileCol] == 'p' && FileInfo[fileRow + 1][fileCol + 1] == 'p')
            gimmickType = DynamicGimmick_Type_PHONE;
        if (FileInfo[fileRow + 1][fileCol] == 'r' && FileInfo[fileRow + 1][fileCol + 1] == 'c')
            gimmickType = DynamicGimmick_Type_REMOCON;
        if (FileInfo[fileRow + 1][fileCol] == 't' && FileInfo[fileRow + 1][fileCol + 1] == 'v')
            gimmickType = DynamicGimmick_Type_TV;
        if (FileInfo[fileRow + 1][fileCol] == 'a' && FileInfo[fileRow + 1][fileCol + 1] == 'l')
            gimmickType = DynamicGimmick_Type_ALBUM;
        if (FileInfo[fileRow + 1][fileCol] == 'f' && FileInfo[fileRow + 1][fileCol + 1] == 'f')
            gimmickType = DynamicGimmick_Type_FLOWER;
        if (FileInfo[fileRow + 1][fileCol] == 'p' && FileInfo[fileRow + 1][fileCol + 1] == 'c')
            gimmickType = DynamicGimmick_Type_PENCIL;
        if (FileInfo[fileRow + 1][fileCol] == 't' && FileInfo[fileRow + 1][fileCol + 1] == 'b')
            gimmickType = DynamicGimmick_Type_TEXTBOOK;
        if (FileInfo[fileRow + 1][fileCol] == 'P' && FileInfo[fileRow + 1][fileCol + 1] == 'f')
            gimmickType = DynamicGimmick_Type_PIC_FLOWER;
        if (FileInfo[fileRow + 1][fileCol] == 'P' && FileInfo[fileRow + 1][fileCol + 1] == 'b')
            gimmickType = DynamicGimmick_Type_PIC_BEAR;
        if (FileInfo[fileRow + 1][fileCol] == 'D' && FileInfo[fileRow + 1][fileCol + 1] == 'b')
            gimmickType = DynamicGimmick_Type_PIC_BEAR_DST;
        if (FileInfo[fileRow + 1][fileCol] == 'D' && FileInfo[fileRow + 1][fileCol + 1] == 'f')
            gimmickType = DynamicGimmick_Type_PIC_FLOWER_DST;

        DynamicGimmick result = DynamicGimmick_GetGimmickPreset(gimmickType);
        //if (FileInfo[fileRow][fileCol + 1] == 'H')
        //    stageInstance.gimmicksHidden[row][col] = result;
        //else 
        if (result.isTrespassable)
            stageInstance.gimmicksTrespassable[row][col] = result;
        else
            stageInstance.gimmicks[row][col] = result;
    }

    // 목표지점 설정
    if (FileInfo[fileRow][fileCol] == 'd' && FileInfo[fileRow][fileCol + 1] == 'b')
    {
        stageInstance.tileMatrix[row][col].tileType = TileType_PATH;
        stageInstance.gimmicksDestination[row][col] = DynamicGimmick_Type_PHONEBOOK;
    }
}
void Stage_IncrementProgress()
{
    stageInstance.currentProgress++;
    if (stageInstance.currentProgress >= stageInstance.endProgress)
        stageInstance.onStageClear();
}
void Stage_OnStageClearDefault()
{
    Cutscene_LoadCutscene();
}