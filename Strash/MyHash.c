#include "MyHash.h"
#include "ResManager.h"

int MyHash_Hash(LPCTSTR _key)
{
    unsigned int result = 0;
    int i = 0;
    while (_key[i] != '\0')
    {
        result += _key[i];
        result *= 2;
        ++i;
    }
    /*while (g_resManager.m_myHash[result].m_texture && wcscmp(g_resManager.m_myHash[result].m_texture->m_res.m_RelativePath, _key) != 0)
    {
        ++result;
        result %= HASHSIZE;
    }*/
    return result %= HASHSIZE;
}
