
// TeacherDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Teacher.h"
#include "TeacherDlg.h"
#include "afxdialogex.h"
#include "resource.h"


#include <windows.h> 
#include"header.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include<unordered_map>  
using namespace std;
struct string_less : public binary_function<const string, const string, bool>
{
public:
	result_type operator()(const first_argument_type& _Left, const second_argument_type& _Right) const
	{
		return(_Left.compare(_Right) < 0 ? true : false);
	}
};
typedef struct row
{
	int afk;
	int nRow;
}LISTROW;
unordered_map<string, LISTROW, hash_compare<string, string_less> > onlineHash;
unordered_map<string, LISTROW, hash_compare<string, string_less>>::iterator it;
CString a;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
	
public:
	CAboutDlg();

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

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


CTeacherDlg::CTeacherDlg(CWnd* pParent /*=null*/)
	: CDialogEx(IDD_TEACHER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTeacherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ComputerList, m_OnlineHost);
	DDX_Control(pDX, IDC_MSG_LIST, m_TalkMessage);
}



BEGIN_MESSAGE_MAP(CTeacherDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_MYSOCK, MyMsgHandle)//将消息映射到消息处理函数
	ON_MESSAGE(WM_SHOWTASK, onShowTask)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SendMSG, &CTeacherDlg::OnBnClickedSendmsg)
	ON_BN_CLICKED(IDC_Broadcast, &CTeacherDlg::OnBnClickedBroadcast)
	ON_BN_CLICKED(IDC_Shut, &CTeacherDlg::OnBnClickedShut)
	ON_BN_CLICKED(IDC_Relod, &CTeacherDlg::OnBnClickedRelod)
	ON_BN_CLICKED(IDC_RollCallBTN, &CTeacherDlg::OnBnClickedRollcallbtn)
END_MESSAGE_MAP()


// CTeacherDlg 消息处理程序

BOOL CTeacherDlg::OnInitDialog()
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

	WSADATA wsaData;
	WORD wRequestVersion;
	wRequestVersion = MAKEWORD(1, 1);
	if (WSAStartup(wRequestVersion, &wsaData))
		return FALSE;

	if (!broadinit())//广播初始化
		broadsock = NULL;
	m_OnlineHost.InsertColumn(0, _T("主机列表"), LVCFMT_LEFT, 150);//插入列
	m_OnlineHost.InsertColumn(1, _T("   学号     |   姓名   "), LVCFMT_LEFT, 150);//插入列

	m_OnlineHost.SetExtendedStyle(m_OnlineHost.GetExtendedStyle() | LVS_EX_FULLROWSELECT);//给listview控件设置扩展风格:点击某一行该行全部选中

	SetTimer(1, 3000, NULL);
	countStu = 0;
	countPc = 0;
	// 除非将焦点设置到控件，否则返回 TRUE
	return TRUE;
}

void CTeacherDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTeacherDlg::OnPaint()
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
HCURSOR CTeacherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//界面颜色设置
HBRUSH CTeacherDlg::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
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

//刷新某一区域
void CTeacherDlg::RefreshControl(UINT uCtrlID)
{
	CRect rc;
	GetDlgItem(uCtrlID)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	InvalidateRect(rc);
}

//用户定义消息的消息处理函数
LRESULT CTeacherDlg::MyMsgHandle(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTEVENT(lParam) == FD_READ)
		receivemsg();
	return 0;
}

