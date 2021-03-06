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
	int serv_sock;
	char mes[BUF];
	int str_len;
	socklen_t clnt_adr_sz;

	struct sockaddr_in serv_adr,clnt_adr;
	if(argc!=2)
	{
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
		
	}

	serv_sock=socket(PF_INET,SOCK_DGRAM,0);
	if(serv_sock==-1)
	{
		err("socket() error!!");
		
	}

	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
		err("bind() error!!!");

	while(1)
	{
		clnt_adr_sz=sizeof(clnt_adr);
		str_len=recvfrom(serv_sock,mes,BUF,0,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
		sendto(serv_sock,mes,str_len,0,(struct sockaddr*)&clnt_adr,clnt_adr_sz);


		for(int i=0;i<sizeof(mes) ;i++)
		{
			if(mes[i]==0)
				break;
			else
				printf("%c",mes[i]);

		}
		

	}
	close(serv_sock);
	return 0;

}
