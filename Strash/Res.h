#pragma once
#include <Windows.h>

typedef struct _tagRes
{
	LPCTSTR m_key;		// 리소스 키
	WCHAR m_RelativePath[100];
	//LPCTSTR m_RelativePath;	// 리소스 상대경로
}Res;

