
// windowTestDlg.h : ͷ�ļ�
//

#pragma once


// CwindowTestDlg �Ի���
class CwindowTestDlg : public CDialogEx
{
// ����
public:
	CwindowTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WINDOWTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMfcmenubutton1();
	afx_msg void On32771();
	afx_msg void On32774();
	afx_msg void canny();
	afx_msg void LaLace();
	afx_msg void MaximumConnectedarea();
	afx_msg void siftStitching();
	afx_msg void Fengshuilin(CCmdUI *pCmdUI);
	afx_msg void weather();
	afx_msg void Color();
	afx_msg void weather2();
	afx_msg void BBlocation();
	afx_msg void Median_filter();
	afx_msg void hsv_color();
	afx_msg void lunkuo();
};