//为对话框增加初始化成员函数
BOOL CTeacherDlg::broadinit(void)
{
	
	BOOL bOpVal=TRUE;	//选项的值
	int alen=sizeof(SOCKADDR_IN);
	if((broadsock=socket(PF_INET,SOCK_DGRAM,0))==INVALID_SOCKET)//创建套接字
	{
		WSACleanup();
		return FALSE;
	}
	memset(&broadaddr,0,alen);
	broadaddr.sin_family=AF_INET;
	broadaddr.sin_port=htons(T_PORT);
	broadaddr.sin_addr.s_addr=INADDR_ANY;//设置广播地址
	if(bind(broadsock,(LPSOCKADDR)&broadaddr,alen))//绑定广播地址到套接字
	{
		WSACleanup();
		return FALSE;
	}
	//设置套接字，定义套接口选项 SO_BROADCAST允许套接口传送广播信息
	if(setsockopt(broadsock,SOL_SOCKET,SO_BROADCAST,(char FAR*)&bOpVal,sizeof(bOpVal))==SOCKET_ERROR)
	{
		WSACleanup();
		return FALSE;
	}
	
	
	//broadsock套接字中发生网络事件，则向窗口发送消息
	//发送的消息是自己定义的消息WM_MYSOCK
	if(WSAAsyncSelect(broadsock,this->m_hWnd,WM_MYSOCK,FD_READ|FD_CONNECT|FD_ACCEPT|FD_WRITE|FD_CLOSE)==SOCKET_ERROR)
	{
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}
//为对话框增加用于接受广播消息的成员函数
//包含字符串消息和操作码
BOOL CTeacherDlg::broadrecv(CString &strMsg ,int* command)
{
	SOCKADDR_IN sourceadd;
	int alen=sizeof(SOCKADDR_IN);

	char buf[BUFLEN];//定义用于接受广播消息的缓冲区
	int n;
	memset(buf,0,BUFLEN);//清空缓冲区
	strMsg.Empty();
	//从学生端接收广播消息
	//内容放到接收缓冲区，sourceadd指向装有原地址的缓冲区
	//如果正确接收字节，返回收到的字节数，失败返回-1
	n=recvfrom(broadsock,buf,BUFLEN-1,0,(LPSOCKADDR)&sourceadd,&alen);
	while(n>0)//如果收到了数据
	{
		PMES_PACKAGE bag;
		bag= (PMES_PACKAGE)&buf;
		*command = ntohs(bag->command);//传递学生端发送来的操作码

		strMsg=inet_ntoa(sourceadd.sin_addr);//加上传递来的地址
		strMsg+="-";//分隔符
		strMsg+=bag->data;//学生端发送来的主机名
		memset(buf,0,BUFLEN);
		alen=sizeof(SOCKADDR_IN);
		n=recvfrom(broadsock,buf,BUFLEN-1,0,(LPSOCKADDR)&sourceadd,&alen);
	}
	
	return TRUE;
}

//广播发送函数
BOOL CTeacherDlg::broadsend(CString strMsg ,int command)
{
	SOCKADDR_IN target;
	int alen=sizeof(SOCKADDR_IN);
	int len=2;
	

	/*if(sndBuf!=NULL)
	{
		free(sndBuf);
		sndBuf=NULL;
	}*/
	(char*)sndBuf=(char*)malloc(BUFLEN);
	ZeroMemory(sndBuf,BUFLEN);//清空
	PMES_PACKAGE bag;
	bag = (PMES_PACKAGE)sndBuf;		//包的头指针指向发送缓存区
	bag -> command=htons(command);	//将命令从主机字节序转换成网络字节序
	char *point;
	point = bag->data;//
	//将要发送的字符串消息填充到包中的数据字段
	sprintf_s(point,strlen(strMsg)+1,"%s",strMsg);
	len += strlen(strMsg)+1;

	memset(&target,0,alen);
	target.sin_family=AF_INET;
	target.sin_port=htons(S_PORT);
	target.sin_addr.s_addr=INADDR_BROADCAST;//目标地址为广播地址

	//向目标套接字发送缓冲区的内容，即数据包的字符串消息字段和命令字段
	if(sendto(broadsock,sndBuf,len+1,0,(LPSOCKADDR)&target,alen)==SOCKET_ERROR)
	{
		return FALSE;
	}
	return TRUE;
}

void CTeacherDlg::receivemsg()
{
	CString strMsg;
	int command = 0;
	if (broadrecv(strMsg, &command)) {
		//广播接收函数收到字符串消息和命令序列，
		opFun(strMsg, command);
	}
}

void CTeacherDlg::opFun(CString strMsg, int command) {
	string sip;
	//收到学生的心跳回应
	if (command == MES_KEEP2) {
		sip = strMsg.Left(strMsg.Find("-"));//找到字符串消息中学生的IP
		if (onlineHash.count(sip) == 0)//如果没有找到存放该ip的容器，即学生不存在容器中
		{
			int nItem = m_OnlineHost.InsertItem(0, strMsg);//插入字符串信息，包含主机名称
			onlineHash[sip].nRow = nItem;
			a.Format("%d", ++countPc);
			RefreshControl(IDC_PCStatic);
			GetDlgItem(IDC_PCStatic)->SetWindowText(a);//在界面中显示增加后的主机数
		}
		onlineHash[sip].afk = GetCurrentTime();//该容器（学生）的离线时间更新
	}

	//收到学生的签到回应
	else if (command == MES_QD2) {
		sip = strMsg.Left(strMsg.Find("-"));//找到学生的IP
		if (onlineHash.count(sip) != 0)//如果存在该学生
		{
			int nRow = onlineHash[sip].nRow;//找到当前学生的行数
			m_OnlineHost.SetItemText(nRow, 1, strMsg.Mid(strMsg.Find("-") + 1));//在该行的学生IP右边写入主机
			a.Format("%d", ++countStu);//学生数增加1
			RefreshControl(IDC_StuStatic);
			GetDlgItem(IDC_StuStatic)->SetWindowText(a);

		}
	}
}
void CTeacherDlg::OnDestroy()
{
	KillTimer(1);
	if (broadsock != NULL)
	{
		closesocket(broadsock);
		WSACleanup();
	}
	CDialog::OnDestroy();
}

//响应settimer消息，settime的时间到了就此处执行代码
//发送心跳
void CTeacherDlg::OnTimer(UINT_PTR nIDEvent)
{
	//如果是定时器1（只设置了一个定时器）
	if (nIDEvent == 1) {
		//发送心跳请求报文
		//MessageBox("发送心跳报文");
		broadsend("", MES_KEEP1);

		//刷新主机列表
		int curtime = GetCurrentTime();
		//定义一个用来更新主机和签到数量显示的值
		bool flag = false;      //CString str;
		//hash表迭代，按顺序查找学生列表
		for (it = onlineHash.begin(); it != onlineHash.end();) {
			//如果系统时间减去离线时间大于6秒，即这个学生不再回应心跳请求已经离线
			if (curtime - it->second.afk > 6000)
			{
				m_OnlineHost.DeleteItem(it->second.nRow);
				onlineHash.erase(it++);//删除该学生
				flag = true;
				countPc--;//在线主机数量-1
				if (countStu)//签到数量-1
					countStu--;
			}
			else
				++it;
		}
		//如果有学生离线，flag会改值为true，即需要更新
		if (flag) {
			a.Format("%d", countPc);
			RefreshControl(IDC_PCStatic);
			GetDlgItem(IDC_PCStatic)->SetWindowText(a);
			a.Format("%d", countStu);
			RefreshControl(IDC_StuStatic);
			GetDlgItem(IDC_StuStatic)->SetWindowText(a);
		}
	}
	else;
	CDialogEx::OnTimer(nIDEvent);
}
NOTIFYICONDATA m_nid;
bool SetupTrayIcon(HWND m_hWnd)//创建托盘图标
{
	m_nid.cbSize = sizeof(NOTIFYICONDATA);				// 结构大小（sizeof struct），必须设置
	m_nid.hWnd = m_hWnd;									// 接收 托盘通知消息 的窗口句柄
	m_nid.uID = IDR_MAINFRAME;							// 图标ID ( 由回调函数的WPARAM 指定)
	m_nid.uFlags = NIF_MESSAGE							//表示uCallbackMessage 有效	#define NIF_MESSAGE 0x1   
		| NIF_ICON								//表示hIcon 有效			#define NIF_ICON    0x2	
		| NIF_TIP;								//表示szTip 有效			#define NIF_TIP     0x4   
	m_nid.uCallbackMessage = WM_SHOWTASK;						// 消息被发送到此窗口过程
	m_nid.hIcon = ::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINFRAME)); // 图标句柄
	strcpy_s(m_nid.szTip, "教师端");				// 提示文本

	return 0 != Shell_NotifyIcon(NIM_ADD, &m_nid);
}
//处理发送给窗口的消息
LRESULT CTeacherDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类 
	switch (message)
	{
		//如果是系统消息  
	case WM_SYSCOMMAND:
	{
		if (wParam == SC_CLOSE)
		{
			SetupTrayIcon(m_hWnd);  //创建托盘图标
			ShowWindow(SW_HIDE);    //隐藏主窗口 
			return 0 != Shell_NotifyIcon(NIM_MODIFY, &m_nid);
		}
		break;
	}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

//发送消息的函数
void CTeacherDlg::sendMsg(CString strMsg, int command, CString target_ip) {
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	int alen = sizeof(SOCKADDR_IN);
	PMES_PACKAGE bag;

	SOCKADDR_IN target;
	memset(&target, 0, alen);
	target.sin_family = AF_INET;
	target.sin_port = htons(S_PORT);
	target.sin_addr.s_addr = inet_addr(target_ip);

	if (sndBuf != NULL)
	{
		free(sndBuf);
		sndBuf = NULL;
	}
	sndBuf = (char*)malloc(BUFLEN);
	ZeroMemory(sndBuf, BUFLEN);

	bag = (PMES_PACKAGE)sndBuf;
	int len = 2;
	bag->command = htons(command);
	char* point;
	point = bag->data;
	sprintf_s(point, strlen(strMsg) + 1, "%s", strMsg);
	len += strlen(strMsg) + 1;
	sendto(sock, sndBuf, len, 0, (LPSOCKADDR)&target, alen);
	closesocket(sock);
}

//发送私信
void CTeacherDlg::OnBnClickedSendmsg()
{

	CString message;
	GetDlgItem(IDC_MSG_EDIT)->GetWindowText(message);//获取编辑框内容
	int pos = m_OnlineHost.GetSelectionMark();
	if (pos != -1) {
		CString strText = m_OnlineHost.GetItemText(pos, 0);
		CString title;
		sendMsg(message, MES_MSG, strText.Left(strText.Find("-")));//向选中的主机地址发送消息
		m_TalkMessage.AddString("老师对" + strText+":\n" + message);
		GetDlgItem(IDC_MSG_EDIT)->SetWindowText("");//发送后清空编辑框内容
	}
	else
		MessageBox("选择一台主机");
}

//发送广播
void CTeacherDlg::OnBnClickedBroadcast()
{
	CString message;
	GetDlgItem(IDC_MSG_EDIT)->GetWindowText(message);//获取编辑框内容
	broadsend(message, MES_MSG);
	m_TalkMessage.AddString("【广播】老师:\n" + message);
	GetDlgItem(IDC_MSG_EDIT)->SetWindowText("");//发送后清空编辑框内容
}

//关机
void CTeacherDlg::OnBnClickedShut()
{
	// TODO: 发送关机命令
	int pos = m_OnlineHost.GetSelectionMark();//返回选中主机
	if (pos != -1) {
		CString strText = m_OnlineHost.GetItemText(pos, 0);//目标主机的名称
		if (MessageBox("确认要关闭目标主机" + strText, "警告！", MB_ICONEXCLAMATION | MB_OKCANCEL) == IDOK) {
			sendMsg("", MES_SHUT, strText.Left(strText.Find("-")));
			m_TalkMessage.AddString("老师对" + strText + ":\n关机!");
		}
	}
	else {
		// TODO: 发送全体关机命令
		if (MessageBox("确认要关闭所有主机？", "警告！", MB_ICONEXCLAMATION | MB_OKCANCEL) == IDOK) {
			broadsend("", MES_SHUT);
			m_TalkMessage.AddString("老师:所有主机关机!");
		}
	}
}

//重启
void CTeacherDlg::OnBnClickedRelod()
{
	// TODO: 在此添加控件通知处理程序代码
	int pos = m_OnlineHost.GetSelectionMark();
	if (pos != -1) {
		CString strText = m_OnlineHost.GetItemText(pos, 0);
		if (MessageBox("确认要重启目标主机：" + strText, "警告！", MB_ICONEXCLAMATION | MB_OKCANCEL) == IDOK) {
			sendMsg("", MES_RESTART, strText.Left(strText.Find("-")));
			m_TalkMessage.AddString("老师对" + strText + ":\n重启!");
		}
	}
	else {
		if (MessageBox("确认要重启所有主机？", "警告！", MB_ICONEXCLAMATION | MB_OKCANCEL) == IDOK) {
			broadsend("", MES_RESTART);
			m_TalkMessage.AddString("老师:所有主机重启!");
		}
	}
}

//点名
void CTeacherDlg::OnBnClickedRollcallbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);//将数据从对话框的控件中传送到对应的数据成员中
	broadsend("", MES_QD1);
	countStu = 0;//每次发送点名初始出勤人数，防止重复点到
}


/*点击最小化托盘时的动作*/
LRESULT CTeacherDlg::onShowTask(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
		return 1;
	switch (lParam)
	{

	case WM_RBUTTONUP://右键起来时弹出快捷菜单 
	{

		LPPOINT lpoint = new tagPOINT;
		::GetCursorPos(lpoint);//得到鼠标位置 
		CMenu menu;
		menu.CreatePopupMenu();//声明一个弹出式菜单 
		//增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已 
		//隐藏），将程序结束。 
		menu.AppendMenu(MF_STRING, WM_DESTROY, "退出");
		//确定弹出式菜单的位置 
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
		//资源回收 
		HMENU hmenu = menu.Detach();
		menu.DestroyMenu();
		delete lpoint;
	}
	break;
	case WM_LBUTTONDBLCLK://双击左键的处理 
	{
		this->ShowWindow(SW_SHOW);// 
	}
	break;
	}
	return 0;
}