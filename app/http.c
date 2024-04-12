#include "http.h"

const char* http_header_values[] = {
    // "Host",
    "User-Agent"
    // "Referer",
    // "Accept"
};

ssize_t send_response(int client_socket, struct response my_r){
    //check socket fd is valid
    char response[strlen(my_r.status_line.http_version) +
                strlen(my_r.status_line.reason_phrase) +
                strlen(my_r.headers) + //By missing this I was BoFing here and thus corrupting the client_socket variable
                strlen(my_r.body) +
                6];
    sprintf(response, "%s %s\r\n%s\n%s",
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
//    close(client_socket);
   return ret;
}

void parse_request_line(char *incoming_request, struct request_line *rl){   
    char request_line[strlen(incoming_request)];
    strncpy(request_line, incoming_request, strlen(incoming_request));
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


void parse_headers(char *r, struct header_kv *rq_headers){
    if(!rq_headers){
        printf("Headers field is Null");
    }
    char *ptr;
    char *token = strtok_r(r, "\r\n",&ptr);
    ssize_t header_n = sizeof(http_header_values) / sizeof(http_header_values[0]);
    while(token){
        printf("\nGot header: %s\n", token);
        char *sptr;
        char *stoken = strtok_r(token, ":", &sptr);
        for(int i=0; i <= header_n ; i++){
            if(strcmp(stoken,"User-Agent") == 0){
                (rq_headers + i )->header_k = (enum http_header) i;
                strcpy((rq_headers + i)->header_v, &(sptr[1]));
                break;
            }
        }
		token = strtok_r(NULL, "\r\n", &ptr);
	}
}
/**
   A request message from a client to a server includes, within the
   first line of that message, the method to be applied to the resource,
   the identifier of the resource, and the protocol version in use.
 
        Request       = Request-Line              ; Section 5.1
                        *(( general-header        ; Section 4.5
                         | request-header         ; Section 5.3
                         | entity-header ) CRLF)  ; Section 7.1
                        CRLF
                        [ message-body ]          ; Section 4.3
*/
ssize_t parse_request(char *r, struct http_request *client_data){
    if( !client_data  || !r || strnlen(r, MAX_REQUEST_BUFFER) >= MAX_REQUEST_BUFFER){
        return -1;
    }
    //tokenize *r and validate individual parts
    char *ptr;
    char r_copy[strlen(r)];
    strncpy(r_copy, r, strlen(r));
    char* r_line = strtok_r(r_copy, "\r\n", &ptr);
    char* body = strstr(r, "\r\n\r\n");
    if(client_data->request_line) {
        parse_request_line(r_line, client_data->request_line);
    }
    parse_headers(ptr, client_data->headers);  
    if(client_data->body)
    {
        //TODO: Parse body and populate field
        strncpy(client_data->body, body + 4 , strlen(body));
    }
    printf("Size of r: %lu size of client_data: %lu \nContents of Request-Line: %s %s %s\n",
            strlen(r_copy),
            sizeof(struct http_request),
            client_data->request_line->method,
            client_data->request_line->path,
            client_data->request_line->http_version
            );  

    return 0;
}