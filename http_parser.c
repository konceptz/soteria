#include <stdio.h>
#include <stdlib.h>
#include "http_parser.h"

#ifndef HTTP_MAX_HEADER
#define HTTP_MAX_HEADER (1<<13)
#endif

size_t http_parser_execute(struct Http* http, const char* data, size_t len) {
    enum parser_state_t state = s_BEGIN;
    const char* data_ptr = data;
    size_t bytes_read;
    char tok;

    for(data_ptr = data, bytes_read = 0; data_ptr != (data+len); ++data_ptr, ++bytes_read) {

        tok = *data_ptr;

        // Avoid DOS
        if (bytes_read >= HTTP_MAX_HEADER) {
            http = NULL;
            return bytes_read;
        }

        switch (state) {

            case s_BEGIN:
                // keep consuming the input
                // until the first meaningful content is reached
                // should also deal with whitespace in case
                switch (tok) {
                    case '\r':
                    case '\n':
                        break;
                    case 'H':
                        state = s_H;
                        break;
                    default:
                        state = s_Req;
                        break;
                }
                break;

            case s_H:

                if (tok == 'T') {           // Request Header
                    state = s_HT;
                }else if (tok == 'E') {    // method HEAD
                    state = s_Req;
                }else {                    // Error unknown char
                    fputs("Failed to parse request or response\n",stderr);
                }

                break;

            case s_HT:
                if (tok == 'T') state = s_HTT;
                else { fputs("Failed to parse HTT\n", stderr); exit(1); }
                break;
            case s_HTT:
                if (tok == 'P') state = s_HTTP;
                else { fputs("Failed to parse HTTP\n", stderr); exit(1); }
                break;
            case s_HTTP:
                if (tok == '/') state = s_MF;
                else { fputs("Failed to parse HTTP/\n", stderr); exit(1); }
                break;
            case s_MF:
                if (tok <= '9' && tok >= '0') state = s_MR;
                else { fputs("Failed to parse HTTP/X\n", stderr); exit(1); }
                break;
            case s_MR:
                puts("Exiting from successful parse of HTTP Version");
                return bytes_read;
                break;
            case s_MiF:
                break;
            case s_MiR:
                break;
            case s_Req:
                puts("Reached Request\n");
                return bytes_read;
                break;
        }
    }

        http = NULL;
        return bytes_read;
}

//int http_parse_url(struct* Url, char char* data, size_t len) {
//    return 1;
//}
