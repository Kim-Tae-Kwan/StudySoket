#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#define BUF 70000

void * Send_Html(void*arg);

char webpage[]="HTTP/1.1 200 OK\r\n"
        "Server.Linux Web Server\r\n"
        "content-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>\r\n"
        "<html><head><title> KimTK Web Page </title>\r\n"
        "<style>body {background-color:#FFFF00}</style></head>\r\n"
        "<body><center><h1>Hi My name is KimTK</h1><br>\r\n"
        "<img src=\"kim.jpg\"></center></body></html>";

void err(char*mes)
{
    fputs(mes,stderr);
    fputc('\n',stderr);
    exit(1);

}

int main(int argc,char*argv[])
{
    
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_adr,clnt_adr;
    int clnt_adr_sz,option=1;
    pthread_t t_id;
    
    if(argc!=2)
    {
        printf("Usage %s <PORT>\n",argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,(void*)&option,sizeof(option));
    
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));
    
    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        err("bind() Error!!");
    if(listen(serv_sock,20)==-1)
        err("listen() Error!!");
    
    while(1)
    {   
        clnt_adr_sz=sizeof(clnt_adr);
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
        puts("New client connection");

        
        pthread_create(&t_id,NULL,Send_Html,(void*)&clnt_sock);
        pthread_detach(t_id);

        
    }
    close(serv_sock);
    return 0;


}
void * Send_Html(void*arg)
{
    
    int clnt_sock=*((int*)arg);
    char buf[2048];
    long size;
	FILE*fp;
    int read_cnt;
    char img_buf[BUF];
    long result;

	//close(serv_sock);
	memset(buf,0,2048);
    read(clnt_sock,buf,2047);
    printf("%s\n",buf);
    

    if(!strncmp(buf,"GET /kim.jpg",12))
    {
		printf("1\n");    
        fp=fopen("kim.jpg","rb"); 
        if(fp==NULL) 
        { 
        	fputs("File error",stderr);
        	exit(1);
		}
		printf("2\n");
		fseek(fp,0,SEEK_END);
		printf("3\n");
		size=ftell(fp);
		printf("4\n");
		rewind(fp);
		printf("5\n");
		result=fread(img_buf,1,size,fp);
		printf("10\n");
		if(result!=size)
		{	
			printf("9\n");
			fputs("Reading error",stderr);
			exit(3);
		}
		printf("6\n");
		write(clnt_sock,img_buf,sizeof(img_buf));
		printf("7\n");
		fclose(fp);
		printf("8\n");
		printf("kim.jpg\n");
     	

    }
	else
		write(clnt_sock,webpage,sizeof(webpage));

	close(clnt_sock);
	puts("closing....");
	
    return NULL;
}

