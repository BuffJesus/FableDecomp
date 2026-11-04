// Adapted from zlib 1.2.1 (https://zlib.net/fossils/zlib-1.2.1.tar.gz).
// Copyright (C) 1995-2003 Mark Adler. See zlib.h for the license.
// C++ adaptation: ANSI parameter lists, `this` renamed to `here`, explicit
// internal_state pointer casts. Algorithm and tables are unchanged.
#include "retail_headers.h"
extern "C" {
int ZEXPORT inflateReset(z_streamp strm)
{
    struct inflate_state FAR *state;

    if (strm == Z_NULL || strm->state == Z_NULL) return Z_STREAM_ERROR;
    state = (struct inflate_state FAR *)strm->state;
    strm->total_in = strm->total_out = state->total = 0;
    strm->msg = Z_NULL;
    state->mode = HEAD;
    state->last = 0;
    state->havedict = 0;
    state->wsize = 0;
    state->whave = 0;
    state->hold = 0;
    state->bits = 0;
    state->lencode = state->distcode = state->next = state->codes;
    Tracev((stderr, "inflate: reset\n"));
    return Z_OK;
}
}
