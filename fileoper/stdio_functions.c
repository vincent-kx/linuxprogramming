//the following functions are ANSI standarded.detail explain can be found in mac app dash
/*
header:	#include <stdio.h>

FILE *fopen(const char *filename, const char *mode);

	fopen opens the file named by the filename parameter and associates a stream with it. The mode parameter specifies how the file is to be opened. It’s one of the following strings:
	❑ “r” or “rb”: Open for reading only
	❑ “w” or “wb”: Open for writing, truncate to zero length
	❑ “a” or “ab”: Open for writing, append to end of file
	❑ “r+” or “rb+” or “r+b”: Open for update (reading and writing)
	❑ “w+” or “wb+” or “w+b”: Open for update, truncate to zero length
	❑ “a+” or “ab+” or “a+b”: Open for update, append to end of file

size_t fread(void *ptr, size_t size, size_t nitems, FILE *stream);

	The fread library function is used to read data from a file stream. Data is read into a data buffer given by ptr from the stream, stream. Both fread and fwrite deal with data records. These are specified by a record size, size, and a count, nitems, of records to transfer. The function returns the number of items (rather than the number of bytes) successfully read into the data buffer. At the end of a file, fewer than nitems may be returned, including zero.

size_t fwrite (const void *ptr, size_t size, size_t nitems, FILE *stream);

	The fwrite library call has a similar interface to fread. It takes data records from the specified data buffer and writes them to the output stream. It returns the number of records successfully written.

	Note that fread and fwrite are not recommended for use with structured data. Part of the problem is that files written with fwrite are potentially not portable between different machine architectures.

int fclose(FILE *stream);
int fflush(FILE *stream);

int fseek(FILE *stream, long int offset, int whence);

	The fseek function is the file stream equivalent of the lseek system call. It sets the position in the stream for the next read or write on that stream. The meaning and values of the offset and whence parameters are the same as those we gave previously for lseek. However, where lseek returns an off_t, fseek returns an integer: 0 if it succeeds, –1 if it fails, with errno set to indicate the error. 


int fgetc(FILE *stream); 
int getc(FILE *stream); 			//is equivalent to fgetc, but it may be implemented as a macro.
int getchar(); 						//same as fget(stdin);
int fputc(int c, FILE *stream); 
int putc(int c, FILE *stream); 		//is equivalent to fputc, but it may be implemented as a macro.
int putchar(int c);					//same as fput(c,stdout);



char *fgets(char *s, int n, FILE *stream); 
char *gets(char *s);

	fgets writes characters to the string pointed to by s until a newline is encountered, n-1 characters have been transferred, or the end of file is reached, whichever occurs first. Any newline encountered is transferred to the receiving string and a terminating null byte, \0, is added. Only a maximum of n-1 characters are transferred in any one call because the null byte must be added to mark the end of the string and bring the total up to n bytes.

	When it successfully completes, fgets returns a pointer to the string s. If the stream is at the end of a file, it sets the EOF indicator for the stream and fgets returns a null pointer. If a read error occurs, fgets returns a null pointer and sets errno to indicate the type of error.
	
	The gets function is similar to fgets, except that it reads from the standard input and discards any newline encountered. It adds a trailing null byte to the receiving string.
	
	Note that gets doesn’t limit the number of characters that can be transferred so it could overrun its transfer buffer. Consequently, you should avoid using it and use fgets instead. Many security issues can be traced back to functions in programs that are made to overflow a buffer of some sort or another. This is one such function, so be careful!
 */

/*
test.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char * argv[])
{
	char * filename = "test.txt";
	FILE * fp = fopen(filename,"a+");
	if(!fp)
	{
		perror("open file fail !");
		exit(EXIT_FAILURE);
	}
	
	char * line = "this is a new line\n";
	int nwrite = fwrite(line,strlen(line)+1,1,fp);
	if(nwrite < 1)
	{
		fprintf(stderr, "write file fail\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		fprintf(stdout, "write success\n");
	}
	fflush(fp);

	fseek(fp,SEEK_SET,0); // move fp to begin
	char buf[256] = {0};
	size_t nread = fread(buf,sizeof(buf),1,fp);
	if(nread < 0)
	{
		printf("read fail\n");
		exit(EXIT_FAILURE);
	}
	printf("read data:%s\n",buf);
	fclose(fp);
	return 0;
}


