#pragma once
// Retail zlib 32-bit stream ABI, from Ego_r z_stream_s and compressor accesses.
#include "rebuild_abi.h"
struct internal_state;
struct z_stream_s
{
    unsigned char* next_in;
    unsigned int avail_in;
    unsigned long total_in;
    unsigned char* next_out;
    unsigned int avail_out;
    unsigned long total_out;
    char* msg;
    internal_state* state;
    void* (__cdecl *zalloc)(void*, unsigned int, unsigned int);
    void (__cdecl *zfree)(void*, void*);
    void* opaque;
    int data_type;
    unsigned long adler;
    unsigned long reserved;
};
FABLE_STATIC_ASSERT(sizeof(z_stream_s) == 0x38);

extern "C" {
int __cdecl z_inflateInit_(z_stream_s*, const char*, int);
int __cdecl z_inflateReset(z_stream_s*);
int __cdecl z_inflate(z_stream_s*, int);
int __cdecl z_inflateEnd(z_stream_s*);
int __cdecl z_deflateEnd(z_stream_s*);
int __cdecl z_deflateInit_(z_stream_s*, int, const char*, int);
int __cdecl z_deflateInit2_(z_stream_s*, int, int, int, int, int, const char*, int);
int __cdecl z_deflate(z_stream_s*, int);
}
