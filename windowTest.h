
// windowTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CwindowTestApp:
// �йش����ʵ�֣������ windowTest.cpp
//

class CwindowTestApp : public CWinApp
{
public:
	CwindowTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	afx_msg void HSV();
};

extern CwindowTestApp theApp;