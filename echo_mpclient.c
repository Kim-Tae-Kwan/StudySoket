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
void read_routine(int sock,char *buf)
{
	while(1)
	{
		int str_len=read(sock,buf,BUF);
		if(str_len==0)
			return;
		buf[str_len]=0;
		printf("Message from server : %s",buf);
	}

}
void write_routine(int sock,char*buf)
{
	while(1)
	{
		fgets(buf,BUF,stdin);
		if(!strcmp(buf,"q\n")||!strcmp(buf,"Q\n"))
		{
			shutdown(sock,SHUT_WR);
			return;
		}
		write(sock,buf,strlen(buf));
	}
}

int main(int argc,char*argv[])
{
	int sock;
	pid_t pid;
	char buf[BUF];
	struct sockaddr_in serv_adr;
	if(argc!=3)
	{
		printf("Usage : %s <IP><PORT>\n",argv[0]);
		exit(1);
	}

	sock=socket(PF_INET,SOCK_STREAM,0);
	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
		err("connect() err");

	pid=fork();
	if(pid==0)
		write_routine(sock,buf);
	else
		read_routine(sock,buf);

	close(sock);
	return 0;



















}
