
// MFCMBRprotect.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCMBRprotectApp: 
// �йش����ʵ�֣������ MFCMBRprotect.cpp
//

class CMFCMBRprotectApp : public CWinApp
{
public:
	CMFCMBRprotectApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCMBRprotectApp theApp;