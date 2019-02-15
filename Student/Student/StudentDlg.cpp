
// StudentDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Student.h"
#include "StudentDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "dmDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
//	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

//CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
//{
//}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CStudentDlg 对话框



CStudentDlg::CStudentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STUDENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStudentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStudentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_MYSOCK, MyMsgHandle)
	ON_MESSAGE(WM_SHOWTASK, onShowTask)       //消息映射
	ON_BN_CLICKED(ID_BTN_MIN, &CStudentDlg::OnBnClickedBtnMin)
END_MESSAGE_MAP()


// CStudentDlg 消息处理程序

BOOL CStudentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//对话框置顶
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//设置开机自启动
	SetAutoRun(TRUE);
	WSADATA wsaData;
	WORD wRequestVersion;
	wRequestVersion = MAKEWORD(1, 1);
	if (WSAStartup(wRequestVersion, &wsaData))
		return FALSE;

	//广播初始化
	if (!broadinit())
		broadsock = NULL;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CStudentDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CStudentDlg::OnPaint()
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
HCURSOR CStudentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//界面颜色设置
HBRUSH CStudentDlg::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
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

//广播初始化
BOOL CStudentDlg::broadinit(void)
{

	BOOL bOpVal = TRUE;	//选项的值
	int alen = sizeof(SOCKADDR_IN);
	if ((broadsock = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)//创建套接字
	{
		WSACleanup();
		return FALSE;
	}
	memset(&sockaddr, 0, alen);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(S_PORT);
	sockaddr.sin_addr.s_addr = INADDR_ANY;//设置地址为广播地址
	if (bind(broadsock, (LPSOCKADDR)&sockaddr, alen))//绑定地址到套接字
	{
		WSACleanup();
		return FALSE;
	}
	//设置套接字，定义套接口选项 SO_BROADCAST允许套接口传送广播信息
	if (setsockopt(broadsock, SOL_SOCKET, SO_BROADCAST, (char FAR*)&bOpVal, sizeof(bOpVal)) == SOCKET_ERROR)
	{
		WSACleanup();
		return FALSE;
	}
	//把broadcast套接字上发生的事件交给对话框的窗口过程处理，
	//接收的消息是自己定义的消息WM_MYSOCK
	if (WSAAsyncSelect(broadsock, this->m_hWnd, WM_MYSOCK, FD_READ | FD_CONNECT | FD_ACCEPT | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
	{
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

//广播接受报文函数
BOOL CStudentDlg::broadrecv(CString &strMsg, int* command)
{

	SOCKADDR_IN sourceadd;
	int alen = sizeof(SOCKADDR_IN);
	char buf[BUFLEN];
	int n;
	memset(buf, 0, BUFLEN);
	strMsg.Empty();

	//从教师端接收广播消息
	//内容放到接收缓冲区，sourceadd指向装有原地址的缓冲区
	//如果正确接收字节，返回收到的字节数，失败返回-1
	if ((n = recvfrom(broadsock, buf, BUFLEN - 1, 0, (LPSOCKADDR)&sourceadd, &alen)) > 0)
	{
		teaaddr = sourceadd;
	}

	//n=recvfrom(broadsock,buf,BUFLEN-1,0,(LPSOCKADDR)&sourceadd,&alen);
	while (n > 0)
	{
		PMES_PACKAGE bag;
		bag = (PMES_PACKAGE)&buf;
		*command = ntohs(bag->command);//提取包中的命令，并转为主机字节序
		strMsg += bag->data;//提取包中的数据，即字符串信息
		memset(buf, 0, BUFLEN);
		alen = sizeof(SOCKADDR_IN);
		n = recvfrom(broadsock, buf, BUFLEN - 1, 0, (LPSOCKADDR)&sourceadd, &alen);
	}
	return TRUE;
}

//消息处理函数
LRESULT CStudentDlg::MyMsgHandle(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTEVENT(lParam) == FD_READ)
		receivemsg();
	return 0;
}

//结束套接字连接
void CStudentDlg::OnDestroy()
{

	if (broadsock != NULL)
	{
		closesocket(broadsock);
		WSACleanup();
	}
	CDialog::OnDestroy();
}

//消息处理函数转到broadrecv，跳转Judge函数
void CStudentDlg::receivemsg()
{
	CString strMsg;
	int command = 0;
	if (broadrecv(strMsg, &command)) {
		opFun(strMsg, command);
	}
}

//命令处理函数
void CStudentDlg::opFun(CString strMsg, int command) {

	//收到点名请求，打开点名窗口，确认后返回点名应答
	if (command == MES_QD1) {
		dmDlg mdlg;
		if (IDOK == mdlg.DoModal()) {
			sendMsg(mdlg.mStuId + " " + mdlg.mStuName, MES_QD2);//发送学号、姓名
		}
	}
	//收到心跳请求，获取本地主机名，返回心跳应答
	else if (command == MES_KEEP1) {
		//MessageBox("收到心跳报文");
		char localname[128];
		gethostname(localname, 128);	//获得主机名 
		sendMsg(localname, MES_KEEP2);//发送心跳消息
	}
	//收到消息报文
	else if (command == MES_MSG) {
		MessageBox(strMsg, _T("教师消息："), MB_OK);
	}
	//收到关机命令
	else if (command == MES_SHUT) {
		closeWin();
	}
	//收到重启命令重启
	else if (command == MES_RESTART) {
		rebootWin();
	}
	else;

}

//发送消息
void CStudentDlg::sendMsg(CString strMsg, int command) {
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	int alen = sizeof(SOCKADDR_IN);
	PMES_PACKAGE bag;

	/*if (sndBuf != NULL)
	{
		free(sndBuf);
		sndBuf = NULL;
	}*/
	sndBuf = (char*)malloc(BUFLEN);
	ZeroMemory(sndBuf, BUFLEN);

	bag = (PMES_PACKAGE)sndBuf;
	int len = 2;
	bag->command = htons(command);
	char* point;
	point = bag->data;
	sprintf_s(point, strlen(strMsg) + 1, "%s", strMsg);
	len += strlen(strMsg) + 1;
	sendto(sock, sndBuf, len, 0, (LPSOCKADDR)&teaaddr, alen);//发送缓冲区的内容到套接字地址中
	closesocket(sock);
}



//关机
void CStudentDlg::closeWin()
{
	MessageBox("关机");
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	// Get a token for this process.  
	//获取进程的令牌句柄
	/*第一个参数 进程句柄(当前进程为GetCurrentProcess()为参数);
	第二个参数 访问令牌特权;
	第三个参数 返回的参数 就是AdjustTokenPrivileges的第一个参数
	*/
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		AfxMessageBox("OpenProcessToken Error!");
		return;
	}
	// 查看系统权限的特权值 NULL：本地系统 ，特权名字，返回信息到一个LUID结构体里
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1; // 修改的特权数目 
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	// Get the shutdown privilege for this process. 
	/*
	第一个参数为OpenProcessToken第三个指针参数传出的句柄值
	第二个参数为是否禁用所有所有的特权(这里填false)
	第三个参数为新的TOKEN_PRIVILEGES的特权结构体指针
	第四个参数是上面结构体的字节长度(sizeof)
	第五个参数是 接受原先的特权的结构体
	第六个参数也是这个结构体的字节长度的指针
	*/
	AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, NULL);
	if (GetLastError() != ERROR_SUCCESS)
	{
		AfxMessageBox("关机失败");
		return;
	}  // Shut down the system and force all applications to close.  
	ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0); //重启只需要把EWX_SHUTDOWN改为EWX_REBOOT
}


//重启
void CStudentDlg::rebootWin()
{
	// TODO: 在此处添加实现代码.
	MessageBox("重启");
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	// Get a token for this process.  
	//打开当前进程
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		AfxMessageBox("OpenProcessToken Error!");
		return;
	}
	// 函数查看系统权限的特权值，返回信息到一个LUID结构体里。NULL：本地系统 
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1; // one privilege to set  
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	// Get the shutdown privilege for this process.  
	AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, NULL);
	if (GetLastError() != ERROR_SUCCESS)
	{
		AfxMessageBox("重启失败");
		return;
	}  // Shut down the system and force all applications to close.  
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
}


