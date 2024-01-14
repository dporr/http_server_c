#ifndef HTTP_H
#define HTTP_H
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
/*Defines special characters and status codes according to RFC 2616*/
#define MAX_REQUEST_BUFFER 8192 //8KiB
#define METHOD_SIZE 10
#define PATH_SIZE 512
//HTTP version
#define HTTP_1_1 "HTTP/1.1"
#define HTTP_VERSION "HTTP/1.1"
//Special chars
#define CR '\r' //Carriage return
#define LF '\n' //LineFeed
#define SP  " " //separator
//Status codes
#define HTTP_100_continue  "100 Continue"
#define HTTP_200_OK        "200 OK"
#define HTTP_302_Found     "302 Found"
#define HTTP_403_Forbbiden "403 Forbidden"
#define HTTP_404_Not_Found "404 Not Found"
enum http_header {
    HTTP_GENERIC_H = -1,
    HTTP_HOST_H,
    HTTP_USER_AGENT_H,
    HTTP_REFERER_H,
    HTTP_ACCEPT_H
};

extern const char* http_header_values[]; // Declaration as an external symbol
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
    short  status_code;
    char   reason_phrase[16];
};

struct response {
    struct status_line status_line;
    char headers[512];
    char body[MAX_REQUEST_BUFFER];
};



//Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
struct request_line{
        char method[10];
        char path[512];
        char http_version[10];
};

struct header_kv {
    enum http_header header_k;
    char header_v[256]; //Limit header value to 256 chars
};
struct request_headers {
    char *headers;
};

struct http_request {
    struct request_line *request_line;
    struct header_kv headers[10];
};

ssize_t send_response(int client_socket, struct response my_r);
ssize_t parse_request(char *r, struct http_request *client_data);
void parse_request_line(char *r, struct request_line *rl);
void parse_path(struct request_line *rl, char **path_sections);
void parse_headers(char *r, struct header_kv *headers);
#endif