#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>	//fcntl
#include <fcntl.h>	//fcntl

#include <sys/socket.h>	// socket(), shutdown(), bind(), listen(), accept(), recv(), send()
#include <arpa/inet.h>	// htons(), htonl(), ntohs(), ntohl()
#include <netinet/in.h>
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
	ssize_t		nb_read;
	char		buff[MAXLINE];
	socklen_t	addr_len, err_size;
	t_poll		poll_lst[OPEN_MAX];
	t_addr_in	addr_client, addr_sock;
	char		*hello = "Hello from server";

	fd_sock = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(fd_sock, F_SETFL, O_NONBLOCK);

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

	int	tmp = 0;
	while (tmp < 3)
	{
		printf("\n*** Waiting for new connection ***\n");
		nb_poll = poll(poll_lst, nb_client + 1, -1);
		printf("\t Number of POLL: %i\n", nb_poll);
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
					poll_lst[i].events = (POLL_FLAGS) & ~POLLOUT;
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
			printf("===> Client #%i\n", i);
			disconnect = 0;
			sockfd = poll_lst[i].fd;
			revents = poll_lst[i].revents;
			printf("REVENTS\n");
			printf("\t POLLERR : %i\n", revents & POLLERR);
			printf("\t POLLHUP : %i\n", revents & POLLHUP);
			printf("\t POLLNVAL: %i\n", revents & POLLNVAL);
			printf("\t POLLIN  : %i\n", revents & POLLIN);
			printf("\t POLLOUT : %i\n", revents & POLLOUT);
			if (sockfd < 0 || revents == POLLOUT)
			{
				printf("Nothing to do\n");
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
				nb_read = read(sockfd, buff, MAXLINE);
				if (nb_read < 0)
				{
					printf("Error while getting socket error!\nb_read");
					disconnect = 1;
				}
				else if (nb_read == 0)
					disconnect = 1;
				else
				{
					printf("OK\n");
					printf("%s\n", buff);
					write(sockfd, hello, strlen(hello));
					printf("------------------Hello message sent-------------------\n");
				}
				if (--nb_poll <= 0)
					break;
			}
			if (disconnect)
				shutdown(sockfd, SHUT_RDWR ); // BAD disconnect
		}
		++tmp;
	}
}
