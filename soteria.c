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


int recv_block(int sock, void *buffer, int length){
    //unsigned char *head_buffer = malloc(sizeof(unsigned char));
    //unsigned char *end_buffer = malloc(sizeof(unsigned char)*3);
    unsigned char head_buffer[1];
    int r = 0;
    int ret = 0;


    //printf("asking for %i bytes\n", length);

    while (1){
        ret = recv(sock, &head_buffer, 1, 0);
        //printf("%s", head_buffer);
        //printf("Got: %s\n", head_buffer);
        if (ret < 0){
            printf("Error\n");
            return -1;
        }
        if (head_buffer[0] == '\r') {
    //      bzero(head_buffer, 1);
            ret = recv(sock, &head_buffer, 1, 0);
        //printf("%s", head_buffer);
            if (head_buffer[0] == '\n') {
    //          bzero(head_buffer, 1);
                ret = recv(sock, &head_buffer, 1, 0);
        //printf("%s", head_buffer);
            }
            if (head_buffer[0] == '\r') {
    //          bzero(head_buffer, 1);
                ret = recv(sock, &head_buffer, 1, 0);
        //printf("%s", head_buffer);
            }
            if (head_buffer[0] == '\n') {
            //  printf("Found End of Header\n");
                break;
            }
        }


    }


    while (r < length){
        ret = recv(sock, buffer+r, length - r, 0);
        if (ret < 0){
            printf("Error\n");
            return -1;
        }
        r += ret;
    }
    //free(head_buffer);
    //free(end_buffer);
    //close(sock);

    return r;



}


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

void merge_arrays(int a[], int low, int mid, int high)
{
    int temp[10000];
    int i = low, j = mid + 1, k = 0;
  
    while (i <= mid && j <= high) {
        if (a[i] <= a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }
    while (i <= mid)
        temp[k++] = a[i++];
  
    while (j <= high)
        temp[k++] = a[j++];
  
    k--;
    while (k >= 0) {
        a[low + k] = temp[k];
        k--;
    }
}
  
void merge_sort(int a[], int low, int high)
{
    if (low < high) {
        int m = (high + low)/2;
        merge_sort(a, low, m);
        merge_sort(a, m + 1, high);
        merge_arrays(a, low, m, high);
    }
}
