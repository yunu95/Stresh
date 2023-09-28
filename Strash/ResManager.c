#include "ResManager.h"
#include "PathManager.h"

void ResManager_Init()
{
	memset(&g_resManager, NULL, sizeof(ResManager));
	for (int i = 0; i < HASHSIZE; i++)
	{
		//g_resManager.m_myHash[i].m_texture->m_res.m_RelativePath = NULL;
	}
}

void ResManager_LoadTexture(Texture** _pTex, LPCTSTR _strRelativePath)
{
    *_pTex = ResManager_FindTexture(_strRelativePath);
    if (*_pTex != NULL)
    {
        return;
    }
    wchar_t _strFilePath[200] = { NULL };
    wcscat(_strFilePath, g_pathManager.m_contentPath);
    wcscat(_strFilePath, _strRelativePath);

    *_pTex = (Texture*)malloc(sizeof(Texture));
    Texture_Load(*_pTex, _strFilePath);

    wcscpy((*_pTex)->m_res.m_RelativePath, _strRelativePath);
    //= ;


    int _adress = MyHash_Hash(_strRelativePath);
    while (g_resManager.m_myHash[_adress].m_texture && wcscmp(g_resManager.m_myHash[_adress].m_texture->m_res.m_RelativePath, _strRelativePath) != 0)
        _adress = ++_adress % 1000;

    g_resManager.m_myHash[_adress].m_texture = *_pTex;
}

Texture* ResManager_FindTexture(LPCTSTR _strKey)
{
    int _adress = MyHash_Hash(_strKey);
    //0x00000096f74ff4c0 L"texture\\Sprite Sheet_Standing_Right.bmp"
    //0x00000096f74ff4c0 L"texture\\Sprite Sheet_Running_Back.bmp"
    while (g_resManager.m_myHash[_adress].m_texture)
    {
        if (wcscmp(g_resManager.m_myHash[_adress].m_texture->m_res.m_RelativePath, _strKey) == 0)
            return g_resManager.m_myHash[_adress].m_texture;
        else
            _adress = ++_adress % 1000;
    }
    /*if (g_resManager.m_myHash[_adress].m_texture == NULL)
    {
        return NULL;
    }*/
    return NULL;
    //return g_resManager.m_myHash[_adress].m_texture;
}
