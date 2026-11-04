// Upstream zlib 1.2.1 zutil.c error strings; zlib license in zlib.h.
#include "third_party/zlib121/retail_headers.h"

extern "C" const char* const z_errmsg[10] = {
    "need dictionary",
    "stream end",
    "",
    "file error",
    "stream error",
    "data error",
    "insufficient memory",
    "buffer error",
    "incompatible version",
    ""
};
