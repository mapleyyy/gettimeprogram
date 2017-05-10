/****************************************************/
/************* datetime Example Client **************/
/****************************************************/

#include "datetime.h"
#include <time.h>
#include <linux/tcp.h>
#include <sys/socket.h>
int main( int argc , char * * argv )
{
	int sockfd , n ;
	struct tcp_info info;
	char recvline[ MAXLINE ];
	struct sockaddr_in servaddr;
	time_t ticks;

	if( argc != 2 )  {
		printf( "usage : a.out <IP address>\n" );
		exit( 1 );
	}

	if( ( sockfd = socket( AF_INET , SOCK_STREAM , 0 ) ) < 0 ) {
		printf( "socket error\n" );
		exit( 1 );
	}

	memset( &servaddr , 0 , sizeof( servaddr ) );
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons( 13 );

	int nRecvBufLen=16;
	setsockopt( sockfd, SOL_SOCKET, SO_RCVBUF, ( const char* )&nRecvBufLen, sizeof( int ) );
	if( inet_pton( AF_INET , argv[ 1 ] , &servaddr.sin_addr ) <= 0 )  {
		printf( "inet_pton error for %s\n" , argv[ 1 ] );
		exit( 1 ); 
	}

	if( connect( sockfd , (struct sockaddr *)&servaddr , sizeof( servaddr ) ) < 0 )  {
		perror("connect");
		printf( "connect error\n" );
		exit( 1 );
	}

	socklen_t len=sizeof(info);
	struct timeval time_val;
	int num;
	int p=0;	
	while( ( n = read( sockfd , &time_val , MAXLINE ) > 0 ))  {
		getsockopt(sockfd,IPPROTO_TCP,TCP_INFO,&info,&len);
		num=info.tcpi_rtt;//
		printf("%d %d %d %d\n",num,time_val.tv_sec,time_val.tv_usec,p++);
		time_val.tv_usec+=num/2;
		settimeofday(&time_val,NULL);
		ticks=time_val.tv_sec;
		
		snprintf( recvline , sizeof( recvline ) , "%.24s\r\n" , ctime( &ticks ) );
		if( fputs( recvline , stdout )== EOF ) {
			
			printf( "fputs error\n" );
			exit( 1 );
		}
	}
	if(n<0)
	printf("read error\n");
	if( n < 0 )  {
		printf( "read error\n" );
		exit( 1 );
	}
	exit( 0 );  
}
