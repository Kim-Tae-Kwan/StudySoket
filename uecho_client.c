#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF 30

void err(char*mes)
{
	fputs(mes,stderr);
	fputc('\n',stderr);
	exit(1);
} 

int main(int argc,char*argv[])
{
	int sock;
	char mes[30];
	int str_len;
	socklen_t adr_sz;

	struct sockaddr_in serv_adr,from_adr;
	if(argc!=3)
	{
		printf("Usage : %s <IP><PORT>\n",argv[0]);
		exit(1);
	}

	sock=socket(PF_INET,SOCK_DGRAM,0);
	if(sock==-1)
		err("socket() error!!");

	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	while(1)
	{
		fputs("Input message(q to quit): ",stdout);
		fgets(mes,sizeof(mes),stdin);
		if(!strcmp(mes,"q\n")||!strcmp(mes,"Q\n"))
			break;
		sendto(sock,mes,strlen(mes),0,(struct sockaddr*)&serv_adr,sizeof(serv_adr));
		adr_sz=sizeof(from_adr);
		str_len=recvfrom(sock,mes,BUF,0,(struct sockaddr*)&from_adr,&adr_sz);
		mes[str_len]=0;
		printf("Message from server : %s\n",mes);
	}
	close(sock);
	return 0;
}
