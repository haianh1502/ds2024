#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024

void send_file(FILE *fp, int sockfd)
{
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp)!=NULL)
    {
        if(send(sockfd, data, sizeof(data), 0)== -1)
        {
            perror("Failed to send the file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

int main()
{
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    char *filename = "test1.txt";
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0)
    {
        perror("Socket creation failed!\n");
        exit(1);
    }
     printf("Socket successfully created..\n");

     server_addr.sin_family = AF_INET;
     server_addr.sin_port = port;
     server_addr.sin_addr.s_addr = inet_addr(ip);

     e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
     if(e == -1)
     {
         perror("Connection with the server failed!\n");
         exit(1);
     }
     printf("Connected to server.\n");
     fp = fopen(filename, "r");
     if(fp == NULL)
     {
         perror("Failed to read file.\n");
         exit(1);
     }
     send_file(fp,sockfd);
     printf("File send successfully. \n");
     close(sockfd);
     printf("Close. \n");
     return 0;

}