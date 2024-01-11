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

	//bind(int socket, const struct sockaddr *address, socklen_t address_len);
	//https://man7.org/linux/man-pages/man2/bind.2.html
	int bind_r = bind(socket_fd, (struct sockaddr *) &address, sizeof(address));
	listen(socket_fd, 5);
	/*Accept clients*/
	//const struct sockaddr_in client_address;
	//int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
	//https://man7.org/linux/man-pages/man2/accept.2.html
	int client_socket = accept(socket_fd, 0,0); //&client_address, sizeof(struct sockaddr_in));

	//Read client request
	//https://man7.org/linux/man-pages/man2/recv.2.html

	//send reply
	//https://man7.org/linux/man-pages/man2/send.2.html
	//ssize_t send(int sockfd, const void buf[.len], size_t len, int flags);
	struct response my_r = { 
		.status_line =  {
			.http_version = HTTP_1_1,
			.reasson_phrase = "HTTP_200_OK",
			.status_code = 200,
		}
	};
	//char response[32];
	//sprintf(response, "%s %s\n\r\n\r", HTTP_1_1, HTTP_200_OK);
	//printf("%s",response);git
	send_response(client_socket, my_r);
	//send(client_socket, &response, strlen(response),0);
	return 0;
}
