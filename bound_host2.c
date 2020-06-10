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
	char msg1[30];
	char msg2[30];
	char msg3[30];

	fgets(msg1,30,stdin);
	fgets(msg2,30,stdin);
	fgets(msg3,30,stdin);
	
	struct sockaddr_in your_adr;
	socklen_t your_adr_sz;
	if(argc!=3)
	{
		printf("Usage : %s <IP> <PORT>\n",argv[0]);
		exit(1);
	
	}

	sock=socket(PF_INET,SOCK_DGRAM,0);
	if(sock==-1)
		err("socket() error!!");

	memset(&your_adr,0,sizeof(your_adr));
	your_adr.sin_family=AF_INET;
	your_adr.sin_addr.s_addr=inet_addr(argv[1]);
	your_adr.sin_port=htons(atoi(argv[2]));

	sendto(sock,msg1,sizeof(msg1),0,(struct sockaddr*)&your_adr,sizeof(your_adr));
	sendto(sock,msg2,sizeof(msg2),0,(struct sockaddr*)&your_adr,sizeof(your_adr));
	sendto(sock,msg3,sizeof(msg3),0,(struct sockaddr*)&your_adr,sizeof(your_adr));
	close(sock);
	return 0;


}
