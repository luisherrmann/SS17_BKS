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

#define MSG_LEN 500

struct shared_mem{
	int len;
	char buf[MSG_LEN];
};

struct shared_mem *shm;
char BUFFER[MSG_LEN];

int connect_fileserver_udp(){
	const char tempAddrString[] = "127.0.0.1";
	int udp_socket_fd = socket(AF_INET,SOCK_DGRAM,17); // AR_INET for IPv4, SOCK_DGRAM for Datagram, 17 for UDP
	struct sockaddr_in my_sock_addr;// = malloc(sizeof(struct sockaddr_in));
	bzero(&my_sock_addr,sizeof(struct sockaddr_in));	
	my_sock_addr.sin_family = AF_INET;
	my_sock_addr.sin_port = htons(50000); //nicht notwendig
	inet_aton(tempAddrString,&my_sock_addr.sin_addr);//.s_addr);
	char *message = "Hello World!";	
	strcpy(BUFFER,message);
	size_t send_byte = 0;
	while (send_byte < strlen(message)){
		send_byte += sendto(udp_socket_fd,BUFFER, strlen(BUFFER),MSG_MORE,(struct sockaddr*)&my_sock_addr,sizeof(struct 		sockaddr_in));	
	}
	send_byte += sendto(udp_socket_fd,(void*)&"\0", 1,MSG_CONFIRM,(struct sockaddr*)&my_sock_addr,sizeof(struct 				sockaddr_in));
	int cur_recv;

	do {	
		cur_recv += recvfrom(udp_socket_fd,(void*)&"\0", 1,MSG_WAITALL,(struct sockaddr*)&my_sock_addr, sizeof(struct 				sockaddr_in));
		if (cur_recv>-1){
			printf("%s",BUFFER);
		}		
		
	}while (cur_recv!=0 || cur_recv==-1);
	printf("\n");
	close(udp_socket_fd);

	return 0;
}
int main(int argc, char *argv[]){
	if (argc <= 1 || argc > 4){
		const char message[] ="Client benötigt drei Parameter.\n";
		write(STDERR_FILENO,message,sizeof(message));
		return EXIT_FAILURE;
	}
	if (argv[1][0]=='-'){
		if (argv[1][1]=='U'){
			connect_fileserver_unix(argv[2);
		}else if (argv[1][1]=='u'){
			connect_fileserver_udp(argv[2);
		}else if (argv[1][1]=='t'){
			connect_fileserver_tcp(argv[2);
		}else{
			const char message[] ="Unbekanntes Protokoll.\n";
			write(STDERR_FILENO,message,sizeof(message));
			return EXIT_FAILURE;
		}
	}
	connect_fileserver_udp();
	return EXIT_SUCCESS;
}

