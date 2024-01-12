#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "http.h"
int main() {
	// Disable output buffering
	setbuf(stdout, NULL);

	// You can use print statements as follows for debugging, they'll be visible when running tests.
	printf("Logs from your program will appear here!\n");

	int socket_fd =  socket(PF_INET, SOCK_STREAM, 0);
	//const struct sockaddr *address
	const struct sockaddr_in address = {
		.sin_family = AF_INET,
		.sin_port = htons(4221),
		.sin_addr = {
			.s_addr = htonl(INADDR_ANY)
		}
	};

	int reuse = 1;
  	if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) {
    	printf("SO_REUSEPORT failed: %s \n", strerror(errno));
    	printf("SO_REUSEPORT failed: %s\n", strerror(errno));
    	return 1;
 	}

	//bind(int socket, const struct sockaddr *address, socklen_t address_len);
	//https://man7.org/linux/man-pages/man2/bind.2.html
	int bind_r = bind(socket_fd, (struct sockaddr *) &address, sizeof(address));
	listen(socket_fd, 5);
	while(1){
		/*Accept clients*/
		//const struct sockaddr_in client_address;
		//int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
		//https://man7.org/linux/man-pages/man2/accept.2.html
		int client_socket = accept(socket_fd, 0,0); //&client_address, sizeof(struct sockaddr_in));
		printf("Accepted connection. Client FD: %d\n", client_socket);
		//Read client request
		//https://man7.org/linux/man-pages/man2/recv.2.html
		char r[MAX_REQUEST_BUFFER] = {0};
		struct http_request *client_data = malloc(sizeof(struct http_request));
		client_data->request_line = malloc(sizeof(struct request_line));

		ssize_t read_bytes = recv(client_socket, r, MAX_REQUEST_BUFFER,0);
		printf("Got %d bytes for request %s", read_bytes, r);
		parse_request(r, client_data);
		
		struct response my_r = { 
			.status_line =  {
				.http_version = HTTP_1_1,
				.reason_phrase = HTTP_200_OK,
				.status_code = 200,
			}
		};
		if(client_data){
			if( strcmp(client_data->request_line->path, "/" ) != 0){
				strcpy(my_r.status_line.reason_phrase, HTTP_404_Not_Found);
				my_r.status_line.status_code = 404;
			} 
		}
		send_response(client_socket, my_r);
		free(client_data->request_line);
		free(client_data);
	}
	return 0;
}
