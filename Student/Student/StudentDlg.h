
// StudentDlg.h: 头文件
//

#pragma once
#include "winsock.h"
#pragma comment(lib,"ws2_32.lib")
#include "header.h"
#include "string.h"

#define WM_MYSOCK (WM_USER+1)//自定义消息套接字
#define WM_SHOWTASK (WM_USER+2)//自定义消息最小化
#define T_PORT 9001
#define S_PORT 9002
#define BUFLEN	1024

// CStudentDlg 对话框
class CStudentDlg : public CDialogEx
{
// 构造
public:
	CStudentDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STUDENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	HBRUSH CStudentDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT MyMsgHandle(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
	SOCKADDR_IN sockaddr;
	SOCKADDR_IN teaaddr;		//教师端主机地址
	int iTimeout;		//超时时间
	SOCKET sock;
	SOCKET broadsock;
	char *rcvBuf;				//接收缓存
	char *sndBuf;				//发送缓存
	LRESULT onShowTask(WPARAM wParam, LPARAM lParam);
	BOOL broadinit(void);
	void SetAutoRun(BOOL bAutoRun);
	BOOL broadrecv(CString &strMsg, int* command);
	void receivemsg();
	void opFun(CString strMsg, int command);
	void sendMsg(CString strMsg, int command);
	CString m_strMsg;
public:
	void closeWin();
	void rebootWin();
	afx_msg void OnBnClickedBtnMin();
};
