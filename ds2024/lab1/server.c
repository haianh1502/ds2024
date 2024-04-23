#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 

#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

void func(int connectID) 
{ 
	char buffer[MAX]; 
	int n; 

	for (;;) { 
		bzero(buffer, MAX); 

		read(connectID, buffer, sizeof(buffer)); 

		printf("From client: %s\t To client : ", buffer); 
		bzero(buffer, MAX); 

		n = 0; 
		while ((buffer[n++] = getchar()) != '\n') 
			; 
		write(connectID, buffer, sizeof(buffer)); 

		if (strncmp("exit", buffer, 4) == 0) { 
			printf("Clossing sever!\n"); 
			break; 
		} 
	} 
} 

int main() 
{ 
	int sockfd, connID, length; 
	struct sockaddr_in serverAddress, client; 

	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("Socket not created!\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&serverAddress, sizeof(serverAddress)); 

	serverAddress.sin_family = AF_INET; 
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); 
	serverAddress.sin_port = htons(PORT); 

	if ((bind(sockfd, (SA*)&serverAddress, sizeof(serverAddress))) != 0) { 
		printf("Socket bind failed!\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded!\n"); 

	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed!\n"); 
		exit(0); 
	} 
	else
		printf("Server listening!\n"); 
	length = sizeof(client); 

	connID = accept(sockfd, (SA*)&client, &length); 
	if (connID < 0) { 
		printf("Server accept failed!\n"); 
		exit(0); 
	} 
	else
		printf("Server accept the client.\n"); 

	func(connID); 

	close(sockfd); 
}
