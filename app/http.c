#include "http.h"
ssize_t send_response(int client_socket, struct response my_r){
    //check socket fd is valid
    char *response = malloc(
                        strlen(my_r.status_line.http_version) +
                        strlen(my_r.status_line.reason_phrase) +
                        6);
    sprintf(response, "%s %s\r\n\r\n",
                     my_r.status_line.http_version,
                     my_r.status_line.reason_phrase
                     );
    printf("Size of my_r: %lu size of response: %lu \nContents of response: %s",
            sizeof(my_r),
            sizeof(response),
            response
            );
   ssize_t  ret =  send(client_socket,response, strlen(response), 0);
   return ret;
}

ssize_t parse_request(char *r, struct http_request client_data){
    if( !client_data  | !r || strnlen(r, MAX_REQUEST_BUFFER) >= MAX_REQUEST_BUFFER) return -1;
    //tokenize *r and validate individual parts
}