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
	char mes[BUF];
	struct sockaddr_in my_adr,your_adr;
	socklen_t adr_sz;
	int str_len,i;

	if(argc!=2)
	{
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}

	sock=socket(PF_INET,SOCK_DGRAM,0);
	if(sock==-1)
		err("sock() error!!");

	memset(&my_adr,0,sizeof(my_adr));
	my_adr.sin_family=AF_INET;
	my_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	my_adr.sin_port=htons(atoi(argv[1]));

	if(bind(sock,(struct sockaddr*)&my_adr,sizeof(my_adr))==-1)
		err("bind() error!!");

	for(i=0;i<3;i++)
	{
		sleep(1);
		adr_sz=sizeof(your_adr);
		str_len=recvfrom(sock,mes,BUF,0,(struct sockaddr*)&your_adr,&adr_sz);
		printf("Message %d: %s \n",i+1,mes);
		
	}
	close(sock);
	return 0;

	



}