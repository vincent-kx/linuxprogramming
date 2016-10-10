/*
	code example for read/write system call
	
	predefined file descriptors:
	0:standard input
	1:standard output
	2:standard error output
	#include <unistd.h>
	size_t write(int fildes, const void *buf, size_t nbytes);
	size_t read(int fildes, void *buf, size_t nbytes);
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>

enum
{
	my_stdin = 0,
	my_stdout = 1,
	my_stderr = 2
};

void test_write()
{
	char * str_output = "this is an output for standard output\n";
	char * str_err_output = "this is an error message !\n";
	uint32_t len = strlen(str_output);
	size_t ret_len = write(my_stdout,str_output,len);
	if(ret_len == -1 || ret_len < len)
	{
		printf("write error ! errno:%d,errmsg:%s\n",errno,strerror(errno));
	}
	len = strlen(str_err_output);
	write(my_stderr,str_err_output,len);
}

void my_write(int fd,const char * data)
{
	if(!data) return;
	size_t len = strlen(data);
	size_t ret_len = write(fd,data,len);
	if(ret_len == -1 || ret_len < len)
	{
		printf("write error ! errno:%d,errmsg:%s\n",errno,strerror(errno));
	}
}

void my_stdin_read()
{
	uint32_t buf_size = 16;
	char buf[buf_size];
	memset(buf,0,buf_size);
	if(!buf) return;
	size_t nread = 0;
	while(-1 != nread)
	{
		nread = read(my_stdin,buf,buf_size);
		printf("read data:%s\n",buf);
		memset(buf,0,buf_size);
		if(nread < buf_size) return;
	}
}

int main(int argc, char const *argv[])
{
	// test_write();
	my_stdin_read();
	return 0;
}