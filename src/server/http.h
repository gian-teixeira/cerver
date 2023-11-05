#ifndef _HTTP_H_
#define _HTTP_H_

typedef struct http_request http_request_t;
typedef struct http_response http_response_t;

struct http_request;
struct http_response;

struct http_request *http_request_parse(char *request_string);
char *http_get_attr(char *target, char *attr);
void http_destroy_request(struct http_request *request);
struct http_response *http_response_create(unsigned status, char *status_str);
char *http_response_build(struct http_response *response);
void http_response_set_body(struct http_response *response, char *body);
char *http_request_get_body(struct http_request *request);
char *http_request_get_url(struct http_request *request);

#endif