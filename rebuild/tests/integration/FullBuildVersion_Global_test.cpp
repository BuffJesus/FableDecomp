#include <stdio.h>
#include <string.h>
#include "fable_string.h"

extern CCharString FableFullBuildVersion;
static int constructed;
static int destroyed;
static const char* initializer;
static long initializerLength;
CCharString::CCharString(const char* text, long length) : storage_(0)
{
    initializer = text;
    initializerLength = length;
    ++constructed;
}
CCharString::~CCharString() { ++destroyed; }

int main()
{
    if (sizeof(FableFullBuildVersion) != 4 || constructed != 1 || destroyed)
        return 1;
    if (*reinterpret_cast<volatile unsigned char*>(&FableFullBuildVersion) != 0)
        return 3;
    if (strcmp(initializer, "Unknown") != 0 || initializerLength != -1)
        return 2;
    puts("FULL_BUILD_VERSION_GLOBAL_PASS");
    return 0;
}
