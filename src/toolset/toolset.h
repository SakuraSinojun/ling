// toolset.h : toolset Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CtoolsetApp:
// �йش����ʵ�֣������ toolset.cpp
//

class CtoolsetApp : public CWinApp
{
public:
	CtoolsetApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CtoolsetApp theApp;