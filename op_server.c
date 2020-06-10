#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF 1024
#define OPSZ 4
void err(char* mes)
{
	fputs(mes,stderr);
	fputc('\n',stderr);
	exit(1);
}

int calculate(int opnum,int opnds[],char op)
{
	int result=opnds[0],i;
	switch(op)
	{
		case '+':
			for(i=1;i<opnum;i++) result+=opnds[i];
			break;
		case '-':
			for(i=1;i<opnum;i++) result-=opnds[i];
			break;
		case '*':
			for(i=0;i<opnum;i++) result*=opnds[i];
			break;	
	}
	return result;

}

int main(int argc,char*argv[])
{
	int serv_sock, clnt_sock;
	char opinfo[BUF];
	int result, opnd_cnt,i;
	int recv_cnt, recv_len;
	struct sockaddr_in ser,cln;
	socklen_t cln_size;
	if(argc!=2)
	{
		printf("Usage : %s <PORT>\n",argv[0]);

		exit(1);
	}
	serv_sock=socket(PF_INET,SOCK_STREAM,0);
	if(serv_sock==-1)
		err("socket() error!!");

	memset(&ser,0,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_addr.s_addr=htonl(INADDR_ANY);
	ser.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock,(struct sockaddr*)&ser,sizeof(ser))==-1)
		err("bind() Error!!");

	if(listen(serv_sock,5)==-1)
		err("listen() Error!!");
	cln_size=sizeof(cln);

	for(i=0;i<5;i++)
	{
		opnd_cnt=0;
		clnt_sock=accept(serv_sock, (struct sockaddr*)&cln,&cln_size);
		read(clnt_sock,&opnd_cnt,1);

		recv_len=0;

		while((opnd_cnt*OPSZ+1)>recv_len)
		{
			recv_cnt=read(clnt_sock,&opinfo[recv_len],BUF-1);
			recv_len+=recv_cnt;
			
		}
		result=calculate(opnd_cnt,(int*)opinfo,opinfo[recv_len-1]);
		write(clnt_sock,(char*)&result, sizeof(result));
		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
	
		
}

