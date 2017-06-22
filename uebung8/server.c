//Compile with gcc -std=c11 -Wall -Wextra -pedantic -o client client.c -lrt -pthread
#define _DEFAULT_SOURCE //Aktiviert glibc-Erweiterungen
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 1502
#define BUF_SIZE 512
struct data_packet{
	size_t len;
	char buffer[BUF_SIZE];
};
struct data_packet *my_data;
int start_fileserver_co(char *server_address, int socketdomain)
{	
	int co_socket_fd = socket(socketdomain,SOCK_STREAM,0);
	struct sockaddr_in server_sock_addr;
	struct sockaddr_in client_sock_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	bzero(&client_sock_addr,sizeof(struct sockaddr_in));	
	memset(&server_sock_addr, '\0', sizeof(struct sockaddr_in));
	server_sock_addr.sin_family = AF_INET;
	server_sock_addr.sin_port = htons(PORT);
	server_sock_addr.sin_addr.s_addr = inet_addr(server_address);
	socklen_t addlen = sizeof(server_sock_addr);


	if(bind(co_socket_fd, (struct sockaddr *) &server_sock_addr, addlen) == -1){
		fprintf(stderr, "Error binding to port");
		return 0;
	}
	listen(co_socket_fd,3);
	
	my_data = (struct data_packet*)malloc(sizeof(struct data_packet));
	bzero(my_data->buffer,BUF_SIZE);
	int client_socket = accept(co_socket_fd,(struct sockaddr *)&client_sock_addr, &addrlen);

	//Lese solange Socket nicht leer ist.
	recv(client_socket, my_data->buffer, BUF_SIZE,0);

	fprintf(stdout, "Message received: %s\n", my_data->buffer);
	bzero(my_data->buffer, BUF_SIZE);
	strcpy(my_data->buffer,"Hello World from server!\n");
	size_t send_byte = 0;
	my_data->len=strlen(my_data->buffer)+sizeof(size_t);
	while(send_byte < my_data->len){
		send_byte += send(client_socket, my_data, my_data->len,MSG_MORE); 
	}
	send_byte += send(co_socket_fd, "\0", 1,MSG_CONFIRM); 
	printf("Send %i\n",send_byte);
	close(client_socket);
	close(co_socket_fd);
	return 0;
}


int start_fileserver_udp(char *server_address){
	int udp_socket_fd = socket(AF_INET,SOCK_DGRAM,17);
	struct sockaddr_in server_sock_addr;
	struct sockaddr_in client_sock_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	bzero(&client_sock_addr,sizeof(struct sockaddr_in));	
	memset(&server_sock_addr, '\0', sizeof(struct sockaddr_in));
	server_sock_addr.sin_family = AF_INET;
	server_sock_addr.sin_port = htons(PORT);
	server_sock_addr.sin_addr.s_addr = inet_addr(server_address);
	socklen_t addlen = sizeof(server_sock_addr);
	if(bind(udp_socket_fd, (struct sockaddr *) &server_sock_addr, addlen) == -1){
		fprintf(stderr, "Error binding to port");
		return 0;
	}
	listen(udp_socket_fd,3);

	my_data = (struct data_packet*)malloc(sizeof(struct data_packet));
	bzero(my_data->buffer,BUF_SIZE);
	
	//Lese solange Socket nicht leer ist.
	recvfrom(udp_socket_fd, my_data->buffer, BUF_SIZE,MSG_WAITALL, (struct sockaddr *)&client_sock_addr, &addrlen);
	fprintf(stdout, "Message received: %s\n", my_data->buffer);
	bzero(my_data->buffer, BUF_SIZE);
	strcpy(my_data->buffer,"Hello World from server!\n");
	size_t send_byte = 0;
	my_data->len=strlen(my_data->buffer)+sizeof(size_t);
	while(send_byte < my_data->len){
		send_byte += sendto(udp_socket_fd, my_data, my_data->len,MSG_MORE, (struct sockaddr*) &client_sock_addr, addrlen); 
	}
	send_byte += sendto(udp_socket_fd, "\0", 1,MSG_CONFIRM, (struct sockaddr*) &client_sock_addr, addrlen); 
	printf("Send %i\n",send_byte);
	close(udp_socket_fd);
	return 0;
}

int start_fileserver_tcp(char *server_address){
	start_fileserver_co(server_address,AF_INET);
	return 0;
}

int main(int argc, char *argv[]){
	if (argc <= 1 || argc > 3){
		const char message[] ="Server benötigt zwei Parameter.\n";
		write(STDERR_FILENO,message,sizeof(message));
		return EXIT_FAILURE;
	}
	if (argv[1][0]=='-'){
		if (argv[1][1]=='U'){
			start_fileserver_co(argv[2],AF_UNIX);
		}else if (argv[1][1]=='u'){
			start_fileserver_udp(argv[2]);
		}else if (argv[1][1]=='t'){
			start_fileserver_tcp(argv[2]);
		}else{
			const char message[] ="Unbekanntes Protokoll.\n";
			write(STDERR_FILENO,message,sizeof(message));
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
