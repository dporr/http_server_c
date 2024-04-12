#include "server.h"
int main(int argc, char **argv) {

	/*
	// If <filename> exists in <directory>, you'll need to respond with a 200 OK response.
	// The response should have a content type of application/octet-stream,
	//  and it should contain the contents of the file as the body.
	*/

	// if(argc == 3  && strcmp("--directory", argv[1]))
	// {
	// 	printf("Getting file from %s\n", argv[2]);
	// }
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
	int client_fd = -1;
	while(( client_fd = accept(socket_fd,0,0)) != -1){
		printf("Accepted connection. Client FD: %d\n", (int)client_fd);
		pthread_t thread_id;
		memset(&thread_id, 0, sizeof(pthread_t));
		pthread_create(&thread_id, NULL, &handle_client, (void *) (intptr_t)client_fd); 
	}

	return 0;
}


void *handle_client(void * pClient_fd){
	{
		if( !pClient_fd || (int *) pClient_fd <= 0){
			printf("Invalid Client socket FD: %d", (int)  (intptr_t) pClient_fd);
		}
		int client_fd = (int) (intptr_t) pClient_fd;
		/*Accept clients*/
		//const struct sockaddr_in client_address;
		//int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
		//https://man7.org/linux/man-pages/man2/accept.2.html
		//int client_socket = accept(socket_fd, 0,0); //&client_address, sizeof(struct sockaddr_in));
		
		//Read client request
		//https://man7.org/linux/man-pages/man2/recv.2.html
		char r[MAX_REQUEST_BUFFER] = {0};
		struct http_request *client_data = malloc(sizeof(struct http_request));
		client_data->request_line = malloc(sizeof(struct request_line));
		// Initialize the array with null values
		for (int i = 0; i < 10; i++) {
			client_data->headers[i].header_k = HTTP_GENERIC_H;
			strcpy(client_data->headers[i].header_v, "");
		}
		//  the body with null values
		client_data->body = malloc(4096);
		memset(client_data->body, 0, 4096);

		ssize_t read_bytes = recv((int) client_fd, r, MAX_REQUEST_BUFFER,0);
		printf("Got %ld bytes for request\n\n%s", read_bytes, r);
		parse_request(r, client_data);
		
		struct response my_r = { 
			.status_line =  {
				.http_version = HTTP_1_1,
				.reason_phrase = HTTP_200_OK,
				.status_code = 200,
			},
			.body = {0}
		};
		if(client_data){
			//TODO: implement a request router for each supported method: GET, POST.
			if(strcmp(client_data->request_line->method, "GET" ) == 0)
			{
				printf("Do GET stuff...");
							if( strcmp(client_data->request_line->path, "/" ) != 0){
				char path[512] = {0};
				strcpy(path, client_data->request_line->path);
				int i = 0;
				char *ptr;
				char *token = strtok_r(path, "/", &ptr);
				if(strcmp(token, "echo" ) == 0){
					strcpy(my_r.body, ptr);
					char headers[512] = {0};
					sprintf(headers, "Content-Type: text/plain\nContent-Length: %ld\n", strlen(ptr));
					strcpy(my_r.headers, headers);
				} else if(strcmp(token, "user-agent" ) == 0){
    				//char *token = strtok_r(r, "\r\n", &ptr);
					//parse_headers(ptr, client_data->headers);
					char* UA = client_data->headers[(int)HTTP_USER_AGENT_H].header_v;
					// char* HH = client_data->headers[(int)HTTP_HOST_H].header_v;
					// char* AH = client_data->headers[(int)HTTP_ACCEPT_H].header_v;
					// printf("\n\nU-A:%s HostH: %s Accept: %s\n\n", UA, HH, AH);
					char res_headers[512] = {0};
					sprintf(res_headers, "Content-Type: text/plain\nContent-Length:%ld\r\n", strlen(UA));
					strcpy(my_r.headers, res_headers);
					strcpy(my_r.body, UA);
				}
				else if(strcmp(token, "files" ) == 0){
					//Extract file slug
					char *base_path = "/tmp/data/codecrafters.io/http-server-tester/";
					char *n_ptr;
					char *file_name = strtok_r(ptr, "/", &n_ptr);
					//printf("File: %s", file_name);
					//Set base path
					size_t abs_path_size = strlen(base_path) + strlen(file_name) + 1;
					if(abs_path_size >= 4096){
						printf("Absolute path too long...");
					}
					char *abs_path = malloc( abs_path_size );
					if(abs_path == NULL){
						perror("Failed to allocate memory for abs_path");
        				exit(EXIT_FAILURE);
					}
					snprintf(abs_path,abs_path_size, "%s%s", base_path, file_name );
					//check for file existence
					if (access(abs_path, F_OK) != 0) {
						printf("File %s doesnt exist. Returning...\n", abs_path);
						char res_headers[512] = {0};
						sprintf(res_headers, "Content-Type: text/plain\nContent-Length: %d\n", 0);
						strcpy(my_r.headers, res_headers);
						strcpy(my_r.status_line.reason_phrase, HTTP_404_Not_Found);
						my_r.status_line.status_code = 404;
					} //send response
					else{
						//read file
						FILE *fp = fopen(abs_path, "r");
						if(fp == NULL){
							//error out
						}
						//get file size
						fseek(fp, 0L, SEEK_END);
						long file_size = ftell(fp);
						fseek(fp, 0L, SEEK_SET);
						char *file_contents = malloc( (size_t) file_size + 1);
						if(file_size >= MAX_REQUEST_BUFFER) {
							printf("File exceeds MAX_RESPONSE_BUFFER: %d", MAX_REQUEST_BUFFER);
						}
						if( fread(file_contents, file_size, 1, fp ) != file_size){
							printf("File read truncated at...");
						}
						//Response
						char res_headers[512] = {0};
						sprintf(res_headers, "Content-Type: application/octet-stream\nContent-Length: %ld\n", file_size);
						strcpy(my_r.headers, res_headers);
						strcpy(my_r.status_line.reason_phrase, HTTP_200_OK);
						strncpy(my_r.body, file_contents, file_size);
						my_r.status_line.status_code = 200;

					}
					


				} else{
					char res_headers[512] = {0};
					sprintf(res_headers, "Content-Type: text/plain\nContent-Length: %d\n", 0);
					strcpy(my_r.headers, res_headers);
					strcpy(my_r.status_line.reason_phrase, HTTP_404_Not_Found);
					my_r.status_line.status_code = 404;
				}
			}else{
				char res_headers[512] = {0};
				sprintf(res_headers, "Content-Type: text/plain\nContent-Length: %d\n", 0);
				strcpy(my_r.headers, res_headers);
			}
			} 
			else if(strcmp(client_data->request_line->method, "POST" ) == 0)
			{
				printf("Do POST stuff...");
				char *ptr;
				char path[512] = {0};
				strcpy(path, client_data->request_line->path);
				char *token = strtok_r(path, "/", &ptr);
				//printf("PATH: %s\n", ptr);
				//TODO: Create file with name `ptr` under `/tmp/data/codecrafters.io/http-server-tester/`
					//Extract file slug
					char *base_path = "/tmp/data/codecrafters.io/http-server-tester/";
					char *n_ptr;
					//printf("File: %s", file_name);
					//Set base path
					size_t abs_path_size = strlen(base_path) + strlen(ptr) + 1;
					if(abs_path_size >= 4096){
						printf("Absolute path too long...");
					}
					char *abs_path = malloc( abs_path_size );
					if(abs_path == NULL){
						perror("Failed to allocate memory for abs_path");
        				exit(EXIT_FAILURE);
					}
					snprintf(abs_path,abs_path_size, "%s%s", base_path, ptr );
					//check for file existence
					if (access(abs_path, F_OK) != 0) {
						FILE *fp = fopen(abs_path, "w");
						if(fp == NULL){
							//error out
						}
						printf("Created file: %s\n", abs_path);
					}
				//Write request parse_request to ptr
				//return 201
				char res_headers[512] = {0};
				sprintf(res_headers, "Content-Type: text/plain\nContent-Length: %d\n", 0);
				strcpy(my_r.headers, res_headers);
				strcpy(my_r.status_line.reason_phrase, HTTP_201_OK);
				my_r.status_line.status_code = 201;
			} 
		}
		printf("Passing FD:%d\n",  (int) client_fd);
		send_response((int) client_fd, my_r);
		free(client_data->request_line);
		free(client_data);
	}
	return NULL;
}