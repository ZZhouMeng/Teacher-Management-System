#pragma once
typedef struct mes_hdr
{
	unsigned short command;
	char data[1];
}MES_PACKAGE, *PMES_PACKAGE;


#define MES_KEEP1	1	//����������
#define MES_KEEP2	2	//������Ӧ����
#define MES_QD1		3	//ǩ��������
#define MES_QD2		4	//ǩ����Ӧ����
#define MES_MSG		5	//��Ϣ����
#define	MES_SHUT	6	//�ػ������
#define	MES_RESTART	7	//���������

#define  TIMEOUT  5000UL