/*学生端开机自启动*/
void CStudentDlg::SetAutoRun(BOOL bAutoRun)
{
	MessageBox("开机自启动");
	HKEY hKey;//注册表
	CString strRegPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";//找到系统的启动项
	if (bAutoRun)
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) //打开启动项key     
		{
			TCHAR szModule[_MAX_PATH];
			GetModuleFileName(NULL, szModule, _MAX_PATH);//得到本程序自身的全路径
			//在注册表中添加一个子Key,并设置值
			RegSetValueEx(hKey, "Tray", 0, REG_SZ, (const BYTE*)(LPCSTR)szModule, strlen(szModule));
			RegCloseKey(hKey); //关闭注册表
		}
		else
		{
			AfxMessageBox("系统参数错误,不能随系统启动");
		}
	}
}
/*点击最小化托盘时的动作*/
LRESULT CStudentDlg::onShowTask(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
		return 1;
	switch (lParam){
		case WM_LBUTTONDBLCLK://双击左键的处理 
		{
			this->ShowWindow(SW_SHOW);
			
		}
		break;
	}
	return 0;
}

//最小化按钮
void CStudentDlg::OnBnClickedBtnMin()
{
	// TODO: 在此添加控件通知处理程序代码
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK;//自定义的消息名称 
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	strcpy_s(nid.szTip, "计算机实验室学生端");    //信息提示条
	Shell_NotifyIcon(NIM_ADD, &nid);    //在托盘区添加图标 
	ShowWindow(SW_HIDE);    //隐藏主窗口
}
