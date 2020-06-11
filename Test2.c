#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

#define BUF_SIZE 1024

char webpage[]="HTTP/1.1 200 OK\r\n"
        "Server.Linux Web Server\r\n"
        "content-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>\r\n"
        "<html><head><title> KimTK Web Page </title>\r\n"
        "<style>body {background-color:#FFFF00}</style></head>\r\n"
        "<body><center><h1>Hi My name is KimTK</h1><br>\r\n"
        "<img src=\"game.jpeg\"></center></body></html>";


void Error_handling(char*message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);

}

int main(int argc,char*argv[])
{
	int serv_sock, clnt_sock;
	char buf[2048];
	struct sockaddr_in serv_addr,clnt_addr;
	socklen_t sin_len=sizeof(clnt_addr);
	int fdimg;
	int option=1;
	char img_buf[700000];
	pid_t pid;
	if(argc!=2)
	{
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}

	serv_sock=socket(AF_INET,SOCK_STREAM,0);
	if(serv_sock==-1)
		Error_handling("sock() error!");
	setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,&option,sizeof(int));


	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
		Error_handling("bind() error!!");

	if(listen(serv_sock,50)==-1)
		Error_handling("listen() error!!!");

	while(1)
	{
		clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&sin_len);
		
		puts("New client connection");
		//write(clnt_sock,webpage,sizeof(webpage));
		pid=fork();
		if(pid==0)
		{
			close(serv_sock);
			memset(buf,0,2048);
			read(clnt_sock,buf,2047);
			printf("%s\n",buf);
			if(!strncmp(buf,"GET /game.jpeg",14))
			{
				fdimg=open("game.jpeg",O_RDONLY);
				read(fdimg,img_buf,sizeof(img_buf));
				write(clnt_sock,img_buf,sizeof(img_buf));
				close(fdimg);
				printf("game.jpeg\n");
			}
			else
				write(clnt_sock,webpage,sizeof(webpage));
			close(clnt_sock);
			puts("closing....");
			exit(1);
		}
		else
		{
//			write(clnt_sock,webpage,sizeof(webpage));
		
			close(clnt_sock);
		}
		
	}	
	return 0;

}
