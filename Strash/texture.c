#include <assert.h>
#include "GDIEngine.h"
#include "texture.h"

void Texture_Load(Texture* _texture, LPCTSTR _strFilepath)
{
	_texture->m_hBit = (HBITMAP)LoadImage(NULL, _strFilepath, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	_texture->m_hDC = CreateCompatibleDC(g_hScreenDC);

	HBITMAP hPrevBit = (HBITMAP)SelectObject(_texture->m_hDC, _texture->m_hBit);
	DeleteObject(hPrevBit);

	GetObject(_texture->m_hBit, sizeof(BITMAP), &_texture->m_bitInfo);

	assert(_texture->m_hBit);
}
