#ifndef HTTP_PARSER_H_
#define HTTP_PARSER_H_
#include <stddef.h>
#include <stdint.h>


/*
 * See RFC 2616 for further information
 * regarding status codes
 * */
enum status_code_t {


    CONTINUE                        = 100,
    SWITCHING_PROTOCOLS             = 101,


    OK                              = 200,
    CREATED                         = 201,
    ACCEPTED                        = 202,
    NON_AUTHORITATIVE_INFORMATION   = 203,
    NO_CONTENT                      = 204,
    RESET_CONTENT                   = 205,
    PARTIAL_CONTENT                 = 206,


    MULTIPLE_CHOICES                = 300,
    MOVED_PERMANENTLY               = 301,
    FOUND                           = 302,
    SEE_OTHER                       = 303,
    NOT_MODIFIED                    = 304,
    USE_PROXY                       = 305,
    TEMPORARY_REDIRECT              = 307,


    BAD_REQUEST                     = 400,
    UNAUTHORIZED                    = 401,
    PAYMENT_REQUIRED                = 402,
    FORBIDDEN                       = 403,
    NOT_FOUND                       = 404,
    METHOD_NOT_ALLOWED              = 405,
    NOT_ACCEPTABLE                  = 406, 
    PROXY_AUTHENTICATION_REQUIRED   = 407,
    REQUEST_TIME_OUT                = 408,
    CONFLICT                        = 409,
    GONE                            = 410,
    LENGTH_REQUIRED                 = 411,
    PRECONDITION_FAILED             = 412,
    REQUEST_ENTITY_TOO_LARGE        = 413,
    REQUEST_URI_TOO_LARGE           = 414,
    UNSUPPORTED_MEDIA_TYPE          = 415,
    REQUESTED_RANGE_NOT_SATISFIABLE = 416,
    EXPECTATION_FAILED              = 417,


    INTERNAL_SERVER_ERROR           = 500,
    NOT_IMPLEMENTED                 = 501,
    BAD_GATEWAY                     = 502,
    SERVICE_UNAVAILABLE             = 503,
    GATEWAY_TIMEOUT                 = 504,
    HTTP_VERSION_NOT_SUPPORTED      = 505,


};

enum method_t {
    OPTIONS,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    TRACE,
    CONNECT,
};

typedef struct {
    char            *b;
    char            *e;
} txt;

struct Http {
    txt*            hd;
    unsigned char*  hdf;
    enum status_code_t   status;
};


/**
 *
 * RFC 822 defines the format of a message as header fields, and
 * optionally a body, which is defined as content under the 
 * empty line (CR without anything preceding)
 *
 * Message := MESSAGE_WITH_BODY | MESSAGE
 * MESSAGE_WITH_BODY := HEADER&MESSAGE | 
 */
/*
 * HTTP Header format is:
 * Message consists of a start-line, zero
 * or more header fields (also known as "headers"), an empty line (i.e.,
 * a line with nothing preceding the CRLF) indicating the end of the
 * header fields, and possibly a message-body.
 *
 *      generic-message = start-line
 *                        *(message-header CRLF)
 *                        CRLF
 *                        [ message-body ]
 *      start-line      = Request-Line | Status-Line
 * */


enum parser_state_t {
    s_BEGIN,
    s_H,                // first H of HTTP
    s_HT,               // HT of HTTP
    s_HTT,              // HTT of HTTP
    s_HTTP,             // HTTP of HTTP
    s_MF,               // HTTP Major.Minor
    s_MR,
    s_MiF,
    s_MiR,
    s_SCF,              // status code first
    s_SCR,
    s_Req,
    s_END,
};


size_t http_parser_execute(struct Http* http, const char* data, size_t len);


//int http_parse_url(struct *Url, const char* data, size_t len);



#endif  // HTTP_PARSER_H_
