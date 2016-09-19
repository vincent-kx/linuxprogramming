/*
 * #include <unistd.h>
 * int getopt(int argc, char *const argv[], const char *optstring); extern char *optarg;
 * extern int optind, opterr, optopt;
 */

/* The getopt function takes the argc and argv parameters as passed to the program’s main
 * function and an options specifier string that tells getopt what options are defined for
 * the program and whether they have associated values. The optstring is simply a list of
 * characters, each representing a single character option. If a character is followed by 
 * a colon, it indicates that the option has an associated value that will be taken as the
 *  next argument. The getopts command in bash performs a very similar function.
 * For example, the following call would be used to handle the preceding example:
 * getopt(argc, argv, “if:lr”);
 * It allows for simple options -i, -l, -r, and -f, followed by a filename argument. Calling
 * the command with the same parameters but in a different order will alter the behavior. 
 * You can try this out when you get to the sample code in the next Try It Out section.
 * The return result for getopt is the next option character found in the argv array 
 * (if there is one). Call getopt repeatedly to get each option in turn. It has the following behavior:
 
 * ❑ If the option takes a value, that value is pointed to by the external variable optarg.
 * ❑ getopt returns -1 when there are no more options to process. A special argument, --, will cause getopt to stop scanning for options.
 * ❑ getopt returns ? if there is an unrecognized option, which it stores in the external variable optopt.
 * ❑ If an option requires a value (such as -f in our example) and no value is given, getopt nor- mally returns ?. 
 * By placing a colon as the first character of the options string, getopt returns : instead of ? when no value is given.
 
 * The external variable, optind, is set to the index of the next argument to process. getopt 
 * uses it to remember how far it’s got. Programs would rarely need to set this variable. When 
 * all the option arguments have been processed, optind indicates where the remaining arguments 
 * can be found at the end of the argv array.Some versions of getopt will stop at the first
 * non-option argument, returning -1 and setting optind. Others, such as those provided with 
 * Linux, can process options wherever they occur in the program arguments. Note that, in this 
 * case, getopt effectively rewrites the argv array so that all of the non- option arguments 
 * are presented together, starting at argv[optind]. For the GNU version of getopt, this behavior 
 * is controlled by the POSIXLY_CORRECT environment variable. If set, getopt will stop at the 
 * first non-option argument. Additionally, some getopt implementations print error messages 
 * for unknown options. Note that the POSIX specification says that if the opterr variable is 
 * non-zero, getopt will print an error message to stderr.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int opt;
	while(-1 != (opt = getopt(argc,argv,":if:lr")))
	{
		switch(opt)
		{
			case 'i':
			case 'l':
			case 'r':
				printf("option:%c\n",opt);
				break;
			case 'f':
				printf("filename:%s\n",optarg);
				break;
			case ':':
				printf("option '%c' needs a value\n",optopt);
				break;
			case '?':
				printf("unknown option %c\n", optopt);
				break;
		}
	}
	for(; optind < argc; ++optind)
	{
		printf("invalid arguments:%s\n",argv[optind]);
	}
	return 0;
}
