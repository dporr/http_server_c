#ifndef HTTP_H
#define HTTP_H
#include <stdio.h>
#include <string.h>
/*Defines special characters and status codes according to RFC 2616*/
//HTTP version
#define HTTP_1_1 "HTTP/1.1"
#define HTTP_VERSION "HTTP/1.1"
//Special chars
#define CR '\r' //Carriage return
#define LF '\n' //LineFeed
#define S  '\s' //separator
//Status codes
#define HTTP_100_continue  "100 Continue"
#define HTTP_200_OK        "200 OK"
#define HTTP_302_Found     "302 Found"
#define HTTP_403_Forbbiden "403 Forbidden"
#define HTTP_404_Not_Found "404 Not Found"
//    After receiving and interpreting a request message, a server responds
//    with an HTTP response message.
//        Response      = Status-Line               ; Section 6.1
//                        *(( general-header        ; Section 4.5
//                         | response-header        ; Section 6.2
//                         | entity-header ) CRLF)  ; Section 7.1
//                        CRLF
//                        [ message-body ]          ; Section 7.2

//Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
struct status_line {
    char   *http_version;
    short  status_code[3];
    char   reason_phrase[16];
};

struct response_headers {
    char *headers;
};

struct response {
    struct status_line status_line;
    struct response_headers headers;
};

ssize_t send_response(int client_socket, struct response my_r);
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
#endif