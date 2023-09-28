#include "UIWndSelector.h"
#include "UIWindow.h"

void UIWndSelector_SelectWindow(struct UIWindow* target)
{
    UIWndSelector_Instance.current = target;
}
