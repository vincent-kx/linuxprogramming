/*
	http://man7.org/linux/man-pages/man2/stat.2.html

	#include <unistd.h> 
	#include <sys/stat.h> 
	#include <sys/types.h>
	int fstat(int fildes, struct stat *buf);
	int stat(const char *path, struct stat *buf); 
	int lstat(const char *path, struct stat *buf);

	Note that the inclusion of sys/types.h is optional, but we recommend it when using system calls, 
	because some of their definitions use aliases for standard types that may change one day.

	The related functions stat and lstat return status information for a named file. 
	They produce the same results, except when the file is a symbolic link. 
	lstat returns information about the link itself, and stat returns information 
	about the file to which the link refers.

	struct stat {
               dev_t     st_dev;         // ID of device containing file 
               ino_t     st_ino;         // inode number 
               mode_t    st_mode;        // file type and mode 
               nlink_t   st_nlink;       // number of hard links 
               uid_t     st_uid;         // user ID of owner 
               gid_t     st_gid;         // group ID of owner 
               dev_t     st_rdev;        // device ID (if special file) 
               off_t     st_size;        // total size, in bytes 
               blksize_t st_blksize;     // blocksize for filesystem I/O 
               blkcnt_t  st_blocks;      // number of 512B blocks allocated 

               Since Linux 2.6, the kernel supports nanosecond
                  precision for the following timestamp fields.
                  For the details before Linux 2.6, see NOTES.

               struct timespec st_atim;   time of last access 
               struct timespec st_mtim;   time of last modification 
               struct timespec st_ctim;   time of last status change 

	           #define st_atime st_atim.tv_sec       //Backward compatibility 
	           #define st_mtime st_mtim.tv_sec
	           #define st_ctime st_ctim.tv_sec
           };

       Note: the order of fields in the stat structure varies somewhat
       across architectures.  In addition, the definition above does not
       show the padding bytes that may be present between some fields on
       various architectures.  Consult the glibc and kernel source code if
       you need to know the details.

       Note: For performance and simplicity reasons, different fields in the
       stat structure may contain state information from different moments
       during the execution of the system call.  For example, if st_mode or
       st_uid is changed by another process by calling chmod(2) or chown(2),
       stat() might return the old st_mode together with the new st_uid, or
       the old st_uid together with the new st_mode.

       S_IFMT     0170000   bit mask for the file type bit field

       S_IFSOCK   0140000   socket
       S_IFLNK    0120000   symbolic link
       S_IFREG    0100000   regular file
       S_IFBLK    0060000   block device
       S_IFDIR    0040000   directory
       S_IFCHR    0020000   character device
       S_IFIFO    0010000   FIFO
	
	test macros:
       S_ISREG(m)  is it a regular file?
       S_ISDIR(m)  directory?
       S_ISCHR(m)  character device?
       S_ISBLK(m)  block device?
       S_ISFIFO(m) FIFO (named pipe)?
       S_ISLNK(m)  symbolic link?  (Not in POSIX.1-1996.)
       S_ISSOCK(m) socket?  (Not in POSIX.1-1996.)
	
	The following mask values are defined for the file mode component of
    the st_mode field:

           S_ISUID     04000   set-user-ID bit
           S_ISGID     02000   set-group-ID bit (see below)
           S_ISVTX     01000   sticky bit (see below)

           S_IRWXU     00700   owner has read, write, and execute permission
           S_IRUSR     00400   owner has read permission
           S_IWUSR     00200   owner has write permission
           S_IXUSR     00100   owner has execute permission

           S_IRWXG     00070   group has read, write, and execute permission
           S_IRGRP     00040   group has read permission
           S_IWGRP     00020   group has write permission
           S_IXGRP     00010   group has execute permission

           S_IRWXO     00007   others (not in group) have read, write, and
                               execute permission
           S_IROTH     00004   others have read permission
           S_IWOTH     00002   others have write permission
           S_IXOTH     00001   others have execute permission
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	struct stat sb;
	if(argc != 2)
	{
		fprintf(stderr,"usage: %s <pathname>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	const char * pathname = argv[1];
	if(-1 == stat(pathname,&sb))
	{
		fprintf(stderr, "stat execute fail,pathname:%s\n",pathname);
		exit(EXIT_FAILURE);
	}

	printf("file type:");
	switch(sb.st_mode & S_IFMT)
	{
		case S_IFBLK : printf("block device\n"); break;
		case S_IFCHR : printf("character device\n"); break;
		case S_IFDIR : printf("directory\n"); break;
		case S_IFIFO : printf("FIFO pipe\n"); break;
		case S_IFLNK : printf("symlink\n"); break;
		case S_IFREG : printf("regular file\n"); break;
		case S_IFSOCK : printf("socket\n"); break;
		default : printf("unknown file type\n");
	}
	printf("I-node number:%ld\n",(long)sb.st_ino);
	printf("Mode:%lo(octal)\n",(unsigned long)sb.st_mode);
	printf("Prefered I/O block size:%ld bytes\n",(long)sb.st_blksize);
	printf("file size:%lld\n", (long long)sb.st_size);
	printf("Blocks allocated:%lld\n", (long long)sb.st_blocks);

	printf("Last status change:%s",ctime(&sb.st_ctime));
	printf("Last file access:%s",ctime(&sb.st_atime));
	printf("Last file modification:%s",ctime(&sb.st_mtime));

	printf("File user uid:%ld\n", (long)sb.st_uid);
	printf("File user group id:%ld\n",(long)sb.st_gid);

	if(S_ISDIR(sb.st_mode))
	{
		printf("directory\n");
	}
	else
	{
		printf("not directory\n");
	}
	if(sb.st_mode & S_IRWXU == S_IXUSR | S_IRUSR | S_IRWXU)
	{
		printf("executable file,%ld,%ld\n",sb.st_mode & S_IRWXU,S_IXUSR);
	}
	else
	{
		printf("not executable file,%ld,%ld\n",sb.st_mode & S_IRWXU,S_IXUSR);
	}

	exit(EXIT_SUCCESS);

	return 0;
}