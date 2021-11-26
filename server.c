#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>


WSADATA data;
SOCKET sock;
SOCKET client;
SOCKADDR_IN adres;
SOCKADDR_IN adres2;
char rbuf[9999];
char command[40];

int main() {
	WORD ver = MAKEWORD(2, 2);
	WSAStartup(ver, &data);
	sock = socket(AF_INET, SOCK_STREAM, 0);
    adres.sin_family = AF_INET;
	adres.sin_port = htons(7777);
	adres.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock,(SOCKADDR*)&adres,sizeof(adres));
	listen(sock,SOMAXCONN);
	client = accept(sock,0,0);
	recv(client,rbuf,sizeof(rbuf),0);
	printf("%s",rbuf);
	for(;;){
		scanf("%s",command);
		send(client,command,sizeof(command),0);
		if(command[0] == 'e'){
			int sz = 0;
			for(;command[sz+2] != '0';sz++){}
			char *nm = calloc(sz+1,1);
			memcpy(nm,command+2,sz);
			HANDLE file = CreateFileA(nm,GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
			if(GetLastError()){break;}
			int fsize = GetFileSize(file,0);
			char *buf = malloc(fsize);
			ReadFile(file,buf,fsize,0,0);
			char sz2[4] = {fsize,fsize >> 8,fsize >> 16,fsize >> 24};
			send(client,sz2,4,0);
			send(client,buf,fsize,0);
			CloseHandle(file);
			free(buf);
		}
		memset(command,0,sizeof(command));
	}
}
