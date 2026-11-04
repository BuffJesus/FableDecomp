// Adapted from zlib 1.2.1 (https://zlib.net/fossils/zlib-1.2.1.tar.gz).
// Copyright (C) 1995-2003 Mark Adler. See zlib.h for the license.
// C++ adaptation: ANSI parameter lists, `this` renamed to `here`, explicit
// internal_state pointer casts. Algorithm and tables are unchanged.
#include "retail_headers.h"
extern "C" {
int ZEXPORT inflateInit_(z_streamp strm, const char *version, int stream_size)
{
    return inflateInit2_(strm, DEF_WBITS, version, stream_size);
}
}
