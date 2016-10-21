#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include "tcp_server.h"

static int exit_flag = 0;

void server_error()
{
	printf("server error,errno:%d,errmsg:%s\n",errno,strerror(errno));
	exit(EXIT_FAILURE);
}

void proc_terminal(int signo)
{
	if(signo == SIGTERM)
	{
		exit_flag = 1;
		printf("server going to shutdown....\n");
	}
}

void proc_signals()
{
	signal(SIGALRM, SIG_IGN);
    signal(SIGINT,  SIG_IGN);
    signal(SIGHUP,  SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
    signal(SIGTERM, proc_terminal);
}

void init_server_deamon()
{
	proc_signals();
	pid_t pid = fork();
	if(pid != 0) //parent process
		exit(EXIT_SUCCESS);

	//child process
	if(setsid() < 0)
		exit(EXIT_FAILURE);
	umask(0);
}

void server_run()
{
	int server_fd = 0;
	int client_fd = 0;
	int port = 2016;
	socklen_t client_len = 0;
	int n = 0;

	struct sockaddr_in server_addr,client_addr;
	bzero((char *)&server_addr,sizeof(server_addr));
	bzero((char *)&client_addr,sizeof(client_addr));
	client_len = sizeof(client_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//1st step:create server socket
	server_fd = socket(AF_INET,SOCK_STREAM,0);
	if(server_fd < 0) server_error();

	//2nd step:bind socket
	if(bind(server_fd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
		server_error();

	//3rd step:listen fd
	if(listen(server_fd,5) == -1) server_error();

	//4th step:accept
	while(exit_flag != 1)
	{
		printf("server running...\n");
		int client_fd = accept(server_fd,(struct sockaddr *)&client_addr,&client_len);
		if(client_fd == -1) server_error();
		
		ssize_t nread = 0;
		//5th step:receive data
		char buf[256];
		bzero(buf,256);
		while((nread = recv(client_fd,buf,sizeof(buf),0)) > 0)
		{	
			printf("server recv:%s\n",buf);
			//6th step:send -- response
			if(send(client_fd,buf,nread,0) == -1) server_error();
			printf("server send:%s\n",buf);
		}
		
		//7th step:close client connection
		bzero((char *)&client_addr,sizeof(client_addr));
		close(client_fd);
	}

}

int main(int argc, char const *argv[])
{
	// init_server_deamon();
	server_run();
	return 0;
}