/*
	#include <unistd.h> 
	#include <sys/types.h>
	off_t lseek(int fildes, off_t offset, int whence);

	The offset parameter is used to specify the position, 
	and the whence parameter specifies how the offset is used. 
	whence can be one of the following:

	❑ SEEK_SET: offset is an absolute position
	❑ SEEK_CUR: offset is relative to the current position
	❑ SEEK_END: offset is relative to the end of the file

	lseek returns the offset measured in bytes from the beginning of the file that the file pointer is set to, 
	or –1 on failure. The type off_t, used for the offset in seek operations, is an implementation-dependent 
	integer type defined in sys/types.h.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
	int fd = open("syscall_lseek.c",O_RDONLY);
	if(-1 == fd)
	{
		printf("open file fail,errno:%d,errmsg:%s\n",errno,strerror(errno));
		return -1;
	}
	char buf[64];
	memset(buf,0,64);

	off_t pos = lseek(fd,100,SEEK_SET);
	size_t nread = read(fd,buf,63);
	if(-1 == nread)
	{
		printf("read fail,errno:%d,errmsg:%s\n",errno,strerror(errno));
		return -1;
	}
	printf("read data:%s\n",buf);

	pos = lseek(fd,-100,SEEK_END);
	nread = read(fd,buf,63);
	write(2,buf,nread);
	write(2,"\n",1);
	return 0;
}