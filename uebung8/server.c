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

#define PORT 1502
#define BUF_SIZE 512

int start_fileserver_unix()
{	
	/*//Starte Server	
	struct sockaddr_un server_addr;
	struct sockaddr_un client_addr;
	socklen_t socksize = sizeof(struct sockaddr_un);
	memset(&server_addr, 0, sizeof(server_addr);
	server_adrr.sun_family = AF_UNIX;
	server_addr.path = "/un_ssocket";

	int fd_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	if(bind(fd_socket, (sockaddr_un *) &server_addr, sizeof(struct sockaddr_un) == -1){
		fprintf(stderr, "Could not bind socket.");
		return EXIT_FAILURE;
	}
	listen(fd_socket,3); //Länge der Queue
	int fd_conn_socket;
	if((fd_conn_socket = accept(fd_socket, (sockaddr_un *) &client_addr, &socksize)) == -1){
		fprintf(stderr, "Error establishing connection with client.");
		return EXIT_FAILURE;
	}
	int i = 0;
	char *msg_buffer = malloc(MSG_LEN);
	char curr;
	while((curr = fgetc(fd_conn_socket)) != EOF && i < MSG_LEN-1){
		msg_buffer[i] = curr;
		i++;
	}
	msg_buffer[i] = '\0';
	FILE *source = fopen(msg_buffer,"r");
	//Filename received, try reading from file
	char *buffer = malloc(BUFFER_SIZE);
	if(source == NULL)
	{
		fprintf(stderr, "An error has occured while reading!\n");
		if(errno == ENOENT){
			fprintf(stderr, "The file specified does not exist!\n");
		}
		return 0;
	}
	char *content = malloc(BUFFER_SIZE);
	while(fgets(buffer,BUFFER_SIZE,source) != NULL){
		char* temp = malloc(strlen(content) + strlen(buffer) + 1);
		strcpy(temp,content);
		strcat(temp,buffer);
		content = temp;
	}
	free(buffer);
	free(content);
	fclose(source);*/
}

char BUFFER[BUF_SIZE];
int start_fileserver_udp(){
	int udp_socket_fd = socket(AF_INET,SOCK_DGRAM,17);
	struct sockaddr_in server_sock_addr;
	struct sockaddr_in client_sock_addr;
	socklen_t socketsize;
	bzero(&server_sock_addr, sizeof(struct sockaddr_in));
	my_sock_addr.sin_family = AF_INET;
	my_sock_addr.sin_port = htons(PORT);
	my_sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(udp_socket_fd, (sockaddr_in *) &client_sock_addr, &socketsize) == -1){
		fprintf(stderr, "Error binding to port");
		return 0;
	}
	listen(upd_socket_fd,3);
	int client_socket;
	//Stelle Verbindung mit Client her
	if((client_socket = accept(udp_socket_fd, (sockaddr_in *) &client_sock_addr, &socketsize) == -1){
		fprintf(stderr, "Error establishing connection");
		return 0;
	}
	char* client_message = malloc(BUF_SIZE);
	//Lese solange Socket nicht leer ist.
	recv(client_socket, BUFFER, BUF_SIZE);
	fprintf(stdout, "Message received: %s", BUFFER);
	close(udp_socket_fd);
	return 0;
}

int main ()
{
	start_fileserver_udp();
} 
