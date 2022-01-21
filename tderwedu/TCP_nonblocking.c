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
#define SERV_PORT		8080
#define POLL_FLAGS		POLLIN | POLLOUT | POLLERR | POLLHUP | POLLNVAL

typedef struct sockaddr_in	t_addr_in;
typedef struct sockaddr		t_addr;
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
	t_addr_in	addr_client, addr_sock;

	// char		*hello = "Hello from server";
	char		*hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

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

	bind(fd_sock, (t_addr *) &addr_sock, sizeof(addr_sock));

	listen(fd_sock, OPEN_MAX - 1);

	poll_lst[0].fd = fd_sock;
	poll_lst[0].events = POLL_FLAGS;
	for (i = 1; i < OPEN_MAX; i++)
		poll_lst[i].fd = -1;
	nb_client = 0;

	while (1)
	{
		printf("\n*** Waiting for new connection ***\n");
		nb_poll = poll(poll_lst, nb_client + 1, -1);
		printf("\t    Number of POLL: %i\n", nb_poll);
		printf("\t Number of CLIENTS: %i\n", nb_client);
		// New Connection
		if (poll_lst[0].revents & POLLIN)
		{
			printf("===> New connection\n");
			addr_len = sizeof(addr_client);
			new_fd = accept(fd_sock, (t_addr *) &addr_client, &addr_len);
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
			printf("===> Client #%i\n", i);
			printf("===>      FD %i\n", sockfd);
			printf("REVENTS\n");
			printf("\t POLLERR : %i\n", revents & POLLERR);
			printf("\t POLLHUP : %i\n", revents & POLLHUP);
			printf("\t POLLNVAL: %i\n", revents & POLLNVAL);
			printf("\t POLLIN  : %i\n", revents & POLLIN);
			printf("\t POLLOUT : %i\n", revents & POLLOUT);
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
			else if (revents & (POLLHUP | POLLNVAL))
			{
				printf("Error : connection HANGUP or socket not RDY\n");
				disconnect = 1;
			}
			else if (revents & POLLIN)
			{
				nb_read = recv(sockfd, buff, MAXLINE, 0);
				if (nb_read < 0)
				{
					printf("Error while getting socket error!\n");
					disconnect = 1;
				}
				else if (nb_read == 0)
				{
					disconnect = 1;
				}
				else
				{
					printf("OK\n");
					printf("%s\n", buff);
					send(sockfd, hello, strlen(hello), 0);
					printf("------------------Hello message sent-------------------\n");
					disconnect = 1;
				}
			}
			if (disconnect == 1)
			{
				shutdown(sockfd, SHUT_RDWR);
				close(sockfd); // Release ressources (make FD available)
				printf("Client disconnected\n");
				poll_lst[i].fd = -1;
				poll_lst[i].revents = 0;
				nb_client -= (i == nb_client ? 1 : 0);
			}
			// else if (disconnect == 2)
			// {
			// 	shutdown(sockfd, SHUT_WR);
			// }
			if (--nb_poll <= 0)
				break;
		}
	}
}
