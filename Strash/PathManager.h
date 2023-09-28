#pragma once
#include <Windows.h>

typedef struct _tagPathManager
{
	wchar_t m_contentPath[255];
}PathManager;

PathManager g_pathManager;

void PathManager_Init();