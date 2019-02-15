#pragma once


// dmDlg 对话框

class dmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(dmDlg)

public:
	dmDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~dmDlg();

	// 对话框数据
	enum { IDD = IDD_DM};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HBRUSH dmDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	CString mStuId;
	CString mStuName;
	afx_msg void OnBnClickedOk();
};
