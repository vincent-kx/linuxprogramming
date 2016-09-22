#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <sys/time.h>
#include <errno.h>
#include <memory.h>
/*
	the most commonly used time structs:
	time_t,struct tm,struct timeval,struct timespec
 */

/***************************************************
1)time_t:
		#ifndef __TIME_T  
		#define __TIME_T  
		typedef  long  time_t;  
		#endif

functions:
		(1)time_t time( time_t *arg );
		(2)double difftime( time_t time_end, time_t time_beg )
		(3)char* ctime( const time_t* time ); //identical to asctime(localtime(&t))
		(4)errno_t ctime_s(char *buffer, rsize_t bufsz, const time_t *time);
----------------------------------------------------
struct tm:
		#ifndef _TM_DEFINED  
		struct tm{  
		    int tm_sec; 	//秒 - 取值区间为[0, 59]  
		    int tm_min; 	//分 - 取值区间为[0, 59]
		    int tm_hour; 	//时 - 取值区间为[0, 23]
		    int tm_mday; 	//日 - 取值区间为[1, 31]
		    int tm_mon; 	//月份 - 取值区间为[0, 11]
		    int tm_year; 	//年份 - 其值为1900年至今年数
		    int tm_wday; 	//星期 - 取值区间[0, 6]，0代表星期天，1代表星期1，以此类推
		    int tm_yday; 	//从每年的1月1日开始的天数-取值区间为[0, 365]，0代表1月1日 
		    int tm_isdst; 	//夏令时标识符，使用夏令时，tm_isdst为正，不使用夏令时，tm_isdst为0，不了解情况时，tm_isdst为负 
		};  
#define _TM_DEFINED  
#endif

functions:
		<time_t ---> struct tm>
		(1)struct tm *gmtime( const time_t *time );
		(2)struct tm *gmtime_s(const time_t *restrict time, struct tm *restrict result); (since c11)
		(3)struct tm *localtime( const time_t *time );
		(4)struct tm *localtime_s(const time_t *restrict time, struct tm *restrict result);
		<struct tm ---> time_t>
		(5)time_t mktime( struct tm *time );
		<struct tm ---> string>
		(6)char* asctime( const struct tm* time_ptr );
		(7)errno_t asctime_s(char *buf, rsize_t bufsz, const struct tm *time_ptr);
		(8)size_t strftime( char *str, size_t count,const char *format, const struct tm *time );
		(9)size_t strftime( char *restrict str, size_t count,const char *restrict format, const struct tm *restrict time);
		<string ---> struct tm>
		(10) char *strptime(const char *s, const char *format, struct tm *tm);
----------------------------------------------------
timeval:
		struct tmieval{  
		    time_t tv_sec;
		    suseconds_t tv_usec;
		}; 
		struct timezone			 //defined in <sys/time.h>,time.h and sys/time.h are different
		{
		    int tz_minuteswest;  //和Greenwich 时间差了多少分钟
		    int tz_dsttime;  	 //日光节约时间的状态
		};

		DST_NONE  //不使用
	    DST_USA  //美国
	    DST_AUST  //澳洲
	    DST_WET  //西欧
	    DST_MET  //中欧
	    DST_EET  //东欧
	    DST_CAN  //加拿大
	    DST_GB  //大不列颠
	    DST_RUM  //罗马尼亚
	    DST_TUR  //土耳其
	    DST_AUSTALT  //澳洲(1986 年以后)
functions:
		(1)int gettimeofday (struct timeval * tv, struct timezone * tz);
----------------------------------------------------
timespec:
		struct timespec{  
		    time_t tv_sec;
		    long tv_nsec; 
		}; 
functions:
		(1)int clock_gettime(clockid_t clk_id, struct timespect *tp);
			CLOCK_REALTIME, a system-wide realtime clock.
			CLOCK_PROCESS_CPUTIME_ID, high-resolution timer provided by the CPU for each process.
			CLOCK_THREAD_CPUTIME_ID, high-resolution timer provided by the CPU for each of the threads. 
		(2)void nsec2timespec( struct timespec *timespec_p, uint64_t nsec);
		(3)uint64_t timespec2nsec( const struct timespec* ts )

***************************************************/

int main(int argc, char const *argv[])
{
	time_t t1 = time(NULL);
	sleep(1);
	time_t t2 = time(NULL);
	printf("time elapse:%f\n",difftime(t2,t1));
	//=========================================================
	printf("time second now:%u\n",time(NULL));
	time_t tmsec_now;
	time(&tmsec_now);
	printf("time second now:%u\n",tmsec_now);
	printf("ctime from time_t:%s\n",ctime(&tmsec_now));
	//=========================================================
	struct tm * tm_val1 = gmtime(&tmsec_now);
	printf("utc datetime now: %d-%d-%d %d:%d:%d\n", 1900+tm_val1->tm_year,tm_val1->tm_mon+1,tm_val1->tm_mday,tm_val1->tm_hour,tm_val1->tm_min,tm_val1->tm_sec);
    struct tm * tm_val2 = localtime(&tmsec_now);
    printf("local datetime now: %d-%d-%d %d:%d:%d\n", 1900+tm_val2->tm_year,tm_val2->tm_mon+1,tm_val2->tm_mday,tm_val2->tm_hour,tm_val2->tm_min,tm_val2->tm_sec);
    printf("time_t from struct tm:%u\n",mktime(tm_val1));
    printf("readable time from struct tm:%s",asctime(tm_val1));
    //=========================================================
    char str_date[32] = {0};
 	strftime(str_date,sizeof(str_date),"%Y-%m-%d %H:%M:%S",tm_val1);
    printf("strftime:%s\n",str_date);
    //=========================================================
    struct tm tm_val3;
    memset((char *)&tm_val3,0,sizeof(tm_val3));
    strptime(str_date,"%Y-%m-%d",&tm_val3);
    printf("truncated time:%u\n", mktime(&tm_val3));
    //=========================================================
    struct timeval tv;
    // struct timezone tz;
    int ret = gettimeofday (&tv, /*&tz*/NULL);
    if(ret == 0)
    {
	    printf("tv_sec: %d\n", tv.tv_sec);
	    printf("tv_usec: %d\n", tv.tv_usec);
	    // printf("tz_minuteswest; %d\n", tz.tz_minuteswest);
	    // printf("tz_dsttime, %d\n", tz.tz_dsttime);
    }
    if(ret == -1)
    {
    	printf("gettimeofday fail,errno:%d\n", errno);
    }
    //=========================================================
    //gcc -o time time.c -lrt
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME,&ts);
    printf("timespec sec:%lu\n",ts.tv_sec);
    printf("timespec nsec:%lu\n",ts.tv_nsec);
    printf("nano second now:%lu\n",ts.tv_sec * 1000000000 + ts.tv_nsec);
	return 0;
}