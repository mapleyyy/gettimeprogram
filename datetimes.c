/****************************************************/
/************* datetime Example Server **************/
/****************************************************/
#include "datetime.h"
#include <time.h>


int
main( int argc , char * * argv )
{
	int listenfd , connfd;
	struct sockaddr_in servaddr;
	char buff[ MAXLINE ];
	
	struct timeval time_val;

	listenfd = socket( AF_INET , SOCK_STREAM , 0 );

	memset( &servaddr , 0 , sizeof( servaddr ) );
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl( INADDR_ANY );
	servaddr.sin_port = htons( 13 );

	int nSendBufLen = 32*1024; 
	setsockopt( listenfd, SOL_SOCKET, SO_SNDBUF, ( const char* )&nSendBufLen, sizeof( int ) );
	bind( listenfd , (struct sockaddr *)&servaddr , sizeof( servaddr ) );
	listen( listenfd , 1024 );

	for( ; ; )
	{
		int pid;
		connfd = accept( listenfd , (struct sockaddr *)NULL , NULL );
		pid=fork();
		if(pid==0){
			close(listenfd);
			for(int i=0;i<1000000;i++)
			{
				time_t ticks;
				ticks = time( NULL );
				gettimeofday(&time_val,NULL);
				snprintf( buff , sizeof( buff ) , "%.24s\r\n" , ctime( &ticks ) );
				printf(buff);
				write( connfd , &time_val , sizeof( time_val ) );
			}
			close(connfd);
			exit(0);
		}
		close( connfd );
	} 
}
