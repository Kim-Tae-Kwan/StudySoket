#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#define BUF 30

void * Send_Html(void*arg);

char webpage[]="HTTP/1.1 200 OK\r\n"
        "Server.Linux Web Server\r\n"
        "content-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>\r\n"
        "<html><head><title> KimTK Web Page </title>\r\n"
        "<style>body {background-color:#FFFF00}</style></head>\r\n"
        "<body><center><h1>Hi My name is KimTK</h1><br>\r\n";
        //"<img src=\"kim.jpg\"></center></body></html>";

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
    char buf[BUF];
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
        printf("thread 생성\n");

        write(clnt_sock,&webpage,sizeof(webpage));
        pthread_create(&t_id,NULL,Send_Html,(void*)&clnt_sock);
        pthread_detach(t_id);
    }
    close(serv_sock);
    return 0;


}
void * Send_Html(void*arg)
{
    printf("send 함수\n");
    int clnt_sock=*((int*)arg);
    char message[1024];
    // FILE* pFile;
    // long lSize;
    // char* buffer;
    // size_t result;

    FILE*fp;
    int read_cnt;
    char buf[BUF];

    
    read(clnt_sock,message,BUF);
    for(int i=0;i<1024;i++)
    {
        printf("%c",message[i]);
    }
    printf("\n");
    

    printf("if전\n");
    if(!strncmp(message,"GET /kim.jpg",13))
    {
         printf("if진입\n");
        // pFile = fopen("kim.jpg", "rb");
        // if (pFile == NULL) {
        //     fputs("File error", stderr);
        //     exit(1);
        // }

        // // 파일의 크기를 ISize 에 저장한다.
        // fseek(pFile, 0, SEEK_END);
        // lSize = ftell(pFile);
        // rewind(pFile);

        // // 전체 파일의 내용을 받을 수 있을 정도의 크기로 메모리를 할당한다.
        // buffer = (char*)malloc(sizeof(char) * lSize);
        // if (buffer == NULL) {
        //     fputs("Memory error", stderr);
        //     exit(2);
        // }

        // // 그 파일의 내용을 버퍼에 저장한다.
        // result = fread(buffer, 1, lSize, pFile);
        // if (result != lSize) {
        //     fputs("Reading error", stderr);
        //     exit(3);
        // }

        // fclose(pFile);
        // /* 이제 파일의 모든 내용은 버퍼에 들어가게 된다. */
        // write(clnt_sock,&buffer,sizeof(buffer));
        // printf("write\n");
        // // 종료
        
        // free(buffer);


        fp=fopen("kim.jpg","rb");

        while(1)
        {
            read_cnt=fread((void*)buf,1,BUF,fp);
            if(read_cnt<BUF)
            {
                write(clnt_sock,buf,read_cnt);
                break;
            }
            write(clnt_sock,buf,BUF);

        }
    }



    
    fclose(fp);
    close(clnt_sock);
    return NULL;
}

