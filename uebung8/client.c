//Compile with gcc -std=c11 -Wall -Wextra -pedantic -o client client.c -lrt -pthread
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
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

struct data_packet{
	size_t len;
	char buffer[BUF_SIZE];
};
struct data_packet *my_data;

int connect_fileserver_unix(char *server_path, char *filepath){

	int unix_socket_fd = socket(AF_LOCAL,SOCK_STREAM,0);

	struct sockaddr_un my_sock_addr;
	bzero(&my_sock_addr,sizeof(struct sockaddr_un));	
	my_sock_addr.sun_family = AF_LOCAL;

  	strcpy(my_sock_addr.sun_path, server_path);

	my_data = (struct data_packet*)malloc(sizeof(struct data_packet));

	bzero(my_data->buffer,BUF_SIZE);

	strcpy(my_data->buffer,filepath);
		printf("Connection\n");
	my_data->len=sizeof(size_t)+strlen(filepath);
	connect(unix_socket_fd,(const struct sockaddr*)&my_sock_addr, sizeof(struct sockaddr)); 

	size_t send_byte = 0;
	while (send_byte < my_data->len){
		send_byte += send(unix_socket_fd,my_data->buffer,my_data->len,0);
	}
	send_byte += send(unix_socket_fd,"\0",1,MSG_CONFIRM);
	
	bzero(my_data->buffer, BUF_SIZE);
	int cur_recv =0;
	size_t recv_bytes = 0;
	my_data->len=(size_t)BUF_SIZE;
	
	do {		
		cur_recv = recv(unix_socket_fd,my_data,my_data->len,0);

		recv_bytes+=cur_recv;
		
	}while (recv_bytes<my_data->len || cur_recv<0);
	printf("Message:%s\n",my_data->buffer);
	close(unix_socket_fd);
	return 0;
}

int connect_fileserver_tcp(char *server_address, char *filepath){
	int tcp_socket_fd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in my_sock_addr;
	bzero(&my_sock_addr,sizeof(struct sockaddr_in));	
	my_sock_addr.sin_family = AF_INET;
	my_sock_addr.sin_port = htons(PORT); 
	inet_aton(server_address,&my_sock_addr.sin_addr);
		
	my_data = (struct data_packet*)malloc(sizeof(struct data_packet));
	bzero(my_data->buffer,BUF_SIZE);
	strcpy(my_data->buffer,filepath);
	my_data->len=sizeof(size_t)+strlen(filepath);
	connect(tcp_socket_fd,(const struct sockaddr*)&my_sock_addr, sizeof(struct sockaddr)); 
	
	size_t send_byte = 0;
	while (send_byte < my_data->len){
		send_byte += send(tcp_socket_fd,my_data->buffer,my_data->len,0);
	}
	send_byte += send(tcp_socket_fd,"\0",1,MSG_CONFIRM);
	
	bzero(my_data->buffer, BUF_SIZE);
	int cur_recv =0;
	size_t recv_bytes = 0;
	my_data->len=(size_t)BUF_SIZE;
	
	do {		
		cur_recv = recv(tcp_socket_fd,my_data,my_data->len,0);

		recv_bytes+=cur_recv;
		
	}while (recv_bytes<my_data->len || cur_recv<0);
	printf("Message:%s\n",my_data->buffer);
	close(tcp_socket_fd);
	return 0;
}
int connect_fileserver_udp(char *server_address, char *filepath){
	int udp_socket_fd = socket(AF_INET,SOCK_DGRAM,17); // AR_INET for IPv4, SOCK_DGRAM for Datagram, 17 for UDP
	struct sockaddr_in server_sock_addr;
	
	bzero(&server_sock_addr,sizeof(struct sockaddr_in));	
	server_sock_addr.sin_family = AF_INET;
	server_sock_addr.sin_port = htons(PORT); 
	inet_aton(server_address,&server_sock_addr.sin_addr);

	my_data = (struct data_packet*) malloc(sizeof(struct data_packet));
	bzero(my_data->buffer,BUF_SIZE);
	strcpy(my_data->buffer,filepath);
	my_data->len=sizeof(size_t)+strlen(filepath);

	socklen_t addlen = sizeof(server_sock_addr);
	size_t send_byte = 0;
	while (send_byte < my_data->len){
		send_byte += sendto(udp_socket_fd,my_data->buffer, my_data->len-send_byte,MSG_MORE,(const struct sockaddr*)&server_sock_addr,addlen);	
	}
	printf("stop sending\n");
	send_byte += sendto(udp_socket_fd,(void*)&"\0", 1,MSG_CONFIRM,(struct sockaddr*)&server_sock_addr,addlen);
	printf("Confirmed\n");
	bzero(my_data->buffer, BUF_SIZE);
	int cur_recv =0;
	size_t recv_bytes = 0;
	
	my_data->len=(size_t)BUF_SIZE;
	do {	
		cur_recv = recvfrom(udp_socket_fd,my_data, my_data->len,MSG_WAITALL,(struct sockaddr*)&server_sock_addr, &addlen);
		recv_bytes+=cur_recv;	
		printf("recv:%i",cur_recv);
	}while (recv_bytes<my_data->len || cur_recv<0);
	printf("Message:%s\n",my_data->buffer);
	close(udp_socket_fd);

	return 0;
}
int main(int argc, char *argv[]){
	if (argc != 4){
		const char message[] ="Client benötigt drei Parameter.\n";
		write(STDERR_FILENO,message,sizeof(message));
		return EXIT_FAILURE;
	}
	if (argv[1][0]=='-'){
		if (argv[1][1]=='U'){
			connect_fileserver_unix(argv[2],argv[3]);//Unix-Socket
		}else if (argv[1][1]=='u'){
			connect_fileserver_udp(argv[2],argv[3]);//UDP
		}else if (argv[1][1]=='t'){
			connect_fileserver_tcp(argv[2],argv[3]);//TCP
		}else{
			const char message[] ="Unbekanntes Protokoll.\n";
			write(STDERR_FILENO,message,sizeof(message));
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

