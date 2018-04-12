#include "version.h"
#include <string.h>

#define FW_VERSION_MAJOR    1
#define FW_VERSION_MINOR    0
#define FW_VERSION_BUILD    0

int version_show(char *buf, int len)
{
    int ret = 0;
    ret += snprintf(buf+ret, len-ret, "version  :%d.%d.%d\n",FW_VERSION_MAJOR, FW_VERSION_MINOR, FW_VERSION_BUILD);
    ret += snprintf(buf+ret, len-ret, "compile time :%s %s\n", __DATE__,__TIME__);
    
    return ret;
}