#pragma once
#include "Texture.h"
#include "MyHash.h"

typedef struct _tagResManager
{
	MyHash m_myHash[HASHSIZE];
}ResManager;

ResManager g_resManager;

void ResManager_Init();
void ResManager_LoadTexture(Texture** _pTex, LPCTSTR _strRelativePath);
Texture* ResManager_FindTexture(LPCTSTR _strKey);