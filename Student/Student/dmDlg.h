#pragma once


// dmDlg �Ի���

class dmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(dmDlg)

public:
	dmDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~dmDlg();

	// �Ի�������
	enum { IDD = IDD_DM};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	HBRUSH dmDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	CString mStuId;
	CString mStuName;
	afx_msg void OnBnClickedOk();
};
