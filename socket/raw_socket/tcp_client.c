#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

void client_error(const char * msg)
{
	printf("exiting ...\n");
	printf("%s,errno:%d,errmsg:%s",msg,errno,strerror(errno));
	exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[])
{
	int sock_fd;
	struct sockaddr_in srv_addr;
	bzero((char *)&srv_addr,sizeof(srv_addr));

	char str_send[256],str_recv[256];
	memset(str_send,0,256);
	memset(str_recv,0,256);

	//1st step:create socket
	sock_fd = socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd == -1)
		client_error("create socket fail");

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(2016);
	srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//2nd step:connect to server
	if(connect(sock_fd,(struct sockaddr *)&srv_addr,sizeof(srv_addr)) < 0)
		client_error("connect server fail");

	// int sock_flag = fcntl(sock_fd,F_GETFL,0);
	// if(-1 == sock_flag) client_error("fcntl fail");
	// if(fcntl(sock_fd,F_SETFL,sock_fd | O_NONBLOCK) < 0) client_error("set O_NONBLOCK fail");

	while(strcmp(str_send,"quit") != 0 && strcmp(str_send,"exit") !=0)
	{
		fgets(str_send,255,stdin);
		//3rd step:send request
		if(send(sock_fd,str_send,strlen(str_send),0) < 0)
			client_error("send data fail");
		printf("send:%s",str_send);
		memset(str_send,0,256);
		//4th step:recv response
		if(recv(sock_fd,str_recv,255,0) > 0)
		{
			printf("recv:%s\n",str_recv);
			memset(str_recv,0,256);
		}
	}
	close(sock_fd);
	return 0;
}