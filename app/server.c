#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

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
	char *response = "";
	send(client_socket, )
	// Uncomment this block to pass the first stage
	//
	// int server_fd, client_addr_len;
	// struct sockaddr_in client_addr;
	//
	// server_fd = socket(AF_INET, SOCK_STREAM, 0);
	// if (server_fd == -1) {
	// 	printf("Socket creation failed: %s...\n", strerror(errno));
	// 	return 1;
	// }
	//
	// // Since the tester restarts your program quite often, setting REUSE_PORT
	// // ensures that we don't run into 'Address already in use' errors
	// int reuse = 1;
	// if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) {
	// 	printf("SO_REUSEPORT failed: %s \n", strerror(errno));
	// 	return 1;
	// }
	//
	// struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
	// 								 .sin_port = htons(4221),
	// 								 .sin_addr = { htonl(INADDR_ANY) },
	// 								};
	//
	// if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
	// 	printf("Bind failed: %s \n", strerror(errno));
	// 	return 1;
	// }
	//
	// int connection_backlog = 5;
	// if (listen(server_fd, connection_backlog) != 0) {
	// 	printf("Listen failed: %s \n", strerror(errno));
	// 	return 1;
	// }
	//
	// printf("Waiting for a client to connect...\n");
	// client_addr_len = sizeof(client_addr);
	//
	// accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
	// printf("Client connected\n");
	//
	// close(server_fd);

	return 0;
}
