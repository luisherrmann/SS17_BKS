//Compile with gcc -std=c11 -Wall -Wextra -pedantic -o client client.c -lrt -pthread
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// alte includes
#include <sys/stat.h>
#include <sys/mman.h>

#include <sys/types.h>
#include <inttypes.h>
#include <unistd.h>
#include <strings.h>

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/mman.h>

#define BUF_SIZE 512

char BUFFER[BUF_SIZE];
int connect_fileserver_udp(){
	const char tempAddrString[] = "127.0.0.1";
	//inet_aton(tempAddrString,ip4_addr);
	int udp_socket_fd = socket(AF_INET,SOCK_DGRAM,17); // AR_INET for IPv4, SOCK_DGRAM for Datagram, 17 for UDP
	struct sockaddr_in my_sock_addr;// = malloc(sizeof(struct sockaddr_in));
	bzero(&my_sock_addr,sizeof(struct sockaddr_in));	
	my_sock_addr.sin_family = AF_INET;
	my_sock_addr.sin_port = htons(50000); //nicht notwendig
	inet_aton(tempAddrString,&my_sock_addr.sin_addr.s_addr);
	char *message = "Hello World!";	
	strcpy(BUFFER,message);
	size_t send_byte = 0;
	while (send_byte < strlen(message)){
		send_byte += sendto(udp_socket_fd,BUFFER, strlen(BUFFER),MSG_MORE,(struct sockaddr*)&my_sock_addr,sizeof(struct 		sockaddr_in));	
	}
	send_byte += sendto(udp_socket_fd,(void*)&"\0", 1,MSG_CONFIRM,(struct sockaddr*)&my_sock_addr,sizeof(struct 				sockaddr_in));
	recvfrom(udp_socket_fd,(void*)&"\0", 1,MSG_WAITALL,(struct sockaddr*)&my_sock_addr, sizeof(struct 				sockaddr_in));	
	printf("%s\n",BUFFER);
	close(udp_socket_fd);

	return 0;
}
int main(int argc, char *argv[]){

	connect_fileserver_udp();
	return EXIT_SUCCESS;
}

