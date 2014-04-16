#include <stdio.h>
#include "http_parser.h"

int main(void) {
    const char* h = "HTTP/1.1";
    http_parser_execute(NULL, h, 9);
    h = "HTTP1.1";
    http_parser_execute(NULL,h,8);
    return 0;
}
