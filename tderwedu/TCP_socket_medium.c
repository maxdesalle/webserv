// Server side C program to demonstrate Socket programming
/* FROM 
** https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
*/
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h> // inet_ntop

#define PORT 8080
int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;
	int opt = 1;
	int server_fd, new_socket; long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	
	char *hello = "Hello from server";
	
	(void)valread;

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}

	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )  
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	memset(address.sin_zero, '\0', sizeof address.sin_zero);
	
	
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 10) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}
		char addr_client[INET_ADDRSTRLEN];
		
		inet_ntop(AF_INET, (void *)&address.sin_addr.s_addr, addr_client, INET_ADDRSTRLEN);
		
		char buffer[30000] = {0};
		valread = read( new_socket , buffer, 30000);
		write(new_socket , addr_client , strlen(addr_client));
		write(new_socket, "\n", 1);
		printf("%s\n", buffer );
		write(new_socket , hello , strlen(hello));
		printf("------------------Hello message sent-------------------\n");
		// close(new_socket);
	}
	return 0;
}
