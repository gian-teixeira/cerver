#include "util.h"
#include <string.h>
#include <stdlib.h>

char *strcpydeep(char *source)
{
    char *dest = malloc(strlen(source)+1);
    strncpy(dest, source, strlen(source));
    return dest;
}