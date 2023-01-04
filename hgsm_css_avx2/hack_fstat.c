
#if !defined WIN32

#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>

#ifndef _STAT_VER

#define _STAT_VER 3 /** _STAT_VER_KERNEL64 / _STAT_VER_GLIBC2_3_4 / _STAT_VER_LINUX */

#endif

__asm__(".symver fstat, __fxstat@GLIBC_2.0");

int __wrap_fstat(int _0, struct stat* _1)
{
    return __fxstat(_STAT_VER, _0, _1);
}

#endif
