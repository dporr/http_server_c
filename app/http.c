#include "http.h"
ssize_t send_response(int client_socket, struct response my_r){
    //check socket fd is valid
    char response[strlen(my_r.status_line.http_version) +
                strlen(my_r.status_line.reason_phrase) +
                6];
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
   //close(client_socket);
   return ret;
}


ssize_t parse_request(char *r, struct http_request *client_data){
    if( !client_data  || !r || strnlen(r, MAX_REQUEST_BUFFER) >= MAX_REQUEST_BUFFER){
        return -1;
    }
    //tokenize *r and validate individual parts
    //Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
    
    char *token = strtok(r, "\r\n");
    //At this point we only care aboyt the request_line ignore headers and body
    // wile(token){
    //     token = strtok(NULL, "\r\n",&sp);
    // }
    /*Parse request line*/
   // Parse request line
    char request_line[strlen(token)];
    strncpy(request_line, token, strlen(token));
    token  = strtok(request_line, SP);
    strncpy(client_data->request_line->method, token,9);
    client_data->request_line->method[9] = '\0';

    token = strtok(NULL, (char *) SP);
    strncpy(client_data->request_line->path, token,511);
    client_data->request_line->path[511] = '\0';
    token = strtok(NULL, (char *) SP);
    strncpy(client_data->request_line->http_version, token,9);
    client_data->request_line->http_version[9] = '\0';
    printf("Size of r: %lu size of client_data: %lu \nContents of Request-Line: %s %s %s",
            strlen(r),
            sizeof(struct http_request),
            client_data->request_line->method,
            client_data->request_line->path,
            client_data->request_line->http_version
            );  

    return 0;
}