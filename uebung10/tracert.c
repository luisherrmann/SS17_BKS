//Compile with gcc -std=c11 -Wall -Wextra -pedantic -o client client.c
#define _DEFAULT_SOURCE //Aktiviert glibc-Erweiterungen
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
int send_udp_packet(char *server_address, int ttl){
	int udp_socket_fd = socket(AF_INET,SOCK_DGRAM,0); // AR_INET for IPv4, SOCK_DGRAM for Datagram, 17 for UDP
	if (udp_socket_fd ==-1){
		fprintf(stderr,"Der Verbindungsaufbau fehlgeschlagen. Host: %s\n",server_address);

		return -1;
	}
	struct sockaddr_in server_sock_addr;
	
	bzero(&server_sock_addr,sizeof(struct sockaddr_in));		
	server_sock_addr.sin_family = AF_INET;
	server_sock_addr.sin_port = htons(PORT); 
		
	int res = inet_aton(server_address,&server_sock_addr.sin_addr);
	if (res ==0){
		fprintf(stderr,"Die Adresse konnte nicht gelesen werden. %i Host: %s\n",res,server_address);
		return -1;
	}
	res= setsockopt(udp_socket_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
	if (res ==-1){
		fprintf(stderr,"TTL konnte nicht gesetzt werden. Host: %s\n",server_address);
		return -1;
	}
	my_data = (struct data_packet*) malloc(sizeof(struct data_packet));
	bzero(my_data->buffer,BUF_SIZE);
	const char buf[] = "Hello World!";
	strcpy(my_data->buffer,buf); // egal
	my_data->len=sizeof(size_t)+strlen(buf);

	socklen_t addlen = sizeof(server_sock_addr);
	int send_byte = 0;
	while (send_byte < (int)my_data->len){
		send_byte += sendto(udp_socket_fd,my_data->buffer, my_data->len-send_byte,MSG_MORE,(const struct sockaddr*)&server_sock_addr,addlen);	
	}
	if (send_byte ==-1){
		fprintf(stderr,"UDP-Paket konnte nicht gesendet werden. Host:%s\n",server_address);
		return -1;
	}
	//printf("stop sending\n");
	send_byte += sendto(udp_socket_fd,(void*)&"\0", 1,MSG_CONFIRM,(struct sockaddr*)&server_sock_addr,addlen);
	//printf("Confirmed. %i Bytes send\n",send_byte);
	bzero(my_data->buffer, BUF_SIZE);	
	close(udp_socket_fd);
	
	return 0;
}
int listen_raw_icmp(char *server_address,int ttl){
	int raw_socket_fd=socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
	if (raw_socket_fd ==-1){
		fprintf(stderr,"Der Verbindungsaufbau fehlgeschlagen. Host: %s\n",server_address);

		return -1;
	}
	struct sockaddr_in server_sock_addr;
	struct sockaddr_in client_sock_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	bzero(&client_sock_addr,sizeof(struct sockaddr_in));	
	memset(&server_sock_addr, '\0', sizeof(struct sockaddr_in));
	server_sock_addr.sin_family = AF_INET;
	server_sock_addr.sin_port = htons(PORT);
	server_sock_addr.sin_addr.s_addr = inet_addr(server_address);
	socklen_t addlen = sizeof(server_sock_addr);
	my_data = (struct data_packet*)malloc(sizeof(struct data_packet));
	bzero(my_data->buffer,BUF_SIZE);
	int res = recvfrom(raw_socket_fd, my_data->buffer, BUF_SIZE,0, (struct sockaddr *)&client_sock_addr, &addrlen);
	if (res ==-1){
		fprintf(stderr,"Auf dem RAW-Socket konnten keine Daten gelesen werden. Host:%s\n",server_address);
		return -1;
	}
//	fprintf(stdout, "Message received: %s\n", my_data->buffer);	
	fprintf(stdout, "%i : %s\n", ttl,inet_ntoa(client_sock_addr.sin_addr));	
	return 0;
}
int main(int argc, char *argv[]){
	

	if (argc != 2){
		const char message[] ="Client benötigt einen Parameter.\n";
		write(STDERR_FILENO,message,sizeof(message));
		return EXIT_FAILURE;
	}
	for (int i = 1; i<15; i++){
	send_udp_packet(argv[1],i);//UDP
	listen_raw_icmp("0.0.0.0",i);	
	}

	return EXIT_SUCCESS;
}

