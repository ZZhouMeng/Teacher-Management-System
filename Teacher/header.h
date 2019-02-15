#pragma once
typedef struct mes_hdr
{
	unsigned short command;
	char data[1];
}MES_PACKAGE, *PMES_PACKAGE;


#define MES_KEEP1	1	//心跳请求
#define MES_KEEP2	2	//心跳响应
#define MES_QD1		3	//点名请求
#define MES_QD2	    4	//点名响应
#define MES_MSG		5	//消息
#define	MES_SHUT	6	//关机命令
#define	MES_RESTART	7	//重启命令