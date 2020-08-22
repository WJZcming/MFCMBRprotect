
// MFCMBRprotectDlg.cpp : ʵ���ļ�
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
// CMFCMBRprotectDlg �Ի���



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


// CMFCMBRprotectDlg ��Ϣ�������

BOOL CMFCMBRprotectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCMBRprotectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCMBRprotectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCMBRprotectDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	nid.uCallbackMessage = WM_SHOWTASK;//�Զ������Ϣ����
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy(nid.szTip, _T("��ҳ(�һ��ر�)")); //��Ϣ��ʾ��
	Shell_NotifyIcon(NIM_ADD, &nid); //�����������ͼ��
}
//wParam���յ���ͼ���ID����lParam���յ���������Ϊ
LRESULT CMFCMBRprotectDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
		return 1;
	switch (lParam)
	{
	case WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա�
	{
		if (MessageBoxA(0, "��ȷ��Ҫ�رգ�", "tip", MB_OKCANCEL) == IDOK)
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
	nid.uCallbackMessage = WM_SHOWTASK; //�Զ������Ϣ����
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy(nid.szTip, _T("��ҳ(�һ��ر�)")); //��Ϣ��ʾ��Ϊ���ƻ��������ѡ�
	Shell_NotifyIcon(NIM_DELETE, &nid); //��������ɾ��ͼ��
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	LRESULT ret = CDialogEx::OnNcHitTest(point);
	return (ret == HTCLIENT) ? HTCAPTION : ret;

}


void CMFCMBRprotectDlg::OnDestroy()
{
	CDialogEx::OnDestroy();


	// TODO:  �ڴ˴������Ϣ����������
}


void CMFCMBRprotectDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	lpwndpos->flags &= ~SWP_SHOWWINDOW;
	CDialogEx::OnWindowPosChanging(lpwndpos);

	// TODO:  �ڴ˴������Ϣ����������
}
