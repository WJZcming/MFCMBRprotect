
// MFCMBRprotectDlg.cpp : 实现文件
//
#include"resource.h"
#include "stdafx.h"
#include "MFCMBRprotect.h"
#include "MFCMBRprotectDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include<winioctl.h>
#include <cstdio>
using namespace std;
DWORD dwBytesReturned;
HANDLE hDevice;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE hMutex = NULL;
// CMFCMBRprotectDlg 对话框



CMFCMBRprotectDlg::CMFCMBRprotectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCMBRprotectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCMBRprotectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCMBRprotectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCMBRprotectDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_SHOWTASK, OnShowTask)
	ON_WM_SYSCOMMAND()
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()


// CMFCMBRprotectDlg 消息处理程序

BOOL CMFCMBRprotectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	TCHAR* lpszName = _T("zcmingnbq");
	hMutex = ::CreateMutex(NULL, FALSE, lpszName);
	DWORD dwRet = ::GetLastError();
	if (hMutex)
	{
		if (ERROR_ALREADY_EXISTS == dwRet)
		{
			CloseHandle(hMutex);
			MessageBoxA(0, "runing", "tip", 0);
			exit(0);
			return FALSE;
		}
	}
	hDevice = CreateFile
		(
		_T("\\\\.\\PHYSICALDRIVE0"),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
		);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(0, "wrong", "tip", 0);
		exit(0);
	}
	DeviceIoControl
		(
		hDevice,
		FSCTL_LOCK_VOLUME,
		NULL,
		0,
		NULL,
		0,
		&dwBytesReturned,
		NULL
		);
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	ToTray();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCMBRprotectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCMBRprotectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCMBRprotectDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	DeviceIoControl
		(
		hDevice,
		FSCTL_UNLOCK_VOLUME,
		NULL,
		0,
		NULL,
		0,
		&dwBytesReturned,
		NULL
		);
	CloseHandle(hDevice);
	exit(0);
}


void CMFCMBRprotectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnSysCommand(nID, lParam);
}
void CMFCMBRprotectDlg::ToTray()
{
	//USES_CONVERSION;
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK;//自定义的消息名称
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy(nid.szTip, _T("主页(右击关闭)")); //信息提示条
	Shell_NotifyIcon(NIM_ADD, &nid); //在托盘区添加图标
}
//wParam接收的是图标的ID，而lParam接收的是鼠标的行为
LRESULT CMFCMBRprotectDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
		return 1;
	switch (lParam)
	{
	case WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个“关闭”
	{
		if (MessageBoxA(0, "您确定要关闭？", "tip", MB_OKCANCEL) == IDOK)
		{
			DeleteTray();
		}
	} break;
	default:
		break;
	}
	return 0;
}
void CMFCMBRprotectDlg::DeleteTray()
{
	//USES_CONVERSION;
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK; //自定义的消息名称
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy(nid.szTip, _T("主页(右击关闭)")); //信息提示条为“计划任务提醒”
	Shell_NotifyIcon(NIM_DELETE, &nid); //在托盘区删除图标
	::CloseHandle(hMutex);
	DeviceIoControl
		(
		hDevice,
		FSCTL_UNLOCK_VOLUME,
		NULL,
		0,
		NULL,
		0,
		&dwBytesReturned,
		NULL
		);
	CloseHandle(hDevice);
	exit(0);
}


LRESULT CMFCMBRprotectDlg::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	LRESULT ret = CDialogEx::OnNcHitTest(point);
	return (ret == HTCLIENT) ? HTCAPTION : ret;

}


void CMFCMBRprotectDlg::OnDestroy()
{
	CDialogEx::OnDestroy();


	// TODO:  在此处添加消息处理程序代码
}


void CMFCMBRprotectDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	lpwndpos->flags &= ~SWP_SHOWWINDOW;
	CDialogEx::OnWindowPosChanging(lpwndpos);

	// TODO:  在此处添加消息处理程序代码
}
