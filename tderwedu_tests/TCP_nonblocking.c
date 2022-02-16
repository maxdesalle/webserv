#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>	//fcntl
#include <fcntl.h>	//fcntl

#include <sys/socket.h>	// socket(), shutdown(), bind(), listen(), accept(), recv(), send()
#include <arpa/inet.h>	// htons(), htonl(), ntohs(), ntohl()
#include <netinet/in.h>
#include <sys/time.h> // struct timeval
#include <poll.h>

#include	<limits.h>	  /* for OPEN_MAX */

#define OPEN_MAX		16
#define MAXLINE			1024
#define SERV_PORT		80
#define POLL_FLAGS		POLLIN | POLLOUT	// POLLERR | POLLHUP | POLLNVAL allways set

typedef struct sockaddr_in	t_sockaddr_in;
typedef struct sockaddr		t_sockaddr;
typedef struct pollfd		t_poll;

int	main(void)
{
	short		revents;
	int			i, nb_client, fd_sock, new_fd, sockfd, disconnect, err;
	int			nb_poll;
	int			opt = 1;
	ssize_t		nb_read;
	char		buff[MAXLINE];
	socklen_t	addr_len, err_size;
	t_poll		poll_lst[OPEN_MAX];
	t_sockaddr_in	addr_client, addr_sock;

	// char		*hello = "Hello from server";
	// char	*hello = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 14\r\n\r\nHello world!\n.";
	char	*hello = "HTTP/1.1 301 Moved Permanently\r\nServer: nginx/1.14.2\r\nDate: Wed, 16 Feb 2022 20:51:41 GMT\r\nContent-Type: text/html\r\nContent-Length: 185\r\nLocation: http://localhost/error_page/\r\nConnection: keep-alive\r\n\r\n<html>\n<head><title>301 Moved Permanently</title></head>\n<body bgcolor=\"white\">\n<center><h1>301 Moved Permanently</h1></center>\n<hr><center>nginx/1.14.2</center>\n</body>\n</html>\r\n";

	fd_sock = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(fd_sock, F_SETFL, O_NONBLOCK);

	if(setsockopt(fd_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )  
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	bzero(&addr_sock, sizeof(addr_sock));
	addr_sock.sin_family = AF_INET;
	addr_sock.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_sock.sin_port = htons(SERV_PORT);

	bind(fd_sock, (t_sockaddr *) &addr_sock, sizeof(addr_sock));

	listen(fd_sock, OPEN_MAX - 1);

	poll_lst[0].fd = fd_sock;
	poll_lst[0].events = POLL_FLAGS;
	for (i = 1; i < OPEN_MAX; i++)
		poll_lst[i].fd = -1;
	nb_client = 0;

	while (1)
	{
		printf("\e[34m \t Number of CLIENTS: %i \e[0m \n", nb_client);
		printf("\e[34m  *** Waiting for new connection *** \e[0m \n");
		nb_poll = poll(poll_lst, nb_client + 1, -1);
		printf("\e[34m \t    Number of POLL: %i \e[0m \n\n", nb_poll);
		// New Connection
		if (poll_lst[0].revents & POLLIN)
		{
			printf("===> New connection\n");
			addr_len = sizeof(addr_client);
			new_fd = accept(fd_sock, (t_sockaddr *) &addr_client, &addr_len);
			fcntl(new_fd, F_SETFL, O_NONBLOCK);
			for (i = 1; i < OPEN_MAX; i++)
			{
				if (poll_lst[i].fd < 0)
				{
					poll_lst[i].fd = new_fd;
					// poll_lst[i].events = POLLIN | POLLOUT;
					poll_lst[i].events = POLLIN;
					nb_client += (i > nb_client ? 1 : 0);
					break;
				}
			}
			printf("\t Number of CLIENTS: %i \n", nb_client);
			printf("\t                FD: %i \n", new_fd);
			if (i == OPEN_MAX)
				exit(EXIT_FAILURE);
			if (--nb_poll <= 0)
				continue;
		}
		// Active Connections
		for (i = 1; i <= nb_client; i++)
		{
			
			disconnect = 0;
			sockfd = poll_lst[i].fd;
			revents = poll_lst[i].revents;
			printf("\e[33m ===> Client #%i \e[0m \n", i);
			printf("\e[33m ===>      FD %i \e[0m \n", sockfd);
			if (sockfd < 0 || revents == POLLOUT)
			{
				printf("Nothing to do : %i\n", sockfd);
				continue;
			}
			if (revents & POLLERR)
			{
				printf("Error : POLLERR\n");
					err_size = sizeof(err);
					if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &err, &err_size) < 0)
						printf("Error while getting socket error!\nb_read");
					else
						printf("Error : %i\nb_read", err);
				disconnect = 1;
			}
			else if (revents & POLLHUP)
			{
				printf("\e[32m POLLHUP \e[0m \n");
				printf("\e[32m Client Disconnected \e[0m \n");
				disconnect = 1;
			}
			else if (revents & POLLIN)
			{
				nb_read = recv(sockfd, buff, MAXLINE, 0);
				if (nb_read < 0)
				{
					printf("\e[31m Error while getting socket error! \e[0m \n");
					disconnect = 1;
				}
				else if (nb_read == 0)
				{
					// printf("\e[32m Client Disconnected \e[0m \n");
					printf("\e[32m Going GraceFull \e[0m \n");
					disconnect = 2;
				}
				else
				{
					printf("\e[33m Msg Len: %li \e[0m \n", nb_read);
					buff[nb_read] = '\0';
					printf("%s\n", buff);
					send(sockfd, hello, strlen(hello), 0);
					printf("------------------Hello message sent-------------------\n");
					disconnect = 0;
				}
			}
			else
			{
				printf("REVENTS\n");
				printf("\t POLLERR : %i\n", revents & POLLERR);
				printf("\t POLLHUP : %i\n", revents & POLLHUP);
				printf("\t POLLNVAL: %i\n", revents & POLLNVAL);
				printf("\t POLLIN  : %i\n", revents & POLLIN);
				printf("\t POLLOUT : %i\n", revents & POLLOUT);
			}
			if (disconnect == 1)
			{
				shutdown(sockfd, SHUT_RDWR);
				close(sockfd); // Release ressources (make FD available)
				printf("\e[32m Connection Closed \e[0m\n");
				poll_lst[i].fd = -1;
				poll_lst[i].revents = 0;
				if (i == nb_client)
				{
					int j = i - 1;
					while (poll_lst[j].fd < 0)
						j--;
					nb_client = j;
				}
			}
			else if (disconnect == 2)
			{
				printf("\e[32m Gracefull Closing \e[0m\n");
				shutdown(sockfd, SHUT_WR);
			}
			if (revents && --nb_poll <= 0)
				break;
		}
	}
}
