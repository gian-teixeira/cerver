#include "http.h"
#include "util.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>

struct http_request {
    char *method;
    char *url;
    char *body;
};

struct http_response {
    char *status;
    char *header;
    char *body;
};

struct http_request *http_request_parse(char *request_string)
{   
    regex_t regex;
    regcomp(&regex, "([A-Z]+) /([^ ]*)", REG_EXTENDED);
    int match_count = 3;
    regmatch_t matches[match_count];

    if(regexec(&regex, request_string, match_count, matches, 0)) {
        errno = EBADR;
        perror("[-] HTTP");
        return NULL;
    }
    for(int i = 0; i < match_count; i++) {
        request_string[matches[i].rm_eo] = '\0';
    }

    struct http_request *request = malloc(sizeof(struct http_request));
    request->method = strcpydeep(request_string + matches[1].rm_so);
    request->url = strcpydeep(request_string + matches[2].rm_so);
    request->body = strcpydeep(request_string + matches[2].rm_eo + 1);

    return request;
}

char *http_get_attr(char *target, char *attr) 
{
    char match_str[BUFFER_SIZE];
    memset(match_str,0,sizeof(match_str));
    sprintf(match_str, "%s: ([^ ]*)\n", attr);

    regex_t regex;
    regmatch_t match[2];
    regcomp(&regex, match_str, REG_EXTENDED);
    
    if(regexec(&regex, target, 2, match, 0) != 0) {
        perror("[-] Match failed\n");
        return NULL;
    }

    size_t attr_len = match[1].rm_eo - match[1].rm_so + 1;
    char *attr_value = calloc(attr_len,1);
    strncpy(attr_value, target + match[1].rm_so, attr_len);

    return attr_value;
}

struct http_response *http_response_create(unsigned status, char *status_str) 
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, BUFFER_SIZE, "HTTP/1.1 %d %s", status, status_str);
    
    struct http_response *response = malloc(sizeof(struct http_response));
    memset(response,0,sizeof(response));
    response->status = strcpydeep(buffer);
    response->header = malloc(BUFFER_SIZE);
    strcpy(response->header, "");

    return response;
}

char *http_response_build(struct http_response *response)
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, BUFFER_SIZE, "%s\n\n%s\n\n%s\n",
            response->status, response->header, response->body);
    return strcpydeep(buffer);
}

void http_response_set_body(struct http_response *response, char *body)
{
    response->body = strcpydeep(body);
    printf("%s\n", response->body);
}

void http_destroy_request(struct http_request *request)
{
    free(request->method);
    free(request->url);
    free(request->body);
    free(request);
}

char *http_request_get_body(struct http_request *request)
{
    return request->body;
}

char *http_request_get_url(struct http_request *request)
{
    return request->url;
}