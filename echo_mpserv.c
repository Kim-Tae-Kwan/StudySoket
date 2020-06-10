#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF 30
void err(char*mes)
{
	fputs(mes,stderr);
	fputc('\n',stderr);
	exit(1);
}

void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid=waitpid(-1,&status,WNOHANG);
	printf("removed proc id : %d \n",pid);
}

int main(int argc,char*argv[])
{
	int serv_sock,clnt_sock , i=1;
	struct sockaddr_in serv_adr, clnt_adr;

	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUF];
	if(argc!=2)
	{
		printf("Usage : %s <PORT> \n",argv[0]);
		exit(1);
	}

	act.sa_handler=read_childproc;
	sigemptyset(&act.sa_mask);
	state=sigaction(SIGCHLD,&act,0);

	serv_sock=socket(PF_INET,SOCK_STREAM,0);
	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
		err("bind() err");

	if(listen(serv_sock,5)==-1)
		err("listen() err");

	while(1)
	{
		adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&adr_sz);
		if(clnt_sock==-1)
			continue;
		else
		{
			printf("%d client connected..\n",i);
			i+=1;
		}
		pid=fork();
		if(pid==-1)
		{
			close(clnt_sock);
			continue;
		}
		if(pid==0)
		{
			close(serv_sock);
			while((str_len=read(clnt_sock,buf,BUF))!=0)
				write(clnt_sock,buf,str_len);
			close(clnt_sock);
			puts("client disconnected ...");
			return 0;
		}	
		else
			close(clnt_sock);
		
	}
	close(serv_sock);
	return 0;
	
}
