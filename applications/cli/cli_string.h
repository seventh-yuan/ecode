#ifndef __CLI_STRING_H__
#define __CLI_STRING_H__
#include "cli_types.h"

int str2int(const char *str, int *value, int8_t base);

int str2float(const char *str, float *value);

int str2double(const char *str, double *value);

int strcompare(const char *str1, int len1, const char *str2, int len2);

#endif