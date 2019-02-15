
// TeacherDlg.h: 头文件
//


#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib") 
#include "header.h"
#include <string>
using namespace std;

#define WM_MYSOCK (WM_USER+1)//自定义套接字处理函数
#define WM_SHOWTASK (WM_USER+2)//自定义任务最小化消息
#define T_PORT 9001
#define S_PORT 9002
#define BUFLEN	1024

// CTeacherDlg 对话框
class CTeacherDlg : public CDialogEx
{
// 构造
public:
	CTeacherDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据

	enum { IDD = IDD_TEACHER_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg LRESULT MyMsgHandle(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	HBRUSH CTeacherDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	CListBox m_TalkMessage;
	NOTIFYICONDATA m_nid;
	int m_nIndex;
	int countStu;
	int countPc;
	CListCtrl m_OnlineHost;
	BOOL broadinit(void);
	void CTeacherDlg::RefreshControl(UINT uCtrlID);
	BOOL broadsend(CString strMsg, int command);
	BOOL broadrecv(CString &strMsg, int* command);
	void receivemsg();
	void opFun(CString strMsg, int command);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	LRESULT onShowTask(WPARAM wParam, LPARAM lParam);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	SOCKADDR_IN broadaddr;
	SOCKET broadsock;
	SOCKET sock;
	char *rcvBuf;				//接收缓存
	char *sndBuf;				//发送缓存
public:
	afx_msg void OnBnClickedSendmsg();
	void sendMsg(CString strMsg, int command, CString target_ip);
	afx_msg void OnBnClickedBroadcast();	
	afx_msg void OnBnClickedShut();
	afx_msg void OnBnClickedRelod();
	afx_msg void OnBnClickedRollcallbtn();
	
};
