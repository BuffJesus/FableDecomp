// Adapted from zlib 1.2.1 (https://zlib.net/fossils/zlib-1.2.1.tar.gz).
// Copyright (C) 1995-2003 Mark Adler. See zlib.h for the license.
// C++ adaptation: ANSI parameter lists, `this` renamed to `here`, explicit
// internal_state pointer casts. Algorithm and tables are unchanged.
#include "retail_headers.h"
extern "C" {
int ZEXPORT inflateEnd(z_streamp strm)
{
    struct inflate_state FAR *state;
    if (strm == Z_NULL || strm->state == Z_NULL || strm->zfree == (free_func)0)
        return Z_STREAM_ERROR;
    state = (struct inflate_state FAR *)strm->state;
    if (state->window != Z_NULL) ZFREE(strm, state->window);
    ZFREE(strm, strm->state);
    strm->state = Z_NULL;
    Tracev((stderr, "inflate: end\n"));
    return Z_OK;
}
}
