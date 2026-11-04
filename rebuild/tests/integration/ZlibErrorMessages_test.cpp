#include "third_party/zlib121/retail_headers.h"
#include <stdio.h>
#include <string.h>

int main()
{
    static const char* expected[10] = {
        "need dictionary", "stream end", "", "file error", "stream error",
        "data error", "insufficient memory", "buffer error", "incompatible version", ""
    };
    for (int i = 0; i < 10; ++i)
        if (!z_errmsg[i] || strcmp(z_errmsg[i], expected[i])) return 1;
    puts("ZLIB_ERROR_MESSAGES_PASS cases=10");
    return 0;
}
