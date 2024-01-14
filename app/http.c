#include "http.h"
ssize_t send_response(int client_socket, struct response my_r){
    //check socket fd is valid
    char response[strlen(my_r.status_line.http_version) +
                strlen(my_r.status_line.reason_phrase) +
                strlen(my_r.headers) + //By missing this I was BoFing here and thus corrupting the client_socket variable
                strlen(my_r.body) +
                6];
    sprintf(response, "%s %s\r\n%s\n%s\r\n",
                    my_r.status_line.http_version,
                    my_r.status_line.reason_phrase,
                    my_r.headers,
                    my_r.body
                    );
    printf("Size of my_r: %lu size of response: %lu \nContents of response:\n%s",
            sizeof(my_r),
            sizeof(response),
            response
            );
   ssize_t  ret =  send(client_socket,response, strlen(response), 0);
   if(ret != strlen(response) || ret < 0){
        printf("File descriptor: %d\n", client_socket);
        perror("send");
   }
   close(client_socket);
   return ret;
}

void parse_request_line(char *r, struct request_line *rl){   
    char request_line[strlen(r)];
    strncpy(request_line, r, strlen(r));
    //Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
    char *token  = strtok(request_line, SP);
    strncpy(rl->method, token,9);
    rl->method[9] = '\0';
    token = strtok(NULL, SP);
    strncpy(rl->path, token,511);
    rl->path[511] = '\0';
    token = strtok(NULL, SP);
    strncpy(rl->http_version, token,9);
    rl->http_version[9] = '\0';
}

ssize_t parse_request(char *r, struct http_request *client_data){
    if( !client_data  || !r || strnlen(r, MAX_REQUEST_BUFFER) >= MAX_REQUEST_BUFFER){
        return -1;
    }
    //tokenize *r and validate individual parts
    char *token = strtok(r, "\r\n");
    if(client_data->request_line) {
        parse_request_line(token, client_data->request_line);
    }
    printf("Size of r: %lu size of client_data: %lu \nContents of Request-Line: %s %s %s",
            strlen(r),
            sizeof(struct http_request),
            client_data->request_line->method,
            client_data->request_line->path,
            client_data->request_line->http_version
            );  

    return 0;
}