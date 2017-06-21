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
#define PORT 1502
char BUFFER[BUF_SIZE];
int connect_fileserver_unix(char *server_address, char *filepath){
	return 0;
}
int connect_fileserver_tcp(char *server_address, char *filepath){
	int tcp_socket_fd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in my_sock_addr;
	bzero(&my_sock_addr,sizeof(struct sockaddr_in));	
	my_sock_addr.sin_family = AF_INET;
	my_sock_addr.sin_port = htons(PORT); 
	inet_aton(server_address,&my_sock_addr.sin_addr);
	strcpy(BUFFER,filepath);
	connect(tcp_socket_fd,(const struct sockaddr*)&my_sock_addr, sizeof(struct sockaddr)); 
	
	size_t send_byte = 0;
	while (send_byte < strlen(filepath)){
		send_byte += send(tcp_socket_fd,BUFFER,strlen(BUFFER),MSG_MORE);
	}
	send_byte += send(tcp_socket_fd,"\0",1,MSG_CONFIRM);
	int cur_recv;

	do {		
		cur_recv += recv(tcp_socket_fd,BUFFER,strlen(BUFFER),0);
		if (cur_recv>-1){
			printf("%s",BUFFER);
		}		
		
	}while (cur_recv!=0 || cur_recv==-1);
	printf("\n");
	close(tcp_socket_fd);
	return 0;
}
int connect_fileserver_udp(char *server_address, char *filepath){
	int udp_socket_fd = socket(AF_INET,SOCK_DGRAM,17); // AR_INET for IPv4, SOCK_DGRAM for Datagram, 17 for UDP
	struct sockaddr_in my_sock_addr;
	bzero(&my_sock_addr,sizeof(struct sockaddr_in));	
	my_sock_addr.sin_family = AF_INET;
	my_sock_addr.sin_port = htons(PORT); 
	inet_aton(server_address,&my_sock_addr.sin_addr);
	strcpy(BUFFER,filepath);
	size_t send_byte = 0;
	while (send_byte < strlen(filepath)){
		send_byte += sendto(udp_socket_fd,BUFFER, strlen(BUFFER),MSG_MORE,(const struct sockaddr*)&my_sock_addr,sizeof(struct 		sockaddr));	
	}
	send_byte += sendto(udp_socket_fd,(void*)&"\0", 1,MSG_CONFIRM,(struct sockaddr*)&my_sock_addr,sizeof(struct 				sockaddr));
	int cur_recv;

	do {	
		cur_recv += recvfrom(udp_socket_fd,BUFFER, strlen(BUFFER),MSG_WAITALL,(struct sockaddr*)&my_sock_addr, NULL);
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
			connect_fileserver_unix(argv[2],argv[3]);
		}else if (argv[1][1]=='u'){
			connect_fileserver_udp(argv[2],argv[3]);
		}else if (argv[1][1]=='t'){
			connect_fileserver_tcp(argv[2],argv[3]);
		}else{
			const char message[] ="Unbekanntes Protokoll.\n";
			write(STDERR_FILENO,message,sizeof(message));
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

