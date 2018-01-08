
#include <winsock2.h>
#include <stdio.h>
#include <string>
#include <MMSystem.h>
#include <windows.h>
#include <process.h>
#include <WS2tcpip.h>
#include "NetService.h"
#include "E3DNet.h"
#pragma comment (lib,"ws2_32.lib")
#pragma comment(lib, "winmm.lib") 

WSADATA		wsData;				//WinSock Async ��������
SOCKADDR_IN ServerAddr;		//��������ַ
SOCKET		ClientSocket;	//�����׽��� SOCKET��һ���ṹ�� ����ϵͳ������
bool		bExit = false;
HANDLE		hThread = NULL;
unsigned int threadID = 0;

unsigned __stdcall NetMain(void*)
{
	int nRcv;
	//3.�����շ����ݵ�ѭ��
	char Buffer[CMD_BUFFER_LENGTH] = { 0 };
	while(!bExit)
	{
		EnterCriticalSection(&E3DNet::GetInstance().p_NetCS);
		if (0 >= (nRcv = recv(ClientSocket, Buffer, sizeof(Buffer), 0)))
		{
			break;
		}
		int cmdLength = CMD_BUFFER_LENGTH;
		E3DNet::GetInstance().GetNetMsg((BYTE*)Buffer, cmdLength);
		LeaveCriticalSection(&E3DNet::GetInstance().p_NetCS);
		Sleep(1);
	}
	return 0;
}

void Send(std::string msg)
{
	send(ClientSocket, msg.c_str(), msg.length(), 0);		//�������� //����Ϣ����ȥ
}

void Close()
{
	//4.�ر��׽���
	shutdown(ClientSocket, SD_SEND);
	closesocket(ClientSocket);
	WSACleanup();	//�ر��׽��ֻ���
}

void CreateTCP(const char *ip, int port)
{
	//�����׽��ֻ���  �������绷�� 0x0202�ǰ汾��
    WSAStartup(0x0202,&wsData);	
	//1.[�����׽���]
	ClientSocket= socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(ClientSocket==INVALID_SOCKET)
	{
		WSACleanup();
		return;
	}

	//2.[���ӵ�ָ���ķ������˿�]
	memset(&ServerAddr,0,sizeof(ServerAddr)); 
	//��ַ��:���������ַ ��ʾintenet��
	ServerAddr.sin_family=AF_INET;		
	//�˿ں�ת���������ֽ�˳��  htonsת�ɻ��������ֽڶ�ȡ����ʽ
	ServerAddr.sin_port=htons(port);	
	inet_pton(AF_INET, ip, (void*)&ServerAddr.sin_addr.S_un.S_addr);

	connect(ClientSocket,(SOCKADDR*)&ServerAddr,sizeof(ServerAddr));
	//�����߳�
	hThread = (HANDLE)_beginthreadex(NULL, 0, &NetMain, NULL, 0, &threadID);
	
}