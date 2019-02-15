// dmDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Student.h"
#include "dmDlg.h"
#include "afxdialogex.h"


// dmDlg 对话框

IMPLEMENT_DYNAMIC(dmDlg, CDialogEx)

dmDlg::dmDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(dmDlg::IDD, pParent)
	, mStuId(_T(""))
	, mStuName(_T(""))
{

}

dmDlg::~dmDlg()
{
}

void dmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, mStuId);
	DDX_Text(pDX, IDC_EDIT_NAME, mStuName);
}


BEGIN_MESSAGE_MAP(dmDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &dmDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// dmDlg 消息处理程序

HBRUSH dmDlg::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG) {
		CBrush *brush = new CBrush(RGB(255, 255, 255));
		return (HBRUSH)(brush->m_hObject);
	}
	else if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkMode(TRANSPARENT);//设置背景透明
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}
void dmDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str1, str2;
	GetDlgItem(IDC_EDIT_ID)->GetWindowText(str1);
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(str2);
	if (strlen(str1) != 0 && strlen(str2) != 0)
		CDialogEx::OnOK();
	else
		MessageBox("输入学号姓名！");
}
