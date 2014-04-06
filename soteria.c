/*
   Soteria

*/
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/crypto.h>
#include <openssl/bn.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <netdb.h>
#include <curl/curl.h>
#include <pthread.h>


void *connection_handler(int newsockfd){


	/*	Handle new connections Here */

	close(newsockfd);
	pthread_exit(NULL);
}

int main( int argc, char *argv[] )
{

	if (argc < 2) {
		printf("Usage: $Executable PortNumber Ordering\n\
			Executable: The Name of the executable\n\
			Port Number\n");
		return 1;
	}



	int sockfd, newsockfd, portno, clilen;
	//char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int  n,pid;
	int flag = 1;
	//POR_REQUEST = malloc(sizeof(struct request));
	/* First call to socket() function */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	{
		perror("ERROR opening socket");
		exit(1);
	}
	/* Initialize socket structure */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	/* Now bind the host address using bind() call.*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
		perror("ERROR on binding");
		exit(1);
	}

	/* Now start listening for the clients, here 
	 * process will go in sleep mode and will wait 
	 * for the incoming connection
	 */
	//listen(sockfd,5);
	//clilen = sizeof(cli_addr);
	while (1){
		printf("Waiting for Connection on 8888\n");
		listen(sockfd,5);
		clilen = sizeof(cli_addr);
		//flag = 1;

		pthread_t * sniffer_thread;
		//sniffer_thread = (pthread_t)malloc(sizeof(pthread_t));
		sniffer_thread = malloc(sizeof(pthread_t));

		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0){
			perror("ERROR on accept");
			//exit(1);
		}

		pthread_create(sniffer_thread, NULL, connection_handler, (void*)newsockfd);

		//to test
		free(sniffer_thread);



	}


	close(sockfd);
	return 1;

}
