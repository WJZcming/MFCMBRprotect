
// MFCMBRprotectDlg.h : ͷ�ļ�
//

#pragma once


// CMFCMBRprotectDlg �Ի���
class CMFCMBRprotectDlg : public CDialogEx
{
// ����
public:
	CMFCMBRprotectDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCMBRPROTECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void ToTray();
	afx_msg void DeleteTray();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};
