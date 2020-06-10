#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF 1024
#define RLT 4
#define OPSZ 4

void error(char*mes)
{
	fputs(mes,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(int argc , char*argv[])
{
	int sock;
	char opmsg[BUF];
	int result,opnd_count,i;
	struct sockaddr_in serv_adr;
	if(argc!=3)
	{
		printf("Usage : %s <IP><PORT>\n",argv[0]);
		exit(1);
	}


	sock=socket(PF_INET,SOCK_STREAM,0);
	if(sock==-1)
		error("socket() error!");

	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
		error("Connect() Error!!");
	else
		puts("Connected............");

	fputs("Operand count : ",stdout);
	scanf("%d",&opnd_count);
	opmsg[0]=(char)opnd_count;
	for(i=0;i<opnd_count;i++)
	{
		printf("Operand %d : ",i+1);
		scanf("%d",(int*)&opmsg[i*OPSZ+1]);
	}
	fgetc(stdin);
	fputs("Operator : ",stdout);
	scanf("%c",&opmsg[opnd_count*OPSZ+1]);
	write(sock,opmsg,opnd_count*OPSZ+2);
	read(sock,&result,RLT);

	printf("Operation result : %d \n",result);
	close(sock);
	return 0;
	











}
