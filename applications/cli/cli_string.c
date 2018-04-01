#include "cli_string.h"
#include <stdlib.h>
#include <string.h>


int str2int(const char *str, int *value, int8_t base)
{
    char *endptr;
    *value = strtol(str, &endptr, base);
    
    return endptr-str;
}

int str2float(const char *str, float *value)
{
    char *endptr;
    *value = strtof(str, &endptr);
    return endptr-str;
}

int str2double(const char *str, double *value)
{
    char *endptr;
    *value = strtod(str, &endptr);
    return endptr-str;
}

int strcompare(const char *str1, int len1, const char *str2, int len2)
{
    if(len1!=len2)
        return 0;
    
    if(0==strncmp(str1, str2, len2))
        return 1;
    
    return 0;
}
