#ifndef HTTP_H
#define HTTP_H

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
// struct status_line = {
//     char   *http_version, 
//     short  status_code[3],
//     char   reasson_phrase[16]
// }

// struct response_headers {
//     char *headers,
// }
// struct response {
//     struct status_line status_line,
//     struct headers response_headers,
// }

#endif