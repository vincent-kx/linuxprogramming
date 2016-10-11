/*
	#include <unistd.h> 
	int close(int fildes);

	#include <fcntl.h> 
	#include <sys/types.h> 
	#include <sys/stat.h>
	int open(const char *path, int oflags);
	int open(const char *path, int oflags, mode_t mode);

	Strictly speaking, you don’t need to include sys/types.h and sys/stat.h 
	to use open on systems that comply with POSIX standards, but they may be 
	necessary on some UNIX systems.

	#include <fcntl.h>
    int creat(const char *path, mode_t mode);
	is the same as open(path, O_CREAT | O_TRUNC | O_WRONLY, mode);

oflags:
	❑ O_RDONLY 		Open for read-only
	❑ O_WRONLY 		Open for write-only
	❑ O_RDWR 			Open for reading and writing
	❑ O_APPEND		Place written data at the end of the file
	❑ O_TRUNC			Set the length of the file to zero, discarding existing contents.
	❑ O_EXCL			Used with O_CREAT, ensures that the caller creates the file. The open is atomic; that is, 
					it’s performed with just one function call. This protects against two programs creating 
					the file at the same time. If the file already exists, open will fail.
	more flags: man 2 open

mode:
	❑ S_IRUSR: Read permission, owner
	❑ S_IWUSR: Write permission, owner
	❑ S_IXUSR: Execute permission, owner
	❑ S_IRGRP: Read permission, group
	❑ S_IWGRP: Write permission, group
	❑ S_IXGRP: Execute permission, group
	❑ S_IROTH: Read permission, others
	❑ S_IWOTH: Write permission, others
	❑ S_IXOTH: Execute permission, others

	#include <unistd.h>
	int ioctl(int fildes, int cmd, ...);
	ioctl performs the function indicated by cmd on the object referenced by the descriptor fildes. 
	It may take an optional third argument, depending on the functions supported by a particular device.

 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>

int my_create_v1(const char * filepath,mode_t mode)
{
	if(!filepath) {
		printf("no file path required\n");
		return -1;
	}
	if(-1 == creat(filepath,mode))
	{
		printf("create file fail,errno:%d,errmsg:%s\n",errno,strerror(errno));
		return -1;
	}
	return 0;
}

int my_create_v2(const char * filepath,mode_t mode)
{
	if(!filepath) {
		printf("no file path required\n");
		return -1;
	}
	int fd = open(filepath,O_CREAT | O_TRUNC | O_WRONLY,mode);
	if(-1 == fd)
	{
		printf("create file fail,errno:%d,errmsg:%s\n",errno,strerror(errno));
		return - 1;
	}
	close(fd);
	return 0;
}

int my_copy(const char * from,const char * to)
{
	mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IROTH | S_IRGRP;
	if(!from || !to)
	{
		printf("need from and to\n");
		return -1;
	}
	int fd1 = open(from,O_RDONLY);
	if(-1 == fd1)
	{
		printf("open file : %s fail\n,errno:%d,errmsg:%s\n",from,errno,strerror(errno));
		return -1;
	}
	int fd2 = open(to,O_CREAT | O_TRUNC | O_WRONLY,mode);
	if(-1 == fd2)
	{
		printf("open file : %s fail\n,errno:%d,errmsg:%s\n",to,errno,strerror(errno));
		return -1;
	}
	char buf[256];
	memset(buf,0,256);
	size_t nread = 0;
	while((nread = read(fd1,buf,256)) > 0)
	{
		if(write(fd2,buf,nread) == -1)
		{
			printf("copy fail,errno:%d,errmsg:%s\n",errno,strerror(errno));
			break;
		}
	}
	close(fd1);
	close(fd2);
	if(nread == -1)
	{
		printf("copy fail,read error,errno:%d,errmsg:%s\n",errno,strerror(errno));
		return -1;
	}
	return 0;
}

int main(int argc, char const * argv[])
{
	/*mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IROTH | S_IRGRP;
	const char * filepath1 = argv[1];
	const char * filepath2 = argv[2];
	my_create_v1(filepath1,mode);
	my_create_v2(filepath2,mode);*/
	my_copy(argv[1],argv[2]);
	return 0;
}