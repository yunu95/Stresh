#pragma once

struct UIWindow;
//typedef struct UIWindow;
typedef struct
{
    struct UIWindow* current;
}
UIWndSelector;

UIWndSelector UIWndSelector_Instance;
void UIWndSelector_SelectWindow(struct UIWindow* target);
