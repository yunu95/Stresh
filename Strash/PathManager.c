#include "PathManager.h"

void PathManager_Init()
{
    GetCurrentDirectory(255, (g_pathManager.m_contentPath));

    int len = wcslen(g_pathManager.m_contentPath);

    /*for (int i = len - 1; i >= 0; --i)
    {
        if (g_pathManager.m_contentPath[i] == '\\')
        {
            g_pathManager.m_contentPath[i] = '\0';
            break;
        }
    }*/

    wcscat_s(g_pathManager.m_contentPath, 255, L"\\content\\");
    //g_pathManager.m_contentPath[0] = 0;
    //wcscpy_s(g_pathManager.m_contentPath, 255, L"./content/");
}
